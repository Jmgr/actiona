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

#include "qsysteminfo_simulator_p.h"
#include "qsysteminfobackend_simulator_p.h"
#include "qsysteminfoconnection_simulator_p.h"

#if !defined(QT_NO_OFONO)
#  include "qnetworkinfo_linux_p.h"
#endif

#include <QtCore/qmetaobject.h>

QT_BEGIN_NAMESPACE

// QBatteryInfoSimulator

QBatteryInfoSimulator::QBatteryInfoSimulator(QBatteryInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , batteryInfoSimulatorBackend(QBatteryInfoSimulatorBackend::getSimulatorBackend())
{
    SystemInfoConnection::ensureSimulatorConnection();
}

QBatteryInfoSimulator::QBatteryInfoSimulator(int batteryIndex, QBatteryInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , batteryInfoSimulatorBackend(QBatteryInfoSimulatorBackend::getSimulatorBackend())
{
    SystemInfoConnection::ensureSimulatorConnection();
    setBatteryIndex(batteryIndex);
}

QBatteryInfoSimulator::~QBatteryInfoSimulator()
{
}

int QBatteryInfoSimulator::batteryCount()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getBatteryCount();

    return -1;
}

int QBatteryInfoSimulator::batteryIndex() const
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getBatteryIndex();

    return -1;
}

bool QBatteryInfoSimulator::isValid()
{
    // valid if the index < total count.
    return (batteryIndex() >= 0) && (batteryIndex() < batteryCount());
}

void QBatteryInfoSimulator::setBatteryIndex(int batteryIndex)
{
    if (batteryInfoSimulatorBackend)
        batteryInfoSimulatorBackend->setBatteryIndex(batteryIndex);
}

int QBatteryInfoSimulator::level()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getLevel(batteryInfoSimulatorBackend->getBatteryIndex());

    return -1;
}

int QBatteryInfoSimulator::currentFlow(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getCurrentFlow(battery);

    return 0;
}

int QBatteryInfoSimulator::currentFlow()
{
    return currentFlow(batteryInfoSimulatorBackend->getBatteryIndex());
}

int QBatteryInfoSimulator::cycleCount()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getCycleCount(batteryInfoSimulatorBackend->getBatteryIndex());

    return -1;
}

int QBatteryInfoSimulator::maximumCapacity(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getMaximumCapacity(battery);

    return -1;
}

int QBatteryInfoSimulator::maximumCapacity()
{
    return maximumCapacity(batteryInfoSimulatorBackend->getBatteryIndex());
}

int QBatteryInfoSimulator::remainingCapacity(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getRemainingCapacity(battery);

    return -1;
}

int QBatteryInfoSimulator::remainingCapacity()
{
    return remainingCapacity(batteryInfoSimulatorBackend->getBatteryIndex());
}

int QBatteryInfoSimulator::remainingChargingTime(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getRemainingChargingTime(battery);

    return -1;
}

int QBatteryInfoSimulator::remainingChargingTime()
{
    return remainingChargingTime(batteryInfoSimulatorBackend->getBatteryIndex());
}

int QBatteryInfoSimulator::voltage(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getVoltage(battery);

    return -1;
}

int QBatteryInfoSimulator::voltage()
{
    return voltage(batteryInfoSimulatorBackend->getBatteryIndex());
}

QBatteryInfo::ChargerType QBatteryInfoSimulator::chargerType()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getChargerType();

    return QBatteryInfo::UnknownCharger;
}

QBatteryInfo::ChargingState QBatteryInfoSimulator::chargingState(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getChargingState(battery);

    return QBatteryInfo::UnknownChargingState;
}

QBatteryInfo::ChargingState QBatteryInfoSimulator::chargingState()
{
    return chargingState(batteryInfoSimulatorBackend->getBatteryIndex());
}

QBatteryInfo::LevelStatus QBatteryInfoSimulator::levelStatus(int battery)
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getLevelStatus(battery);

    return QBatteryInfo::LevelUnknown;
}

QBatteryInfo::LevelStatus QBatteryInfoSimulator::levelStatus()
{
    return levelStatus(batteryInfoSimulatorBackend->getBatteryIndex());
}

QBatteryInfo::Health QBatteryInfoSimulator::health()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getHealth(batteryInfoSimulatorBackend->getBatteryIndex());

    return QBatteryInfo::HealthUnknown;
}

