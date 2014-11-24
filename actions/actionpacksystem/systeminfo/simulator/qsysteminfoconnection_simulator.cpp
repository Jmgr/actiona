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

#include "qsysteminfoconnection_simulator_p.h"
#include "qsysteminfobackend_simulator_p.h"

#include <QtSimulator/connection.h>
#include <QtSimulator/version.h>
#include <QtSimulator/connectionworker.h>
#include <QtSimulator/QtSimulator>

#include <QTimer>
#include <QEventLoop>
#include <QMutex>

QT_BEGIN_NAMESPACE

using namespace Simulator;

const QString SystemInfoConnection::SERVERNAME(QStringLiteral("QtSimulator_Mobility_ServerName1.3.0.0"));
const int SystemInfoConnection::PORT(0xbeef+1);
const Simulator::Version SystemInfoConnection::VERSION(1,3,0,0);

SystemInfoConnection::SystemInfoConnection(QObject *parent)
    : QObject(parent)
    , mInitialDataSent(false)
{
    qt_registerSystemInfoTypes();
    mConnection = new Connection(Connection::Client, SERVERNAME, PORT, VERSION, this);
    mWorker = mConnection->connectToServer(Connection::simulatorHostName(true), PORT);
    if (mWorker) {
        mWorker->addReceiver(this);
        mWorker->call("setRequestsSystemInfo");

        // wait until initial data is received
        QTimer timer;
        QEventLoop loop;
        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        connect(this, SIGNAL(initialDataReceived()), &loop, SLOT(quit()));
        timer.start(1000);
        loop.exec();
        timer.stop();
    } else
        qWarning("Could not connect to server");
}

SystemInfoConnection::~SystemInfoConnection()
{
    delete mWorker;
}

void SystemInfoConnection::ensureSimulatorConnection()
{
    static QMutex mutex;

    mutex.lock();
    static SystemInfoConnection systemInfoConnection;
    mutex.unlock();
}

void SystemInfoConnection::initialSystemInfoDataSent()
{
    mInitialDataSent = true;
    emit initialDataReceived();
}

void SystemInfoConnection::setBatteryInfoData(const QBatteryInfoData &data)
{
    QBatteryInfoSimulatorBackend *batteryInfoBackend = QBatteryInfoSimulatorBackend::getSimulatorBackend();

    batteryInfoBackend->setMaximumCapacity(data.maximumCapacity);
    batteryInfoBackend->setRemainingCapacity(data.remainingCapacity);
    batteryInfoBackend->setCurrentFlow(data.currentFlow);
    batteryInfoBackend->setCycleCount(data.cycleCount);
    batteryInfoBackend->setVoltage(data.voltage);
    batteryInfoBackend->setRemainingChargingTime(data.remainingChargingTime);
    batteryInfoBackend->setChargerType(data.chargerType);
    batteryInfoBackend->setChargingState(data.chargingState);
    batteryInfoBackend->setLevelStatus(data.levelStatus);
    batteryInfoBackend->setHealth(data.health);
    batteryInfoBackend->setTemperature(data.temperature);
}

void SystemInfoConnection::setDeviceInfoData(const QDeviceInfoData &data)
{
    QDeviceInfoSimulatorBackend *deviceInfoBackend = QDeviceInfoSimulatorBackend::getSimulatorBackend();

    deviceInfoBackend->setManufacturer(data.manufacturer);
    deviceInfoBackend->setModel(data.model);
    deviceInfoBackend->setProductName(data.productName);
    deviceInfoBackend->setActivatedLocks(data.activatedLocks);
    deviceInfoBackend->setEnabledLocks(data.enabledLocks);
    deviceInfoBackend->setUniqueDeviceID(data.uniqueDeviceID);
    deviceInfoBackend->setThermalState(data.currentThermalState);
    deviceInfoBackend->setVersion(QDeviceInfo::Os, data.versionList.value(QDeviceInfo::Os));
    deviceInfoBackend->setVersion(QDeviceInfo::Firmware, data.versionList.value(QDeviceInfo::Firmware));

    int imeiCount = deviceInfoBackend->getImeiCount();
    int receivedImeiCount = data.imeiList.count();
    if (imeiCount > receivedImeiCount) {
        int imeiIndex;
        for (imeiIndex = 0; imeiIndex < imeiCount; imeiIndex++)
            deviceInfoBackend->setImei(imeiIndex, QStringLiteral(""));
        for (imeiIndex = 0; imeiIndex < receivedImeiCount; imeiIndex++)
            deviceInfoBackend->setImei(imeiIndex, data.imeiList.value(imeiIndex));
    } else {
        for (int imeiIndex = 0; imeiIndex < imeiCount; imeiIndex++)
            deviceInfoBackend->setImei(imeiIndex, data.imeiList.value(imeiIndex));
    }

    QList<QDeviceInfo::Feature> featureKeys = data.featureList.keys();
    foreach (const QDeviceInfo::Feature &featureKey, featureKeys)
        deviceInfoBackend->setFeature(featureKey, data.featureList.value(featureKey));
}

