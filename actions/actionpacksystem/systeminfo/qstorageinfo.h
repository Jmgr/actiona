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

#ifndef QSTORAGEINFO_H
#define QSTORAGEINFO_H

#include "qsysteminfoglobal.h"
#include <QtCore/qobject.h>
#include <QtCore/qstringlist.h>

QT_BEGIN_NAMESPACE

class QStorageInfo_CustomPrivate;

class Q_SYSTEMINFO_EXPORT QStorageInfo_Custom : public QObject
{
    Q_OBJECT
    Q_ENUMS(DriveType)
    Q_PROPERTY(QStringList allLogicalDrives READ allLogicalDrives NOTIFY logicalDriveChanged)

public:
    enum DriveType {
        UnknownDrive = 0,
        InternalDrive,
        RemovableDrive,
        RemoteDrive,
        CdromDrive,
        RamDrive
    };

    QStorageInfo_Custom(QObject *parent = nullptr);
    ~QStorageInfo_Custom() override;

    QStringList allLogicalDrives();

    Q_INVOKABLE qlonglong availableDiskSpace(const QString &drive) const;
    Q_INVOKABLE qlonglong totalDiskSpace(const QString &drive) const;
    Q_INVOKABLE QString uriForDrive(const QString &drive) const;
    Q_INVOKABLE QStorageInfo_Custom::DriveType driveType(const QString &drive) const;

Q_SIGNALS:
    void logicalDriveChanged(const QString &drive, bool added);

protected:
    void connectNotify(const QMetaMethod &signal) override;
    void disconnectNotify(const QMetaMethod &signal) override;

private:
    Q_DISABLE_COPY(QStorageInfo_Custom)
    QStorageInfo_CustomPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QStorageInfo_Custom)
};

QT_END_NAMESPACE

#endif // QSTORAGEINFO_H