float QBatteryInfoSimulator::temperature()
{
    if (batteryInfoSimulatorBackend)
        return batteryInfoSimulatorBackend->getTemperature(batteryInfoSimulatorBackend->getBatteryIndex());

    return -1.0f;
}

extern QMetaMethod proxyToSourceSignal(const QMetaMethod &, QObject *);

void QBatteryInfoSimulator::connectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod batteryCountChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::batteryCountChanged);
    static const QMetaMethod chargerTypeChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::chargerTypeChanged);
    static const QMetaMethod chargingStateChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::chargingStateChanged);
    static const QMetaMethod currentFlowChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::currentFlowChanged);
    static const QMetaMethod remainingCapacityChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::remainingCapacityChanged);
    static const QMetaMethod remainingChargingTimeChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::remainingChargingTimeChanged);
    static const QMetaMethod voltageChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::voltageChanged);
    static const QMetaMethod levelStatusChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::levelStatusChanged);

    if (batteryInfoSimulatorBackend && (signal == batteryCountChangedSignal
                                        || signal == currentFlowChangedSignal
                                        || signal == voltageChangedSignal
                                        || signal == remainingCapacityChangedSignal
                                        || signal == remainingChargingTimeChangedSignal
                                        || signal == chargerTypeChangedSignal
                                        || signal == chargingStateChangedSignal
                                        || signal == levelStatusChangedSignal)) {
        QMetaMethod sourceSignal = proxyToSourceSignal(signal, batteryInfoSimulatorBackend);
        connect(batteryInfoSimulatorBackend, sourceSignal, this, signal);
    }
}

void QBatteryInfoSimulator::disconnectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod batteryCountChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::batteryCountChanged);
    static const QMetaMethod chargerTypeChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::chargerTypeChanged);
    static const QMetaMethod chargingStateChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::chargingStateChanged);
    static const QMetaMethod currentFlowChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::currentFlowChanged);
    static const QMetaMethod remainingCapacityChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::remainingCapacityChanged);
    static const QMetaMethod remainingChargingTimeChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::remainingChargingTimeChanged);
    static const QMetaMethod voltageChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::voltageChanged);
    static const QMetaMethod levelStatusChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoSimulator::levelStatusChanged);

    if (batteryInfoSimulatorBackend && (signal == batteryCountChangedSignal
                                        || signal == currentFlowChangedSignal
                                        || signal == voltageChangedSignal
                                        || signal == remainingCapacityChangedSignal
                                        || signal == remainingChargingTimeChangedSignal
                                        || signal == chargerTypeChangedSignal
                                        || signal == chargingStateChangedSignal
                                        || signal == levelStatusChangedSignal)) {
        QMetaMethod sourceSignal = proxyToSourceSignal(signal, batteryInfoSimulatorBackend);
        disconnect(batteryInfoSimulatorBackend, sourceSignal, this, signal);
    }
}


// QDeviceInfoSimulator

