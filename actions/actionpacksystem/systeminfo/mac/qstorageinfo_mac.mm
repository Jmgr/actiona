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

#include "qstorageinfo_mac_p.h"

#include <QtCore/private/qcore_mac_p.h>
#include <QtCore/qmetaobject.h>
#include <QDebug>

#include <CoreFoundation/CoreFoundation.h>
#include <DiskArbitration/DiskArbitration.h>
#include <ApplicationServices/ApplicationServices.h>
#include <DiskArbitration/DASession.h>
#include <IOKit/storage/IOMedia.h>
#include <IOKit/storage/IOCDMedia.h>
#include <IOKit/storage/IODVDMedia.h>
#include <IOKit/storage/IOBlockStorageDevice.h>
#include <Foundation/NSDictionary.h>
#include <CoreFoundation/CFUUID.h>

#include <sys/mount.h>

DAApprovalSessionRef session = NULL;

void mountCallback(DADiskRef disk, CFArrayRef /*keys*/, void *context)
{
    NSDictionary *batDoctionary;
    batDoctionary = (NSDictionary *)DADiskCopyDescription(disk);
    NSURL *volumePath = [[batDoctionary objectForKey:(NSString *)kDADiskDescriptionVolumePathKey] copy];

    QString name = QCFString::toQString([volumePath path]);

    static_cast<QStorageInfoPrivate*>(context)->storageChanged(true, name);
}

void mountCallback2(DADiskRef diskRef, void *context)
{
    DADiskRef wholeDisk;
    wholeDisk = DADiskCopyWholeDisk(diskRef);
// only deal with whole disks here.. i.e. cdroms
    if (wholeDisk) {
        io_service_t mediaService;

        mediaService = DADiskCopyIOMedia(wholeDisk);
        if (mediaService) {
            if (IOObjectConformsTo(mediaService, kIOCDMediaClass) || IOObjectConformsTo(mediaService, kIODVDMediaClass)) {

                NSDictionary *batDoctionary;
                batDoctionary = (NSDictionary *)DADiskCopyDescription(diskRef);
                NSURL *volumePath = [[batDoctionary objectForKey:(NSString *)kDADiskDescriptionVolumePathKey] copy];

                QString name = QCFString::toQString([volumePath path]);

                static_cast<QStorageInfoPrivate*>(context)->storageChanged(true, name);
                CFRelease(batDoctionary);
            }
        }
        IOObjectRelease(mediaService);
        CFRelease(wholeDisk);
    }
}

void unmountCallback(DADiskRef disk, void *context)
{
    NSDictionary *batDoctionary;
    batDoctionary = (NSDictionary *)DADiskCopyDescription(disk);
    NSURL *volumePath = [[batDoctionary objectForKey:(NSString *)kDADiskDescriptionVolumePathKey] copy];

    QString name = QCFString::toQString([volumePath path]);

    static_cast<QStorageInfoPrivate*>(context)->storageChanged(false,name);
}



QT_BEGIN_NAMESPACE

QDASessionThread::QDASessionThread(QObject *parent)
    :QObject(parent), session(NULL)
{
    moveToThread(&t);
    t.start();
    if (session == NULL)
        session = DASessionCreate(kCFAllocatorDefault);
}

QDASessionThread::~QDASessionThread()
{
}

void QDASessionThread::stop()
{
    QMutexLocker locker(&mutex);
    CFRunLoopStop(CFRunLoopGetCurrent());
    keepRunning = false;
    t.quit();
    t.wait();
}

void QDASessionThread::doWork()
{
#ifndef MAC_SDK_10_5
    if (QThread::currentThread() != &t) {
        QMetaObject::invokeMethod(this, "doWork",
                                  Qt::QueuedConnection);
    } else {

        mutex.lock();
        keepRunning = true;
        mutex.unlock();
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

        DASessionScheduleWithRunLoop(session,CFRunLoopGetCurrent(),kCFRunLoopDefaultMode);

        SInt32 result;
        while (keepRunning &&
               (result = CFRunLoopRunInMode(kCFRunLoopDefaultMode ,1, YES))) {
        }

        DASessionUnscheduleFromRunLoop(session, CFRunLoopGetCurrent(),kCFRunLoopDefaultMode);
        [pool release];
    }
#endif
}

