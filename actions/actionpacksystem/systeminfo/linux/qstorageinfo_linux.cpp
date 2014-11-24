/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSystems module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qstorageinfo_linux_p.h"

#include <QtCore/qfile.h>
#include <QtCore/qdir.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qsocketnotifier.h>

#include <errno.h>
#include <mntent.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <sys/inotify.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <unistd.h>
#include <paths.h>
#include <mntent.h>

#if !defined(QT_NO_UDEV)
#include "qudevwrapper_p.h"
#endif // QT_NO_UDEV

#if !defined(QT_NO_UDISKS)
#include <QtDBus/qdbusconnection.h>
#include <QtDBus/qdbusinterface.h>
#include <QtDBus/qdbusreply.h>
#endif // QT_NO_UDISKS

QT_BEGIN_NAMESPACE

QStorageInfoPrivate::QStorageInfoPrivate(QStorageInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , inotifyWatcher(-1)
    , inotifyFileDescriptor(-1)
    , notifier(0)
#if !defined(QT_NO_UDEV)
    , udevWrapper(0)
    , needsUDevWatcher(-1)
#endif // QT_NO_UDEV
{
}

QStorageInfoPrivate::~QStorageInfoPrivate()
{
    cleanupWatcher();
}

qlonglong QStorageInfoPrivate::availableDiskSpace(const QString &drive)
{
#ifdef __USE_LARGEFILE64
    struct statfs64 statistics;
    if (statfs64(drive.toLatin1(), &statistics) == 0) {
        qlonglong blockSize = statistics.f_bsize;
        qlonglong availBlocks = statistics.f_bavail;
        return availBlocks * blockSize;
    }
#else
    struct statfs statistics;
    if (statfs(drive.toLatin1(), &statistics) == 0) {
        qlonglong blockSize = statistics.f_bsize;
        qlonglong availBlocks = statistics.f_bavail;
        return availBlocks * blockSize;
    }
#endif
    return -1;
}

qlonglong QStorageInfoPrivate::totalDiskSpace(const QString &drive)
{
#ifdef __USE_LARGEFILE64
    struct statfs64 statistics;
    if (statfs64(drive.toLatin1(), &statistics) == 0) {
        qlonglong blockSize = statistics.f_bsize;
        qlonglong totalBlocks = statistics.f_blocks;
        return totalBlocks * blockSize;
    }
#else
    struct statfs statistics;
    if (statfs(drive.toLatin1(), &statistics) == 0) {
        qlonglong blockSize = statistics.f_bsize;
        qlonglong totalBlocks = statistics.f_blocks;
        return totalBlocks * blockSize;
    }
#endif
    return -1;
}

QString QStorageInfoPrivate::uriForDrive(const QString &drive)
{
    QFileInfoList fileinfolist = QDir(QString(QStringLiteral("/dev/disk/by-uuid/"))).entryInfoList(QDir::AllEntries | QDir::NoDot | QDir::NoDotDot);
    if (!fileinfolist.isEmpty()) {
        FILE *fsDescription = setmntent(_PATH_MOUNTED, "r");
        struct mntent entry;
        char buffer[512];
        QString uri;
        while ((getmntent_r(fsDescription, &entry, buffer, sizeof(buffer))) != NULL) {
            if (drive != QString::fromLatin1(entry.mnt_dir))
                continue;
            int idx = fileinfolist.indexOf(QString::fromLatin1(entry.mnt_fsname));
            if (idx != -1)
                uri = fileinfolist[idx].fileName();
            break;
        }
        endmntent(fsDescription);

        if (!uri.isEmpty())
            return uri;
    }

#if !defined(QT_NO_UDISKS)
    QDBusReply<QList<QDBusObjectPath> > reply = QDBusConnection::systemBus().call(
                QDBusMessage::createMethodCall(QString(QStringLiteral("org.freedesktop.UDisks")),
                                               QString(QStringLiteral("/org/freedesktop/UDisks")),
                                               QString(QStringLiteral("org.freedesktop.UDisks")),
                                               QString(QStringLiteral("EnumerateDevices"))));
    if (reply.isValid()) {
        QList<QDBusObjectPath> paths = reply.value();
        foreach (const QDBusObjectPath &path, paths) {
            QDBusInterface interface(QString(QStringLiteral("org.freedesktop.UDisks")), path.path(),
                                     QString(QStringLiteral("org.freedesktop.DBus.Properties")),
                                     QDBusConnection::systemBus());
            if (!interface.isValid())
                continue;
            QDBusReply<QVariant> reply = interface.call(QString(QStringLiteral("Get")),
                                                        QString(QStringLiteral("org.freedesktop.UDisks.Device")),
                                                        QString(QStringLiteral("DeviceMountPaths")));
            if (reply.isValid() && reply.value().toString() == drive) {
                reply = interface.call(QString(QStringLiteral("Get")),
                                       QString(QStringLiteral("org.freedesktop.UDisks.Device")),
                                       QString(QStringLiteral("IdUuid")));
                if (reply.isValid())
                    return reply.value().toString();
            }
        }
    }
#endif // QT_NO_UDISKS

    return QString::null;
}

QStringList QStorageInfoPrivate::allLogicalDrives()
{
    // No need to update the list if someone is listening to the signal, as it will be updated in that case
#if !defined(QT_NO_UDEV)
    if (inotifyWatcher == -1 && needsUDevWatcher != 2)
#else
    if (inotifyWatcher == -1)
#endif // QT_NO_UDEV
        updateLogicalDrives();

    return logicalDrives;
}

QStorageInfo::DriveType QStorageInfoPrivate::driveType(const QString &drive)
{
    QStorageInfo::DriveType type = QStorageInfo::UnknownDrive;
    FILE *fsDescription = setmntent(_PATH_MOUNTED, "r");
    struct mntent entry;
    char buffer[512];
    while ((getmntent_r(fsDescription, &entry, buffer, sizeof(buffer))) != NULL) {
        if (drive != QString::fromLatin1(entry.mnt_dir))
            continue;

        if (strcmp(entry.mnt_type, "binfmt_misc") == 0
            || strcmp(entry.mnt_type, "debugfs") == 0
            || strcmp(entry.mnt_type, "devpts") == 0
            || strcmp(entry.mnt_type, "devtmpfs") == 0
            || strcmp(entry.mnt_type, "fusectl") == 0
            || strcmp(entry.mnt_type, "none") == 0
            || strcmp(entry.mnt_type, "proc") == 0
            || strcmp(entry.mnt_type, "ramfs") == 0
            || strcmp(entry.mnt_type, "securityfs") == 0
            || strcmp(entry.mnt_type, "sysfs") == 0
            || strcmp(entry.mnt_type, "tmpfs") == 0) {
            type = QStorageInfo::RamDrive;
            break;
        }

        if (strcmp(entry.mnt_type, "cifs") == 0
            || strcmp(entry.mnt_type, "ncpfs") == 0
            || strcmp(entry.mnt_type, "nfs") == 0
            || strcmp(entry.mnt_type, "nfs4") == 0
            || strcmp(entry.mnt_type, "smbfs") == 0) {
            type = QStorageInfo::RemoteDrive;
            break;
        }

        if (strcmp(entry.mnt_type, "iso9660") == 0) {
            type = QStorageInfo::CdromDrive;
            break;
        }

        if (strcmp(entry.mnt_type, "rootfs") == 0) {
            type = QStorageInfo::InternalDrive;
            break;
        }

        // Now need to guess if it's InternalDrive or RemovableDrive
        QString fsName(QString::fromLatin1(entry.mnt_fsname));
        if (fsName.contains(QString(QStringLiteral("mapper")))) {
            struct stat status;
            stat(entry.mnt_fsname, &status);
            fsName = QString(QStringLiteral("/sys/block/dm-%1/removable")).arg(status.st_rdev & 0377);
        } else {
 #if defined(QT_SIMULATOR)
            if (fsName.startsWith(QStringLiteral("/dev/loop")))
               return QStorageInfo::RemovableDrive;
#endif
            fsName = fsName.section(QString(QStringLiteral("/")), 2, 3);
            if (!fsName.isEmpty()) {
                if (fsName.length() > 3) {
                    // only take the parent of the device
                    QString fsNameCopy(fsName);

                    // Remove all trailing digits
                    while (fsNameCopy.at(fsNameCopy.size() - 1).isDigit())
                        fsNameCopy.chop(1);

                    // If we removed some digits previously
                    if (fsNameCopy.size() < fsName.size() && fsNameCopy.at(fsNameCopy.size() - 1) == QChar(QLatin1Char('p'))) {
                        fsNameCopy.chop(1);
                        fsName = fsNameCopy;
                    }

                    if (fsName.startsWith(QString(QStringLiteral("mmc")))) {
                        // "removable" attribute is set only for removable media, and we may have internal mmc cards
                        fsName = QString(QStringLiteral("/sys/block/")) + fsName + QString(QStringLiteral("/device/uevent"));
                        QFile file(fsName);
                        if (file.open(QIODevice::ReadOnly)) {
                            QByteArray buf = file.readLine();
                            while (buf.size() > 0) {
                                if (qstrncmp(buf.constData(), "MMC_TYPE=", 9) == 0) {
                                    if (qstrncmp(buf.constData() + 9, "MMC", 3) == 0)
                                        type = QStorageInfo::InternalDrive;
                                    else if (qstrncmp(buf.constData() + 9, "SD", 2) == 0)
                                        type = QStorageInfo::RemovableDrive;
                                    if (type != QStorageInfo::UnknownDrive) {
                                        endmntent(fsDescription);
                                        return type;
                                    }
                                    break;  // fall back to check the "removable" attribute
                                }
                                buf = file.readLine();
                            }
                        }
                    }
                }
                fsName = QString(QStringLiteral("/sys/block/")) + fsName + QString(QStringLiteral("/removable"));
            }
        }
        QFile removable(fsName);
        char isRemovable;
        if (!removable.open(QIODevice::ReadOnly) || 1 != removable.read(&isRemovable, 1))
            break;
        if (isRemovable == '0')
            type = QStorageInfo::InternalDrive;
        else
            type = QStorageInfo::RemovableDrive;
        break;
    }

    endmntent(fsDescription);
    return type;
}

void QStorageInfoPrivate::connectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod logicalDriveChangedSignal = QMetaMethod::fromSignal(&QStorageInfoPrivate::logicalDriveChanged);
    if (signal == logicalDriveChangedSignal)
        setupWatcher();
}