QDeviceInfoSimulator::QDeviceInfoSimulator(QDeviceInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , deviceInfoSimulatorBackend(QDeviceInfoSimulatorBackend::getSimulatorBackend())
{
    SystemInfoConnection::ensureSimulatorConnection();
}

QDeviceInfoSimulator::~QDeviceInfoSimulator()
{
}

bool QDeviceInfoSimulator::hasFeature(QDeviceInfo::Feature feature)
{
    if (deviceInfoSimulatorBackend)
        return deviceInfoSimulatorBackend->hasFeature(feature);

    return false;
}

QDeviceInfo::LockTypeFlags QDeviceInfoSimulator::activatedLocks()
{
    if (deviceInfoSimulatorBackend)
        return deviceInfoSimulatorBackend->getActivatedLocks();

    return QDeviceInfo::NoLock;
}

QDeviceInfo::LockTypeFlags QDeviceInfoSimulator::enabledLocks()
{
    if (deviceInfoSimulatorBackend)
        return deviceInfoSimulatorBackend->getEnabledLocks();

    return QDeviceInfo::NoLock;
}

QDeviceInfo::ThermalState QDeviceInfoSimulator::thermalState()
{
    if (deviceInfoSimulatorBackend)
        return deviceInfoSimulatorBackend->getThermalState();

    return QDeviceInfo::UnknownThermal;
}

int QDeviceInfoSimulator::imeiCount()
{
    if (deviceInfoSimulatorBackend)
        return deviceInfoSimulatorBackend->getImeiCount();

    return -1;
}

QString QDeviceInfoSimulator::imei(int interface)
{
    if (deviceInfoSimulatorBackend)
        return deviceInfoSimulatorBackend->getImei(interface);

    return QString();
}

QString QDeviceInfoSimulator::manufacturer()
{
    if (deviceInfoSimulatorBackend)
        return deviceInfoSimulatorBackend->getManufacturer();

    return QString();
}

QString QDeviceInfoSimulator::model()
{
    if (deviceInfoSimulatorBackend)
        return deviceInfoSimulatorBackend->getModel();

    return QString();
}

QString QDeviceInfoSimulator::productName()
{
    if (deviceInfoSimulatorBackend)
        return deviceInfoSimulatorBackend->getProductName();

    return QString();
}

QString QDeviceInfoSimulator::uniqueDeviceID()
{
    if (deviceInfoSimulatorBackend)
        return deviceInfoSimulatorBackend->getUniqueDeviceID();

    return QString();
}

QString QDeviceInfoSimulator::version(QDeviceInfo::Version type)
{
    if (deviceInfoSimulatorBackend)
        return deviceInfoSimulatorBackend->getVersion(type);

    return QString();
}

QString QDeviceInfoSimulator::operatingSystemName()
{
    return QString();
}

QString QDeviceInfoSimulator::boardName()
{
    return QString();
}

void QDeviceInfoSimulator::connectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod activatedLocksChangedSignal = QMetaMethod::fromSignal(&QDeviceInfoSimulator::activatedLocksChanged);
    static const QMetaMethod enabledLocksChangedSignal = QMetaMethod::fromSignal(&QDeviceInfoSimulator::enabledLocksChanged);
    static const QMetaMethod thermalStateChangedSignal = QMetaMethod::fromSignal(&QDeviceInfoSimulator::thermalStateChanged);

    if (signal == activatedLocksChangedSignal || signal == enabledLocksChangedSignal) {
        if (deviceInfoSimulatorBackend) {
            QMetaMethod sourceSignal = proxyToSourceSignal(signal, deviceInfoSimulatorBackend);
            connect(deviceInfoSimulatorBackend, sourceSignal, this, signal);
        }
        return;
    }

    if (deviceInfoSimulatorBackend && signal == thermalStateChangedSignal) {
        QMetaMethod sourceSignal = proxyToSourceSignal(signal, deviceInfoSimulatorBackend);
        connect(deviceInfoSimulatorBackend, sourceSignal, this, signal);
    }
}

void QDeviceInfoSimulator::disconnectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod activatedLocksChangedSignal = QMetaMethod::fromSignal(&QDeviceInfoSimulator::activatedLocksChanged);
    static const QMetaMethod enabledLocksChangedSignal = QMetaMethod::fromSignal(&QDeviceInfoSimulator::enabledLocksChanged);
    static const QMetaMethod thermalStateChangedSignal = QMetaMethod::fromSignal(&QDeviceInfoSimulator::thermalStateChanged);

    if (signal == activatedLocksChangedSignal || signal == enabledLocksChangedSignal) {
        if (deviceInfoSimulatorBackend) {
            QMetaMethod sourceSignal = proxyToSourceSignal(signal, deviceInfoSimulatorBackend);
            disconnect(deviceInfoSimulatorBackend, sourceSignal, this, signal);
        }
        return;
    }

    if (deviceInfoSimulatorBackend && signal == thermalStateChangedSignal) {
        QMetaMethod sourceSignal = proxyToSourceSignal(signal, deviceInfoSimulatorBackend);
        disconnect(deviceInfoSimulatorBackend, sourceSignal, this, signal);
    }
}

bool QDeviceInfoSimulator::currentBluetoothPowerState()
{
    return false;
}

// QNetworkInfoSimulator

