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

#include "qsysteminfodata_simulator_p.h"
#include <QtCore/QDataStream>

QT_BEGIN_NAMESPACE

void qt_registerSystemInfoTypes()
{
    qRegisterMetaTypeStreamOperators<QNetworkInfoData>("QNetworkInfoData");
    qRegisterMetaTypeStreamOperators<QNetworkInfoData::BasicNetworkInfo>("QNetworkInfoData::BasicNetworkInfo");
    qRegisterMetaTypeStreamOperators<QNetworkInfoData::EthernetInfo>("QNetworkInfoData::EthernetInfo");
    qRegisterMetaTypeStreamOperators<QNetworkInfoData::WLanInfo>("QNetworkInfoData::WLanInfo");
    qRegisterMetaTypeStreamOperators<QNetworkInfoData::CellularInfo>("QNetworkInfoData::CellularInfo");
    qRegisterMetaTypeStreamOperators<QNetworkInfoData::BluetoothInfo>("QNetworkInfoData::BluetoothInfo");
    qRegisterMetaTypeStreamOperators<QDeviceInfoData>("QDeviceInfoData");
    qRegisterMetaTypeStreamOperators<QBatteryInfoData>("QBatteryInfoData");
    qRegisterMetaTypeStreamOperators<QStorageInfoData>("QStorageInfoData");
    qRegisterMetaTypeStreamOperators<QStorageInfoData::DriveInfo>("QStorageInfoData::DriveInfo");
    qRegisterMetaTypeStreamOperators<QScreenSaverData>("QScreenSaverData");
}

QDataStream &operator<<(QDataStream &out, const QNetworkInfoData::BasicNetworkInfo &s)
{
    out << s.name;
    out << static_cast<qint32>(s.signalStrength) << static_cast<qint32>(s.mode)
        << static_cast<qint32>(s.status);
    return out;
}

QDataStream &operator>>(QDataStream &in, QNetworkInfoData::BasicNetworkInfo &s)
{
    in >> s.name;
    qint32 signalStrength;
    in >> signalStrength;
    s.signalStrength = signalStrength;
    qint32 mode, status;
    in >> mode;
    in >> status;
    s.mode = static_cast<QNetworkInfo::NetworkMode>(mode);
    s.status = static_cast<QNetworkInfo::NetworkStatus>(status);
    return in;
}

QDataStream &operator<<(QDataStream &out, const QNetworkInfoData::EthernetInfo &s)
{
    out << s.basicNetworkInfo;
    out << s.macAddress;

    return out;
}

QDataStream &operator>>(QDataStream &in, QNetworkInfoData::EthernetInfo &s)
{
    in >> s.basicNetworkInfo;
    in >> s.macAddress;

    return in;
}

QDataStream &operator<<(QDataStream &out, const QNetworkInfoData::WLanInfo &s)
{
    out << s.basicNetworkInfo;
    out << s.macAddress;
    return out;
}

QDataStream &operator>>(QDataStream &in, QNetworkInfoData::WLanInfo &s)
{
    in >> s.basicNetworkInfo;
    in >> s.macAddress;
    return in;
}

QDataStream &operator<<(QDataStream &out, const QNetworkInfoData::BluetoothInfo &s)
{
    out << s.basicNetworkInfo;
    out << s.btAddress;
    return out;
}

QDataStream &operator>>(QDataStream &in, QNetworkInfoData::BluetoothInfo &s)
{
    in >> s.basicNetworkInfo;
    in >> s.btAddress;
    return in;
}

QDataStream &operator<<(QDataStream &out, const QNetworkInfoData::CellularInfo &s)
{
    out << s.basicNetworkInfo;
    out << s.imsi << s.cellId << s.locationAreaCode;
    out << s.currentMobileCountryCode << s.currentMobileNetworkCode;
    out << s.homeMobileCountryCode << s.homeMobileNetworkCode;
    out << static_cast<qint32>(s.cellData);

    return out;
}

QDataStream &operator>>(QDataStream &in, QNetworkInfoData::CellularInfo &s)
{
    in >> s.basicNetworkInfo;
    qint32 cellData;
    in >> s.imsi >> s.cellId >> s.locationAreaCode;
    in >> s.currentMobileCountryCode >> s.currentMobileNetworkCode;
    in >> s.homeMobileCountryCode >> s.homeMobileNetworkCode;
    in >> cellData;
    s.cellData = static_cast<QNetworkInfo::CellDataTechnology>(cellData);

    return in;
}

QDataStream &operator<<(QDataStream &out, const QNetworkInfoData &s)
{
    out << s.ethernetInfo;
    out << s.wLanInfo;
    out << s.cellularInfo;
    out << s.bluetoothInfo;

    return out;
}