void SystemInfoConnection::setNetworkInfoData(const QNetworkInfoData &data)
{
    QNetworkInfoSimulatorBackend *networkInfoBackend = QNetworkInfoSimulatorBackend::getSimulatorBackend();

    int actualCount = networkInfoBackend->getNetworkInterfaceCount(QNetworkInfo::WlanMode);
    int newCount = data.wLanInfo.count();

    if (newCount < actualCount) {
        for (int i = actualCount; i > newCount; i--)
            networkInfoBackend->removeInterface(QNetworkInfo::WlanMode, i - 1);
    } else if (newCount > actualCount) {
        for (int i = actualCount; i < newCount; i++) {
            QNetworkInfoData::WLanInfo info = {{QString(),
                                               -1,
                                               QNetworkInfo::WlanMode,
                                               QNetworkInfo::UnknownStatus},
                                               QString()};
            networkInfoBackend->addWlanInterface(info);
        }
    }

#if defined(QT_NO_OFONO)
    actualCount = networkInfoBackend->getNetworkInterfaceCount(QNetworkInfo::EthernetMode);
    newCount = data.ethernetInfo.count();
    if (newCount < actualCount) {
        for (int i = actualCount; i > newCount; i--)
            networkInfoBackend->removeInterface(QNetworkInfo::EthernetMode, i - 1);
    } else if (newCount > actualCount) {
        for (int i = actualCount; i < newCount; i++) {
            QNetworkInfoData::EthernetInfo info = {{QString(),
                                                   -1,
                                                   QNetworkInfo::EthernetMode,
                                                   QNetworkInfo::UnknownStatus},
                                                   QString()};
            networkInfoBackend->addEthernetInterface(info);
        }
    }

    actualCount = networkInfoBackend->getNetworkInterfaceCount(QNetworkInfo::BluetoothMode);
    newCount = data.bluetoothInfo.count();
    if (newCount < actualCount) {
        for (int i = actualCount; i > newCount; i--)
            networkInfoBackend->removeInterface(QNetworkInfo::BluetoothMode, i - 1);
    } else if (newCount > actualCount) {
        for (int i = actualCount; i < newCount; i++) {
            QNetworkInfoData::BluetoothInfo info = {{QString(),
                                                    -1,
                                                    QNetworkInfo::BluetoothMode,
                                                    QNetworkInfo::UnknownStatus},
                                                    QString()};
            networkInfoBackend->addBluetoothInterface(info);
        }
    }

    actualCount = networkInfoBackend->getNetworkInterfaceCount(QNetworkInfo::GsmMode);
    newCount = data.cellularInfo.count();
    if (newCount < actualCount) {
        for (int i = actualCount; i > newCount; i--)
            networkInfoBackend->removeInterface(QNetworkInfo::GsmMode, i - 1);
    } else if (newCount > actualCount) {
        for (int i = actualCount; i < newCount; i++) {
            QNetworkInfoData::CellularInfo info = {{QString(),
                                                   -1,
                                                   QNetworkInfo::GsmMode,
                                                   QNetworkInfo::UnknownStatus},
                                                   QString(),
                                                   QString(),
                                                   QString(),
                                                   QString(),
                                                   QString(),
                                                   QString(),
                                                   QString(),
                                                   QNetworkInfo::UnknownDataTechnology};
            networkInfoBackend->addCellularInterface(info);
        }
    }
#endif

    for (int i = 0; i < data.wLanInfo.count(); i++) {
        networkInfoBackend->setNetworkName(QNetworkInfo::WlanMode, i, data.wLanInfo[i].basicNetworkInfo.name);
        networkInfoBackend->setNetworkSignalStrength(QNetworkInfo::WlanMode, i, data.wLanInfo[i].basicNetworkInfo.signalStrength);
        networkInfoBackend->setNetworkStatus(QNetworkInfo::WlanMode, i, data.wLanInfo[i].basicNetworkInfo.status);
        networkInfoBackend->setNetworkMacAddress(QNetworkInfo::WlanMode, i, data.wLanInfo[i].macAddress);
    }

#if defined(QT_NO_OFONO)
    for (int i = 0; i < data.ethernetInfo.count(); i++) {
        networkInfoBackend->setNetworkName(QNetworkInfo::EthernetMode, i, data.ethernetInfo[i].basicNetworkInfo.name);
        networkInfoBackend->setNetworkSignalStrength(QNetworkInfo::EthernetMode, i, data.ethernetInfo[i].basicNetworkInfo.signalStrength);
        networkInfoBackend->setNetworkStatus(QNetworkInfo::EthernetMode, i, data.ethernetInfo[i].basicNetworkInfo.status);
        networkInfoBackend->setNetworkMacAddress(QNetworkInfo::EthernetMode, i, data.ethernetInfo[i].macAddress);
    }

    for (int i = 0; i < data.bluetoothInfo.count(); i++) {
        networkInfoBackend->setNetworkName(QNetworkInfo::BluetoothMode, i, data.bluetoothInfo[i].basicNetworkInfo.name);
        networkInfoBackend->setNetworkSignalStrength(QNetworkInfo::BluetoothMode, i, data.bluetoothInfo[i].basicNetworkInfo.signalStrength);
        networkInfoBackend->setNetworkStatus(QNetworkInfo::BluetoothMode, i, data.bluetoothInfo[i].basicNetworkInfo.status);
        networkInfoBackend->setNetworkMacAddress(QNetworkInfo::BluetoothMode, i, data.bluetoothInfo[i].btAddress);
    }

    for (int i = 0; i < data.cellularInfo.count(); i++) {
        networkInfoBackend->setNetworkName(data.cellularInfo[i].basicNetworkInfo.mode, i, data.cellularInfo[i].basicNetworkInfo.name);
        networkInfoBackend->setNetworkSignalStrength(data.cellularInfo[i].basicNetworkInfo.mode, i, data.cellularInfo[i].basicNetworkInfo.signalStrength);
        networkInfoBackend->setMode(i, data.cellularInfo[i].basicNetworkInfo.mode);
        networkInfoBackend->setNetworkStatus(data.cellularInfo[i].basicNetworkInfo.mode, i, data.cellularInfo[i].basicNetworkInfo.status);
        networkInfoBackend->setImsi(i, data.cellularInfo[i].imsi);
        networkInfoBackend->setCellId(i, data.cellularInfo[i].cellId);
        networkInfoBackend->setLocationAreaCode(i, data.cellularInfo[i].locationAreaCode);
        networkInfoBackend->setCurrentMobileCountryCode(i, data.cellularInfo[i].currentMobileCountryCode);
        networkInfoBackend->setCurrentMobileNetworkCode(i, data.cellularInfo[i].currentMobileNetworkCode);
        networkInfoBackend->setHomeMobileCountryCode(i, data.cellularInfo[i].homeMobileCountryCode);
        networkInfoBackend->setHomeMobileNetworkCode(i, data.cellularInfo[i].homeMobileNetworkCode);
        networkInfoBackend->setCellDataTechnology(i, data.cellularInfo[i].cellData);
    }
#endif
}

QT_END_NAMESPACE