QNetworkInfoSimulator::QNetworkInfoSimulator(QNetworkInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , networkInfoSimulatorBackend(QNetworkInfoSimulatorBackend::getSimulatorBackend())
#if !defined(QT_NO_OFONO)
    , d_ptr(new QNetworkInfoPrivate())
#endif
{
    SystemInfoConnection::ensureSimulatorConnection();
}

QNetworkInfoSimulator::~QNetworkInfoSimulator()
{
#if !defined(QT_NO_OFONO)
    delete d_ptr;
#endif
}

int QNetworkInfoSimulator::networkInterfaceCount(QNetworkInfo::NetworkMode mode)
{
#if !defined(QT_NO_OFONO)
    if (mode != QNetworkInfo::WlanMode) {
        if (d_ptr)
            return d_ptr->networkInterfaceCount(mode);
        else
            return -1;
    }
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getNetworkInterfaceCount(mode);
    return -1;
}

int QNetworkInfoSimulator::networkSignalStrength(QNetworkInfo::NetworkMode mode, int interface)
{
#if !defined(QT_NO_OFONO)
    if (mode != QNetworkInfo::WlanMode) {
        if (d_ptr)
            return d_ptr->networkSignalStrength(mode, interface);
        else
            return -1;
    }
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getNetworkSignalStrength(mode, interface);
    return -1;
}

QNetworkInfo::CellDataTechnology QNetworkInfoSimulator::currentCellDataTechnology(int interface)
{
#if !defined(QT_NO_OFONO)
        if (d_ptr)
            return d_ptr->currentCellDataTechnology(interface);
        else
            return QNetworkInfo::UnknownDataTechnology;
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getCurrentCellDataTechnology(interface);
    return QNetworkInfo::UnknownDataTechnology;
}

QNetworkInfo::NetworkMode QNetworkInfoSimulator::currentNetworkMode()
{
    QNetworkInfo::NetworkMode mode = QNetworkInfo::UnknownMode;
    if (networkInfoSimulatorBackend)
        mode = networkInfoSimulatorBackend->getCurrentNetworkMode();
#if !defined(QT_NO_OFONO)
    QNetworkInfo::NetworkMode mode2 = QNetworkInfo::UnknownMode;
        if (d_ptr)
            mode2 = d_ptr->currentNetworkMode();
        switch (mode) {
        case QNetworkInfo::WlanMode:
            if (mode2 == QNetworkInfo::EthernetMode)
                mode = mode2;
            break;
        default:
            mode = mode2;
            break;
        }

#endif
    return mode;
}

QNetworkInfo::NetworkStatus QNetworkInfoSimulator::networkStatus(QNetworkInfo::NetworkMode mode, int interface)
{
#if !defined(QT_NO_OFONO)
    if (mode != QNetworkInfo::WlanMode) {
        if (d_ptr)
            return d_ptr->networkStatus(mode, interface);
        else
            return QNetworkInfo::UnknownStatus;
    }
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getNetworkStatus(mode, interface);
    return QNetworkInfo::UnknownStatus;
}

#ifndef QT_NO_NETWORKINTERFACE
QNetworkInterface QNetworkInfoSimulator::interfaceForMode(QNetworkInfo::NetworkMode mode, int interface)
{
#if !defined(QT_NO_OFONO)
    if (mode != QNetworkInfo::WlanMode) {
        if (d_ptr)
            return d_ptr->interfaceForMode(mode, interface);
        else
            return QNetworkInterface();
    }
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getInterfaceForMode(mode, interface);
    return QNetworkInterface();
}
#endif // QT_NO_NETWORKINTERFACE

QString QNetworkInfoSimulator::cellId(int interface)
{
#if !defined(QT_NO_OFONO)
    if (d_ptr)
        return d_ptr->cellId(interface);
    else
        return QString();
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getCellId(interface);
    return QString();
}

QString QNetworkInfoSimulator::currentMobileCountryCode(int interface)
{
#if !defined(QT_NO_OFONO)
    if (d_ptr)
        return d_ptr->currentMobileCountryCode(interface);
    else
        return QString();
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getCurrentMobileCountryCode(interface);
    return QString();
}

QString QNetworkInfoSimulator::currentMobileNetworkCode(int interface)
{
#if !defined(QT_NO_OFONO)
    if (d_ptr)
        return d_ptr->currentMobileNetworkCode(interface);
    else
        return QString();
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getCurrentMobileNetworkCode(interface);
    return QString();
}

QString QNetworkInfoSimulator::homeMobileCountryCode(int interface)
{
#if !defined(QT_NO_OFONO)
    if (d_ptr)
        return d_ptr->homeMobileCountryCode(interface);
    else
        return QString();
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getHomeMobileCountryCode(interface);
    return QString();
}

QString QNetworkInfoSimulator::homeMobileNetworkCode(int interface)
{
#if !defined(QT_NO_OFONO)
    if (d_ptr)
        return d_ptr->homeMobileNetworkCode(interface);
    else
        return QString();
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getHomeMobileNetworkCode(interface);
    return QString();
}

QString QNetworkInfoSimulator::imsi(int interface)
{
#if !defined(QT_NO_OFONO)
    if (d_ptr)
        return d_ptr->imsi(interface);
    else
        return QString();
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getImsi(interface);
    return QString();
}

QString QNetworkInfoSimulator::locationAreaCode(int interface)
{
#if !defined(QT_NO_OFONO)
    if (d_ptr)
        return d_ptr->locationAreaCode(interface);
    else
        return QString();
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getLocationAreaCode(interface);
    return QString();
}

QString QNetworkInfoSimulator::macAddress(QNetworkInfo::NetworkMode mode, int interface)
{
#if !defined(QT_NO_OFONO)
    if (mode != QNetworkInfo::WlanMode) {
        if (d_ptr)
            return d_ptr->macAddress(mode, interface);
        else
            return QString();
    }
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getMacAddress(mode, interface);
    return QString();
}

QString QNetworkInfoSimulator::networkName(QNetworkInfo::NetworkMode mode, int interface)
{
#if !defined(QT_NO_OFONO)
    if (mode != QNetworkInfo::WlanMode) {
        if (d_ptr)
            return d_ptr->networkName(mode, interface);
        else
            return QString();
    }
#endif
    if (networkInfoSimulatorBackend)
        return networkInfoSimulatorBackend->getNetworkName(mode, interface);
    return QString();
}

void QNetworkInfoSimulator::connectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod networkInterfaceCountChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoSimulator::networkInterfaceCountChanged);
    static const QMetaMethod currentNetworkModeChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoSimulator::currentNetworkModeChanged);
    static const QMetaMethod networkNameChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoSimulator::networkNameChanged);
    static const QMetaMethod networkSignalStrengthChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoSimulator::networkSignalStrengthChanged);
    static const QMetaMethod networkStatusChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoSimulator::networkStatusChanged);