QStorageInfoPrivate::QStorageInfoPrivate(QStorageInfo *parent)
    : QObject(parent)
    , q_ptr(parent), daSessionThread(0),sessionThreadStarted(0)
{
    updateVolumesMap();
}

QStorageInfoPrivate::~QStorageInfoPrivate()
{
    if (sessionThreadStarted) {
        daSessionThread->stop();
        delete daSessionThread;
    }
}

qlonglong QStorageInfoPrivate::availableDiskSpace(const QString &driveVolume)
{
    qint64 totalFreeBytes=0;
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSDictionary *attr = [ [NSFileManager defaultManager] attributesOfFileSystemForPath:QCFString::toNSString(driveVolume) error:nil];
    totalFreeBytes = [[attr objectForKey:NSFileSystemFreeSize] doubleValue];
    [pool release];

    return  totalFreeBytes;
}

qlonglong QStorageInfoPrivate::totalDiskSpace(const QString &driveVolume)
{
    qint64 totalBytes=0;
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    NSString *vol = QCFString::toNSString(driveVolume);
    NSDictionary *attr = [ [NSFileManager defaultManager] attributesOfFileSystemForPath:vol error:nil];
    totalBytes = [[attr objectForKey:NSFileSystemSize] doubleValue];
    [pool release];

    return totalBytes;
}
QString QStorageInfoPrivate::uriForDrive(const QString &driveVolume)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    DADiskRef diskRef;
    DASessionRef sessionRef;
    CFDictionaryRef descriptionDictionary;

    sessionRef = DASessionCreate(NULL);
    if (sessionRef == NULL) {
        return QString();
    }

    diskRef = DADiskCreateFromBSDName(NULL, sessionRef, mountEntriesMap.key(driveVolume).toLatin1());
    if (diskRef == NULL) {
        CFRelease(sessionRef);
        return QString();
    }

    descriptionDictionary = DADiskCopyDescription(diskRef);
    if (descriptionDictionary == NULL) {
        CFRelease(diskRef);
        CFRelease(sessionRef);
        return QString();
    }

    CFUUIDRef uuid;
    uuid = (const __CFUUID *)CFDictionaryGetValue(descriptionDictionary, kDADiskDescriptionVolumeUUIDKey);

    CFRelease(diskRef);
    CFRelease(sessionRef);
    QString uri = QCFString::toQString(CFUUIDCreateString(kCFAllocatorDefault, uuid));
    [pool release];

    return uri;
}

QStringList QStorageInfoPrivate::allLogicalDrives()
{
    updateVolumesMap();
    QStringList drivesList;
    QMapIterator<QString, QString> it(mountEntriesMap);
     while (it.hasNext()) {
         it.next();
         drivesList << it.value();
     }
     drivesList.sort();
    return drivesList;
}

QStorageInfo::DriveType QStorageInfoPrivate::driveType(const QString &driveVolume)
{
    QStorageInfo::DriveType drivetype =  QStorageInfo::UnknownDrive;

    DADiskRef diskRef;
    DASessionRef sessionRef;
    CFBooleanRef boolRef;
    CFBooleanRef boolRef2;
    CFDictionaryRef descriptionDictionary;

    sessionRef = DASessionCreate(NULL);
    if (sessionRef == NULL) {
        return QStorageInfo::UnknownDrive;
    }

    diskRef = DADiskCreateFromBSDName(NULL, sessionRef, mountEntriesMap.key(driveVolume).toLatin1());
    if (diskRef == NULL) {
        CFRelease(sessionRef);
        return QStorageInfo::UnknownDrive;
    }

    descriptionDictionary = DADiskCopyDescription(diskRef);
    if (descriptionDictionary == NULL) {
        CFRelease(diskRef);
        CFRelease(sessionRef);
        return QStorageInfo::RemoteDrive;
    }

    boolRef = (CFBooleanRef)
              CFDictionaryGetValue(descriptionDictionary, kDADiskDescriptionMediaRemovableKey);
    if (boolRef) {
        if (CFBooleanGetValue(boolRef)) {
            drivetype = QStorageInfo::RemovableDrive;
        } else {
            drivetype = QStorageInfo::InternalDrive;
        }
    }
    boolRef2 = (CFBooleanRef)
              CFDictionaryGetValue(descriptionDictionary, kDADiskDescriptionVolumeNetworkKey);
    if (boolRef2) {
        if (CFBooleanGetValue(boolRef2)) {
            drivetype = QStorageInfo::RemoteDrive;
        }
    }

    DADiskRef wholeDisk;
    wholeDisk = DADiskCopyWholeDisk(diskRef);

    if (wholeDisk) {
        io_service_t mediaService;

        mediaService = DADiskCopyIOMedia(wholeDisk);
        if (mediaService) {
            if (IOObjectConformsTo(mediaService, kIOCDMediaClass)) {
                drivetype = QStorageInfo::CdromDrive;
            }
            if (IOObjectConformsTo(mediaService, kIODVDMediaClass)) {
                drivetype = QStorageInfo::CdromDrive;
            }
            IOObjectRelease(mediaService);
        }
        CFRelease(wholeDisk);
    }
    CFRelease(diskRef);
    CFRelease(descriptionDictionary);
    CFRelease(boolRef);
    CFRelease(boolRef2);
    CFRelease(sessionRef);

    return drivetype;
}