void QStorageInfoPrivate::disconnectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod logicalDriveChangedSignal = QMetaMethod::fromSignal(&QStorageInfoPrivate::logicalDriveChanged);
    if (signal == logicalDriveChangedSignal)
        cleanupWatcher();
}

void QStorageInfoPrivate::cleanupWatcher()
{
#if !defined(QT_NO_UDEV)
    if (needsUDevWatcher == 2) {
        needsUDevWatcher = 1;
        if (udevWrapper)
            disconnect(udevWrapper, SIGNAL(driveChanged()), this, SLOT(onDriveChanged()));
        return;
    }
#endif // QT_NO_UDEV

    if (notifier) {
        delete notifier;
        notifier = 0;
    }

    if (inotifyWatcher != -1) {
        inotify_rm_watch(inotifyFileDescriptor, inotifyWatcher);
        inotifyWatcher = -1;
    }

    if (inotifyFileDescriptor != -1) {
        close(inotifyFileDescriptor);
        inotifyFileDescriptor = -1;
    }
}

void QStorageInfoPrivate::setupWatcher()
{
    updateLogicalDrives();

#if !defined(QT_NO_UDEV)
    if (needsUDevWatcher == -1)
        needsUDevWatcher = QFileInfo(QStringLiteral("/etc/mtab")).isSymLink();

    if (needsUDevWatcher == 1) {
        if (!udevWrapper)
            udevWrapper = new QUDevWrapper(this);
        connect(udevWrapper, SIGNAL(driveChanged()), this, SLOT(onDriveChanged()));
        needsUDevWatcher = 2;
        return;
    }
#endif // QT_NO_UDEV

    if (inotifyFileDescriptor == -1
        && (inotifyFileDescriptor = inotify_init()) == -1) {
        return;
    }

    if (inotifyWatcher == -1
        && (inotifyWatcher = inotify_add_watch(inotifyFileDescriptor, _PATH_MOUNTED, IN_MODIFY)) == -1) {
        close(inotifyFileDescriptor);
        return;
    }

    if (notifier == 0) {
        notifier = new QSocketNotifier(inotifyFileDescriptor, QSocketNotifier::Read);
        connect(notifier, SIGNAL(activated(int)), this, SLOT(onInotifyActivated()));
    }
}