#if !defined(QT_NO_OFONO)
    if (signal == networkInterfaceCountChangedSignal) {
        if (networkInfoSimulatorBackend)
            connect(networkInfoSimulatorBackend, SIGNAL(networkInterfaceCountChanged(QNetworkInfo::NetworkMode,int)), this, SLOT(onNetworkInterfaceCountChanged(QNetworkInfo::NetworkMode,int)), Qt::UniqueConnection);
        if (d_ptr)
            connect(d_ptr, SIGNAL(networkInterfaceCountChanged(QNetworkInfo::NetworkMode,int)), this, SLOT(onNetworkInterfaceCountChanged(QNetworkInfo::NetworkMode,int)), Qt::UniqueConnection);
        return;
    } else if (signal == currentNetworkModeChangedSignal) {
        if (networkInfoSimulatorBackend)
            connect(networkInfoSimulatorBackend, SIGNAL(currentNetworkModeChanged(QNetworkInfo::NetworkMode)), this, SLOT(onCurrentNetworkModeChanged(QNetworkInfo::NetworkMode)), Qt::UniqueConnection);
        if (d_ptr)
            connect(d_ptr, SIGNAL(currentNetworkModeChanged(QNetworkInfo::NetworkMode)), this, SLOT(onCurrentNetworkModeChanged(QNetworkInfo::NetworkMode)), Qt::UniqueConnection);
        return;
    } else if (signal == networkNameChangedSignal) {
        if (networkInfoSimulatorBackend)
            connect(networkInfoSimulatorBackend, SIGNAL(networkNameChanged(QNetworkInfo::NetworkMode,int,QString)), this, SLOT(onNetworkNameChanged(QNetworkInfo::NetworkMode,int,QString)), Qt::UniqueConnection);
        if (d_ptr)
            connect(d_ptr, SIGNAL(networkNameChanged(QNetworkInfo::NetworkMode,int,QString)), this, SLOT(onNetworkNameChanged(QNetworkInfo::NetworkMode,int,QString)), Qt::UniqueConnection);
        return;
    } else if (signal == networkSignalStrengthChangedSignal) {
        if (networkInfoSimulatorBackend)
            connect(networkInfoSimulatorBackend, SIGNAL(networkSignalStrengthChanged(QNetworkInfo::NetworkMode,int,int)), this, SLOT(onNetworkSignalStrengthChanged(QNetworkInfo::NetworkMode,int,int)), Qt::UniqueConnection);
        if (d_ptr)
            connect(d_ptr, SIGNAL(networkSignalStrengthChanged(QNetworkInfo::NetworkMode,int,int)), this, SLOT(onNetworkSignalStrengthChanged(QNetworkInfo::NetworkMode,int,int)), Qt::UniqueConnection);
        return;
    } else if (signal == networkStatusChangedSignal) {
        if (networkInfoSimulatorBackend)
            connect(networkInfoSimulatorBackend, SIGNAL(networkStatusChanged(QNetworkInfo::NetworkMode,int,QNetworkInfo::NetworkStatus)), this, SLOT(onNetworkStatusChanged(QNetworkInfo::NetworkMode,int,QNetworkInfo::NetworkStatus)), Qt::UniqueConnection);
        if (d_ptr)
            connect(d_ptr, SIGNAL(networkStatusChanged(QNetworkInfo::NetworkMode,int,QNetworkInfo::NetworkStatus)), this, SLOT(onNetworkStatusChanged(QNetworkInfo::NetworkMode,int,QNetworkInfo::NetworkStatus)), Qt::UniqueConnection);
        return;
    }