void QStorageInfoPrivate::connectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod logicalDriveChangedSignal = QMetaMethod::fromSignal(&QStorageInfoPrivate::logicalDriveChanged);
    if (signal == logicalDriveChangedSignal) {
        ///
        sessionThread();

        DARegisterDiskDescriptionChangedCallback(daSessionThread->session,kDADiskDescriptionMatchVolumeMountable,
                                                 kDADiskDescriptionWatchVolumePath, mountCallback,this);
        DARegisterDiskAppearedCallback(daSessionThread->session,kDADiskDescriptionMatchVolumeMountable,mountCallback2,this);
        DARegisterDiskDisappearedCallback(daSessionThread->session,kDADiskDescriptionMatchVolumeMountable,unmountCallback,this);
        sessionThreadStarted = true;
    }
}

void QStorageInfoPrivate::disconnectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod logicalDriveChangedSignal = QMetaMethod::fromSignal(&QStorageInfoPrivate::logicalDriveChanged);
    if (signal == logicalDriveChangedSignal) {
    }
}


void QStorageInfoPrivate::storageChanged( bool added, const QString &vol)
{
    if (!vol.isEmpty()) {
        QMapIterator<QString, QString> it(mountEntriesMap);
        QString foundKey;
        bool seen = false;
        while (it.hasNext()) {
            it.next();
            if ( vol == it.value()) {
                seen = true;
                foundKey = it.key();
            }
        }

        if (added && !seen) {
            Q_EMIT logicalDriveChanged(vol, added);
            updateVolumesMap();
        }
        if (!added && seen) {
            mountEntriesMap.remove(foundKey);
            Q_EMIT logicalDriveChanged(vol, added);
        }


    } else {
        if (added) {

        } else { //removed
            // cdroms unmounting seem to not have a volume name with the notification here, so
            // we need to manually deal with it
            QMap <QString,QString> oldDrives = mountEntriesMap;
            updateVolumesMap();
            QStringList newDrives = mountEntriesMap.keys();
            QString foundDrive;

            QMapIterator<QString, QString> it(oldDrives);
            while (it.hasNext()) {
                it.next();
                if (!newDrives.contains(it.key())) {
                    Q_EMIT logicalDriveChanged(it.value(), added);
                }
            }
        }
    }
}

bool QStorageInfoPrivate::updateVolumesMap()
{
    struct statfs *buf = NULL;
    unsigned i, count = 0;

    mountEntriesMap.clear();

    count = getmntinfo(&buf, 0);
    for (i=0; i<count; i++) {
        char *volName = buf[i].f_mntonname;

        if (buf[i].f_type != 19
           && buf[i].f_type != 20
           && !mountEntriesMap.contains(volName)) {
            mountEntriesMap.insert(buf[i].f_mntfromname,volName);
//            qDebug() << __FUNCTION__ << "insert" << buf[i].f_mntfromname<< volName;
        }
    }
    return true;
}

bool QStorageInfoPrivate::sessionThread()
{
    if (!sessionThreadStarted) {
        daSessionThread = new QDASessionThread();
        sessionThreadStarted = true;
    }
    daSessionThread->doWork();
    return true;
}

QT_END_NAMESPACE