void QStorageInfoPrivate::updateLogicalDrives()
{
    FILE *fsDescription = setmntent(_PATH_MOUNTED, "r");
    struct mntent entry;
    char buffer[512];

    logicalDrives.clear();
    while (getmntent_r(fsDescription, &entry, buffer, sizeof(buffer)) != NULL)
        logicalDrives << QString::fromLatin1(entry.mnt_dir);
    endmntent(fsDescription);
}

void QStorageInfoPrivate::onInotifyActivated()
{
    inotify_event event;
    if (read(inotifyFileDescriptor, (void *)&event, sizeof(event)) > 0
        && event.wd == inotifyWatcher) {
        // Have to do this, otherwise I can't get further notification
        inotify_rm_watch(inotifyFileDescriptor, inotifyWatcher);
        inotifyWatcher = inotify_add_watch(inotifyFileDescriptor, _PATH_MOUNTED, IN_MODIFY);

        QStringList oldLogicalDrives = logicalDrives;
        updateLogicalDrives();

        foreach (const QString &drive, oldLogicalDrives) {
            if (!logicalDrives.contains(drive))
                emit logicalDriveChanged(drive, false);
        }

        foreach (const QString &drive, logicalDrives) {
            if (!oldLogicalDrives.contains(drive))
                emit logicalDriveChanged(drive, true);
        }
    }
}

#if !defined(QT_NO_UDEV)
void QStorageInfoPrivate::onDriveChanged()
{
    QStringList oldLogicalDrives = logicalDrives;
    updateLogicalDrives();

    foreach (const QString &drive, oldLogicalDrives) {
        if (!logicalDrives.contains(drive))
            emit logicalDriveChanged(drive, false);
    }

    foreach (const QString &drive, logicalDrives) {
        if (!oldLogicalDrives.contains(drive))
            emit logicalDriveChanged(drive, true);
    }
}
#endif // QT_NO_UDEV

QT_END_NAMESPACE