#endif
    if (networkInfoSimulatorBackend) {
        QMetaMethod sourceSignal = proxyToSourceSignal(signal, networkInfoSimulatorBackend);
        connect(networkInfoSimulatorBackend, sourceSignal, this, signal, Qt::UniqueConnection);
    }
}

void QNetworkInfoSimulator::disconnectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod networkInterfaceCountChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoSimulator::networkInterfaceCountChanged);
    static const QMetaMethod currentNetworkModeChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoSimulator::currentNetworkModeChanged);
    static const QMetaMethod networkNameChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoSimulator::networkNameChanged);
    static const QMetaMethod networkSignalStrengthChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoSimulator::networkSignalStrengthChanged);
    static const QMetaMethod networkStatusChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoSimulator::networkStatusChanged);

#if !defined(QT_NO_OFONO)
    if (signal == networkInterfaceCountChangedSignal) {
        if (networkInfoSimulatorBackend)
            disconnect(networkInfoSimulatorBackend, SIGNAL(networkInterfaceCountChanged(QNetworkInfo::NetworkMode,int)), this, SLOT(onNetworkInterfaceCountChanged(QNetworkInfo::NetworkMode,int)));
        if (d_ptr)
            disconnect(d_ptr, SIGNAL(networkInterfaceCountChanged(QNetworkInfo::NetworkMode,int)), this, SLOT(onNetworkInterfaceCountChanged(QNetworkInfo::NetworkMode,int)));
        return;
    } else if (signal == currentNetworkModeChangedSignal) {
        if (networkInfoSimulatorBackend)
            disconnect(networkInfoSimulatorBackend, SIGNAL(currentNetworkModeChanged(QNetworkInfo::NetworkMode)), this, SLOT(onCurrentNetworkModeChanged(QNetworkInfo::NetworkMode)));
        if (d_ptr)
            disconnect(d_ptr, SIGNAL(currentNetworkModeChanged(QNetworkInfo::NetworkMode)), this, SLOT(onCurrentNetworkModeChanged(QNetworkInfo::NetworkMode)));
        return;
    } else if (signal == networkNameChangedSignal) {
        if (networkInfoSimulatorBackend)
            disconnect(networkInfoSimulatorBackend, SIGNAL(networkNameChanged(QNetworkInfo::NetworkMode,int,QString)), this, SLOT(onNetworkNameChanged(QNetworkInfo::NetworkMode,int,QString)));
        if (d_ptr)
            disconnect(d_ptr, SIGNAL(networkNameChanged(QNetworkInfo::NetworkMode,int,QString)), this, SLOT(onNetworkNameChanged(QNetworkInfo::NetworkMode,int,QString)));
        return;
    } else if (signal == networkSignalStrengthChangedSignal) {
        if (networkInfoSimulatorBackend)
            disconnect(networkInfoSimulatorBackend, SIGNAL(networkSignalStrengthChanged(QNetworkInfo::NetworkMode,int,int)), this, SLOT(onNetworkSignalStrengthChanged(QNetworkInfo::NetworkMode,int,int)));
        if (d_ptr)
            disconnect(d_ptr, SIGNAL(networkSignalStrengthChanged(QNetworkInfo::NetworkMode,int,int)), this, SLOT(onNetworkSignalStrengthChanged(QNetworkInfo::NetworkMode,int,int)));
        return;
    } else if (signal == networkStatusChangedSignal) {
        if (networkInfoSimulatorBackend)
            disconnect(networkInfoSimulatorBackend, SIGNAL(networkStatusChanged(QNetworkInfo::NetworkMode,int,QNetworkInfo::NetworkStatus)), this, SLOT(onNetworkStatusChanged(QNetworkInfo::NetworkMode,int,QNetworkInfo::NetworkStatus)));
        if (d_ptr)
            disconnect(d_ptr, SIGNAL(networkStatusChanged(QNetworkInfo::NetworkMode,int,QNetworkInfo::NetworkStatus)), this, SLOT(onNetworkStatusChanged(QNetworkInfo::NetworkMode,int,QNetworkInfo::NetworkStatus)));
        return;
    }
