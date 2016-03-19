/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Copyright (C) 2014 BlackBerry Limited. All rights reserved.
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

#ifndef QSYSTEMINFODATA_SIMULATOR_P_H
#define QSYSTEMINFODATA_SIMULATOR_P_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qnetworkinfo.h>
#include <qdeviceinfo.h>
#include <qbatteryinfo.h>
#include <qstorageinfo.h>
#include <qscreensaver.h>

#include <QHash>
#include <QVector>
#include <QString>
#include <QMetaType>

QT_BEGIN_NAMESPACE

struct QNetworkInfoData
{
    struct BasicNetworkInfo {
        QString name;
        int signalStrength;
        QNetworkInfo::NetworkMode mode;
        QNetworkInfo::NetworkStatus status;
    };

    struct EthernetInfo
    {
        BasicNetworkInfo basicNetworkInfo;
        QString macAddress;
    };

    struct WLanInfo
    {
        BasicNetworkInfo basicNetworkInfo;
        QString macAddress;
    };

    struct CellularInfo {
        BasicNetworkInfo basicNetworkInfo;

        QString imsi;
        QString cellId;
        QString locationAreaCode;

        QString currentMobileCountryCode;
        QString currentMobileNetworkCode;
        QString homeMobileCountryCode;
        QString homeMobileNetworkCode;
        QNetworkInfo::CellDataTechnology cellData;
    };

    struct BluetoothInfo {
        BasicNetworkInfo basicNetworkInfo;
        QString btAddress;
    };

    QVector<EthernetInfo> ethernetInfo;
    QVector<WLanInfo> wLanInfo;
    QVector<CellularInfo> cellularInfo;
    QVector<BluetoothInfo> bluetoothInfo;
};

struct QBatteryInfoData
{
    int index;
    int currentFlow;
    int cycleCount;
    int maximumCapacity;
    int remainingCapacity;
    int remainingChargingTime;
    int voltage;

    QBatteryInfo::ChargingState chargingState;
    QBatteryInfo::ChargerType chargerType;
    QBatteryInfo::LevelStatus levelStatus;
    QBatteryInfo::Health health;
    float temperature;
};

struct QDeviceInfoData
{
    QString manufacturer;
    QString model;
    QString productName;
    QString uniqueDeviceID;

    QDeviceInfo::LockTypeFlags enabledLocks;
    QDeviceInfo::LockTypeFlags activatedLocks;
    QDeviceInfo::ThermalState currentThermalState;

    QHash<QDeviceInfo::Feature, bool> featureList;
    QList<QString> imeiList;
    QMap<QDeviceInfo::Version, QString> versionList;
};

struct QStorageInfoData
{
    struct DriveInfo
    {
      qint64 totalSpace;
      qint64 availableSpace;
      QString uri;
      QStorageInfo_Custom::DriveType type;
    };
    QHash<QString, DriveInfo> drives;
};

struct QScreenSaverData
{
    bool screenSaverEnabled;
};


Q_DECLARE_METATYPE(QNetworkInfoData)
Q_DECLARE_METATYPE(QDeviceInfoData)
Q_DECLARE_METATYPE(QBatteryInfoData)
Q_DECLARE_METATYPE(QStorageInfoData)
Q_DECLARE_METATYPE(QScreenSaverData)

void qt_registerSystemInfoTypes();

QDataStream &operator<<(QDataStream &out, const QNetworkInfoData::BasicNetworkInfo &s);
QDataStream &operator>>(QDataStream &in, QNetworkInfoData::BasicNetworkInfo &s);
QDataStream &operator<<(QDataStream &out, const QNetworkInfoData::EthernetInfo &s);
QDataStream &operator>>(QDataStream &in, QNetworkInfoData::EthernetInfo &s);
QDataStream &operator<<(QDataStream &out, const QNetworkInfoData::WLanInfo &s);
QDataStream &operator>>(QDataStream &in, QNetworkInfoData::WLanInfo &s);
QDataStream &operator<<(QDataStream &out, const QNetworkInfoData::BluetoothInfo &s);
QDataStream &operator>>(QDataStream &in, QNetworkInfoData::BluetoothInfo &s);
QDataStream &operator<<(QDataStream &out, const QNetworkInfoData::CellularInfo &s);
QDataStream &operator>>(QDataStream &in, QNetworkInfoData::CellularInfo &s);
QDataStream &operator<<(QDataStream &out, const QNetworkInfoData &s);
QDataStream &operator>>(QDataStream &in, QNetworkInfoData &s);
QDataStream &operator<<(QDataStream &out, const QDeviceInfoData &s);
QDataStream &operator>>(QDataStream &in, QDeviceInfoData &s);
QDataStream &operator<<(QDataStream &out, const QDeviceInfo::Feature s);
QDataStream &operator>>(QDataStream &in, QDeviceInfo::Feature &s);
QDataStream &operator<<(QDataStream &out, const QDeviceInfo::Version s);
QDataStream &operator>>(QDataStream &in, QDeviceInfo::Version &s);
QDataStream &operator<<(QDataStream &out, const QBatteryInfoData &s);
QDataStream &operator>>(QDataStream &in, QBatteryInfoData &s);
QDataStream &operator<<(QDataStream &out, const QStorageInfoData &s);
QDataStream &operator>>(QDataStream &in, QStorageInfoData &s);
QDataStream &operator<<(QDataStream &out, const QStorageInfoData::DriveInfo &s);
QDataStream &operator>>(QDataStream &in, QStorageInfoData::DriveInfo &s);
QDataStream &operator<<(QDataStream &out, const QScreenSaverData &s);
QDataStream &operator>>(QDataStream &in, QScreenSaverData &s);

QT_END_NAMESPACE

#endif // QSYSTEMINFODATA_SIMULATOR_P_H