QDataStream &operator>>(QDataStream &in, QNetworkInfoData &s)
{
    in >> s.ethernetInfo;
    in >> s.wLanInfo;
    in >> s.cellularInfo;
    in >> s.bluetoothInfo;

    return in;
}

QDataStream &operator<<(QDataStream &out, const QDeviceInfoData &s)
{
    out << s.manufacturer << s.model << s.productName << s.uniqueDeviceID;
    out << s.imeiList;
    out << s.featureList;
    out << s.versionList;
    out << static_cast<qint32>(s.enabledLocks) << static_cast<qint32>(s.activatedLocks)
        << static_cast<qint32>(s.currentThermalState);

    return out;
}

QDataStream &operator>>(QDataStream &in, QDeviceInfoData &s)
{
    in >> s.manufacturer >> s.model >> s.productName >> s.uniqueDeviceID;
    in >> s.imeiList;
    in >> s.featureList;
    in >> s.versionList;
    qint32 enabledLocks, activatedLocks, currentThermalState;
    in >> enabledLocks >> activatedLocks >> currentThermalState;
    s.enabledLocks = static_cast<QDeviceInfo::LockTypeFlags>(enabledLocks);
    s.activatedLocks = static_cast<QDeviceInfo::LockTypeFlags>(activatedLocks);
    s.currentThermalState = static_cast<QDeviceInfo::ThermalState>(currentThermalState);

    return in;
}

QDataStream &operator<<(QDataStream &out, const QDeviceInfo::Feature s)
{
    out << static_cast<qint32>(s);
    return out;
}

QDataStream &operator>>(QDataStream &in, QDeviceInfo::Feature &s)
{
    qint32 v;
    in >> v;
    s = static_cast<QDeviceInfo::Feature>(v);
    return in;
}

QDataStream &operator<<(QDataStream &out, const QDeviceInfo::Version s)
{
    out << static_cast<qint32>(s);
    return out;
}

QDataStream &operator>>(QDataStream &in, QDeviceInfo::Version &s)
{
    qint32 v;
    in >> v;
    s = static_cast<QDeviceInfo::Version>(v);
    return in;
}

QDataStream &operator<<(QDataStream &out, const QBatteryInfoData &s)
{
    out << static_cast<qint32>(s.chargingState) << static_cast<qint32>(s.chargerType)
        << static_cast<qint32>(s.levelStatus) << static_cast<qint32>(s.levelStatus);;

    out << static_cast<qint32>(s.currentFlow) << static_cast<qint32>(s.maximumCapacity)
        << static_cast<qint32>(s.remainingCapacity) << static_cast<qint32>(s.remainingChargingTime)
        << static_cast<qint32>(s.voltage);

    return out;
}

QDataStream &operator>>(QDataStream &in, QBatteryInfoData &s)
{
    qint32 chargingState, chargerType, batteryStatus;
    in >> chargingState >> chargerType >> batteryStatus;

    s.chargingState = static_cast<QBatteryInfo::ChargingState>(chargingState);
    s.chargerType = static_cast<QBatteryInfo::ChargerType>(chargerType);
    s.levelStatus = static_cast<QBatteryInfo::LevelStatus>(batteryStatus);

    qint32 currentFlow, maximumCapacity, remainingCapacity, remainingChargingTime, voltage;
    in >> currentFlow >> maximumCapacity >> remainingCapacity >> remainingChargingTime >> voltage;

    s.currentFlow = currentFlow;
    s.maximumCapacity = maximumCapacity;
    s.remainingCapacity = remainingCapacity;
    s.remainingChargingTime = remainingChargingTime;
    s.voltage = voltage;

    return in;
}

QDataStream &operator<<(QDataStream &out, const QStorageInfoData &s)
{
    out << s.drives;
    return out;
}

QDataStream &operator>>(QDataStream &in, QStorageInfoData &s)
{
    in >> s.drives;
    return in;
}

QDataStream &operator<<(QDataStream &out, const QStorageInfoData::DriveInfo &s)
{
    out << s.totalSpace << s.availableSpace;
    out << s.uri;
    out << static_cast<qint32>(s.type);

    return out;
}

QDataStream &operator>>(QDataStream &in, QStorageInfoData::DriveInfo &s)
{
    in >> s.totalSpace >> s.availableSpace;
    in >> s.uri;
    qint32 type;
    in >> type;
    s.type = static_cast<QStorageInfo_Custom::DriveType>(type);

    return in;
}

QDataStream &operator<<(QDataStream &out, const QScreenSaverData &s)
{
    out << s.screenSaverEnabled;
    return out;
}

QDataStream &operator>>(QDataStream &in, QScreenSaverData &s)
{
    in >> s.screenSaverEnabled;
    return in;
}

QT_END_NAMESPACE


