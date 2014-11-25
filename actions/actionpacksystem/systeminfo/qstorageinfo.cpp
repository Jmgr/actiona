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

#include "qstorageinfo.h"

#include <QtCore/qstringlist.h>

#if defined(Q_OS_LINUX)
#  include "linux/qstorageinfo_linux_p.h"
#elif defined(Q_OS_WIN)
#  include "windows/qstorageinfo_win_p.h"
#elif defined(Q_OS_MAC)
#  include "mac/qstorageinfo_mac_p.h"
#else
QT_BEGIN_NAMESPACE
class QStorageInfoPrivate
{
public:
    QStorageInfoPrivate(QStorageInfo *) {}

    qlonglong availableDiskSpace(const QString &) { return -1; }
    qlonglong totalDiskSpace(const QString &) { return -1; }
    QString uriForDrive(const QString &) { return QString(); }
    QStringList allLogicalDrives() { return QStringList(); }
    QStorageInfo::DriveType driveType(const QString &) { return QStorageInfo::UnknownDrive; }
};
QT_END_NAMESPACE
#endif

#include <QtCore/qmetaobject.h>

QT_BEGIN_NAMESPACE

/*!
    \class QStorageInfo
    \inmodule QtSystemInfo
    \ingroup systeminfo
    \brief The QStorageInfo class provides various disk storage information about the system.
*/

/*!
    \enum QStorageInfo::DriveType
    This enum describes the type of drive or volume

    \value UnknownDrive         Drive type unknown.
    \value InternalDrive        Internal mass storage drive, e.g. hard drive.
    \value RemovableDrive       Removable drive, e.g. memory card.
    \value RemoteDrive          Network drive.
    \value CdromDrive           CD-ROM.
    \value RamDrive             Virtual drive made in RAM memory.
*/

/*!
    \fn void QStorageInfo::logicalDriveChanged(const QString &drive, bool added)

    This signal gets emitted when a new \a drive storage has been \a added or removed.
*/

/*!
    Constructs a QStorageInfo object with the given \a parent.
*/
QStorageInfo::QStorageInfo(QObject *parent)
    : QObject(parent)
    , d_ptr(new QStorageInfoPrivate(this))
{
}

/*!
    Destroys the object
*/
QStorageInfo::~QStorageInfo()
{
}

/*!
    \property QStorageInfo::allLogicalDrives
    \brief The  list of all drives.

    Returns a list of all logical drives or volumes, or an empty list if no drives are found or
    on error.
*/
QStringList QStorageInfo::allLogicalDrives()
{
    return d_ptr->allLogicalDrives();
}

/*!
    Returns the available free space on the \a drive, in bytes. -1 is returned if the drive doesn't
    exist, or the information is not available.
*/
qlonglong QStorageInfo::availableDiskSpace(const QString &drive) const
{
    return d_ptr->availableDiskSpace(drive);
}

/*!
    Returns the total space of the \a drive, in bytes. -1 is returned if the drive doesn't exist,
    or the information is not available.
*/
qlonglong QStorageInfo::totalDiskSpace(const QString &drive) const
{
    return d_ptr->totalDiskSpace(drive);
}

/*!
    Returns the URI, or unique identifier for the given \a drive. If the drive doesn't exist, or the
    information is not available, an empty string is returned.
*/
QString QStorageInfo::uriForDrive(const QString &drive) const
{
    return d_ptr->uriForDrive(drive);
}

/*!
    Returns the type of the given \a drive.
*/
QStorageInfo::DriveType QStorageInfo::driveType(const QString &drive) const
{
    return d_ptr->driveType(drive);
}

extern QMetaMethod proxyToSourceSignal(const QMetaMethod &, QObject *);

/*!
    \internal
*/
void QStorageInfo::connectNotify(const QMetaMethod &signal)
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN) || defined(Q_OS_MAC)
    QMetaMethod sourceSignal = proxyToSourceSignal(signal, d_ptr);
    connect(d_ptr, sourceSignal, this, signal, Qt::UniqueConnection);
#else
    Q_UNUSED(signal)
#endif
}

/*!
    \internal
*/
void QStorageInfo::disconnectNotify(const QMetaMethod &signal)
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN) || defined(Q_OS_MAC)
    // We can only disconnect with the private implementation, when there is no receivers for the signal.
    if (isSignalConnected(signal))
        return;

    QMetaMethod sourceSignal = proxyToSourceSignal(signal, d_ptr);
    disconnect(d_ptr, sourceSignal, this, signal);
#else
    Q_UNUSED(signal)
#endif
}

QT_END_NAMESPACE
