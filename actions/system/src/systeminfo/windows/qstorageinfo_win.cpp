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

#include "qstorageinfo_win_p.h"

#if !defined( Q_CC_MINGW)
#ifndef Q_OS_WINCE
#include "qwmihelper_win_p.h"
#endif
#endif

#include <QtCore/qdir.h>
#include <QtCore/qmetaobject.h>

#include <qt_windows.h>

QT_BEGIN_NAMESPACE

QStorageInfo_CustomPrivate::QStorageInfo_CustomPrivate(QStorageInfo_Custom *parent)
    : QObject(parent)
    , q_ptr(parent)
{
#if !defined( Q_CC_MINGW)
#if !defined( Q_OS_WINCE)
    WMIHelper *wHelper;
    wHelper =  WMIHelper::instance();
    wHelper->setWmiNamespace(QLatin1String("root/cimv2"));
    wHelper->setClassName(QLatin1String("Win32_VolumeChangeEvent"));
    QString aString = QLatin1String("SELECT * FROM __InstanceOperationEvent WITHIN 1 WHERE (TargetInstance ISA 'Win32_LogicalDisk') AND (TargetInstance.DriveType = 5 OR TargetInstance.DriveType = 2)");

    wHelper->setupNotfication(QLatin1String("root/cimv2"),aString,QStringList());

    connect(wHelper,SIGNAL(wminotificationArrived()),this,SLOT(notificationArrived()));
#endif
#endif
}

QStorageInfo_CustomPrivate::~QStorageInfo_CustomPrivate()
{
}

qlonglong QStorageInfo_CustomPrivate::availableDiskSpace(const QString &drive)
{
    qlonglong availableBytes(-1);
    if (!GetDiskFreeSpaceEx((WCHAR *)drive.utf16(), 0, 0, (PULARGE_INTEGER)&availableBytes))
        availableBytes = -1;
    return availableBytes;
}

qlonglong QStorageInfo_CustomPrivate::totalDiskSpace(const QString &drive)
{
    qlonglong totalBytes(-1);
    if (!GetDiskFreeSpaceEx((WCHAR *)drive.utf16(), 0, (PULARGE_INTEGER)&totalBytes, 0))
        totalBytes = -1;
    return totalBytes;
}

QString QStorageInfo_CustomPrivate::uriForDrive(const QString &drive) {
    WCHAR uri[50] = {0};
    // Make sure the drive string is null-terminated
    if (GetVolumeNameForVolumeMountPoint(reinterpret_cast<LPCWSTR>(drive.utf16()), uri, 50)) {
        // Correctly cast to the expected type
        return QString::fromUtf16(reinterpret_cast<const char16_t *>(uri));
    }
    return QString();
}

QStringList QStorageInfo_CustomPrivate::allLogicalDrives()
{
    mountEntriesList.clear();
    QFileInfoList drives = QDir::drives();
    foreach (const QFileInfo &drive, drives)
        mountEntriesList << drive.absoluteFilePath();
    return mountEntriesList;
}

QStorageInfo_Custom::DriveType QStorageInfo_CustomPrivate::driveType(const QString &drive)
{
    UINT type = GetDriveType((WCHAR *)drive.utf16());
    switch (type) {
    case DRIVE_REMOVABLE:
        return QStorageInfo_Custom::RemovableDrive;
    case DRIVE_FIXED:
        return QStorageInfo_Custom::InternalDrive;
    case DRIVE_REMOTE:
        return QStorageInfo_Custom::RemoteDrive;
    case DRIVE_CDROM:
        return QStorageInfo_Custom::CdromDrive;
    case DRIVE_RAMDISK:
        return QStorageInfo_Custom::RamDrive;
    case DRIVE_UNKNOWN:
    case DRIVE_NO_ROOT_DIR:
    default:
        return QStorageInfo_Custom::UnknownDrive;
    };
}

void QStorageInfo_CustomPrivate::connectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod logicalDriveChangedSignal = QMetaMethod::fromSignal(&QStorageInfo_CustomPrivate::logicalDriveChanged);
    if (signal == logicalDriveChangedSignal) {
    }
}

void QStorageInfo_CustomPrivate::disconnectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod logicalDriveChangedSignal = QMetaMethod::fromSignal(&QStorageInfo_CustomPrivate::logicalDriveChanged);
    if (signal == logicalDriveChangedSignal) {
    }
}

void QStorageInfo_CustomPrivate::notificationArrived()
{
    QStringList oldDrives;

     oldDrives = mountEntriesList;
     allLogicalDrives();

     if (mountEntriesList.count() < oldDrives.count()) {
         QStringListIterator i(oldDrives);
          while (i.hasNext()) {
             QString key = i.next();
              if (!mountEntriesList.contains(key)) {
                  emit logicalDriveChanged(key, false);
              }
          }
      } else if (mountEntriesList.count() > oldDrives.count()) {
         QStringListIterator i(mountEntriesList);
          while (i.hasNext()) {
             QString key = i.next();

              if (oldDrives.contains(key))
                  continue;
                 emit logicalDriveChanged(key, true);
         }
     }
 }

QT_END_NAMESPACE