#endif
    if (networkInfoSimulatorBackend) {
        QMetaMethod sourceSignal = proxyToSourceSignal(signal, networkInfoSimulatorBackend);
        disconnect(networkInfoSimulatorBackend, sourceSignal, this, signal);
    }
}

#if !defined(QT_NO_OFONO)

void QNetworkInfoSimulator::onCurrentNetworkModeChanged(QNetworkInfo::NetworkMode mode)
{
    Q_UNUSED(mode)
    emit currentNetworkModeChanged(currentNetworkMode());
}

void QNetworkInfoSimulator::onNetworkInterfaceCountChanged(QNetworkInfo::NetworkMode mode, int count)
{
    if (strcmp(sender()->metaObject()->className(), "QNetworkInfoSimulatorBackend") == 0) {
        if (mode == QNetworkInfo::WlanMode)
            emit networkInterfaceCountChanged(mode, count);
    } else {
        if (mode != QNetworkInfo::WlanMode)
            emit networkInterfaceCountChanged(mode, count);
    }
}

void QNetworkInfoSimulator::onNetworkNameChanged(QNetworkInfo::NetworkMode mode, int interface, const QString &name)
{
    if (strcmp(sender()->metaObject()->className(), "QNetworkInfoSimulatorBackend") == 0) {
        if (mode == QNetworkInfo::WlanMode)
            emit networkNameChanged(mode, interface, name);
    } else {
        if (mode != QNetworkInfo::WlanMode)
            emit networkNameChanged(mode, interface, name);
    }
}

void QNetworkInfoSimulator::onNetworkSignalStrengthChanged(QNetworkInfo::NetworkMode mode, int interface, int strength)
{
    if (strcmp(sender()->metaObject()->className(), "QNetworkInfoSimulatorBackend") == 0) {
        if (mode == QNetworkInfo::WlanMode)
            emit networkSignalStrengthChanged(mode, interface, strength);
    } else {
        if (mode != QNetworkInfo::WlanMode)
            emit networkSignalStrengthChanged(mode, interface, strength);
    }
}

void QNetworkInfoSimulator::onNetworkStatusChanged(QNetworkInfo::NetworkMode mode, int interface, QNetworkInfo::NetworkStatus status)
{
    if (strcmp(sender()->metaObject()->className(), "QNetworkInfoSimulatorBackend") == 0) {
        if (mode == QNetworkInfo::WlanMode)
            emit networkStatusChanged(mode, interface, status);
    } else {
        if (mode != QNetworkInfo::WlanMode)
            emit networkStatusChanged(mode, interface, status);
    }
}

#endif

QT_END_NAMESPACE
