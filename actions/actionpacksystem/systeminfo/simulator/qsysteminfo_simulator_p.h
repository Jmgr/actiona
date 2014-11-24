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

#ifndef QSYSTEMINFO_SIMULATOR_P_H
#define QSYSTEMINFO_SIMULATOR_P_H


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

#include <qbatteryinfo.h>
#include <qdeviceinfo.h>
#include <qnetworkinfo.h>

QT_BEGIN_NAMESPACE

class QBatteryInfoSimulatorBackend;
class QDeviceInfoSimulatorBackend;
class QNetworkInfoSimulatorBackend;

#if !defined(QT_NO_OFONO)
class QNetworkInfoPrivate;
#endif

class QBatteryInfoSimulator : public QObject
{
    Q_OBJECT

public:
    QBatteryInfoSimulator(QBatteryInfo *parent);
    QBatteryInfoSimulator(int batteryIndex, QBatteryInfo *parent);
    ~QBatteryInfoSimulator();

    int batteryCount();
    int batteryIndex() const;
    bool isValid();
    int level();
    int currentFlow(int battery);
    int currentFlow();
    int cycleCount();
    int maximumCapacity(int battery);
    int maximumCapacity();
    int remainingCapacity(int battery);
    int remainingCapacity();
    int remainingChargingTime(int battery);
    int remainingChargingTime();
    int voltage(int battery);
    int voltage();
    QBatteryInfo::ChargerType chargerType();
    QBatteryInfo::ChargingState chargingState(int battery);
    QBatteryInfo::ChargingState chargingState();
    QBatteryInfo::LevelStatus levelStatus(int battery);
    QBatteryInfo::LevelStatus levelStatus();
    QBatteryInfo::Health health();
    float temperature();

    void setBatteryIndex(int batteryIndex);

Q_SIGNALS:
    void batteryCountChanged(int count);
    void batteryIndexChanged(int batteryIndex);
    void validChanged(bool isValid);
    void chargerTypeChanged(QBatteryInfo::ChargerType type);
    void chargingStateChanged(QBatteryInfo::ChargingState state);
    void levelChanged(int level);
    void currentFlowChanged(int flow);
    void cycleCountChanged(int cycleCount);
    void remainingCapacityChanged(int capacity);
    void remainingChargingTimeChanged(int seconds);
    void voltageChanged(int voltage);
    void levelStatusChanged(QBatteryInfo::LevelStatus levelStatus);

protected:
    void connectNotify(const QMetaMethod &signal);
    void disconnectNotify(const QMetaMethod &signal);

private:
    QBatteryInfo * const q_ptr;
    Q_DECLARE_PUBLIC(QBatteryInfo)

    QBatteryInfoSimulatorBackend *batteryInfoSimulatorBackend;
};

class QDeviceInfoSimulator : public QObject
{
    Q_OBJECT

public:
    QDeviceInfoSimulator(QDeviceInfo *parent);
    ~QDeviceInfoSimulator();

    bool hasFeature(QDeviceInfo::Feature feature);
    int imeiCount();
    QDeviceInfo::LockTypeFlags activatedLocks();
    QDeviceInfo::LockTypeFlags enabledLocks();
    QDeviceInfo::ThermalState thermalState();
    QString imei(int interface);
    QString manufacturer();
    QString model();
    QString productName();
    QString uniqueDeviceID();
    QString version(QDeviceInfo::Version type);
    QString operatingSystemName();
    QString boardName();
    bool currentBluetoothPowerState();

Q_SIGNALS:
    void activatedLocksChanged(QDeviceInfo::LockTypeFlags types);
    void enabledLocksChanged(QDeviceInfo::LockTypeFlags types);
    void thermalStateChanged(QDeviceInfo::ThermalState state);

protected:
    void connectNotify(const QMetaMethod &signal);
    void disconnectNotify(const QMetaMethod &signal);

private:
    QDeviceInfo * const q_ptr;
    Q_DECLARE_PUBLIC(QDeviceInfo)

    QDeviceInfoSimulatorBackend *deviceInfoSimulatorBackend;
};

class QNetworkInfoSimulator : public QObject
{
    Q_OBJECT

public:
    QNetworkInfoSimulator(QNetworkInfo *parent);
    ~QNetworkInfoSimulator();

    int networkInterfaceCount(QNetworkInfo::NetworkMode mode);
    int networkSignalStrength(QNetworkInfo::NetworkMode mode, int interface);
    QNetworkInfo::CellDataTechnology currentCellDataTechnology(int interface);
    QNetworkInfo::NetworkMode currentNetworkMode();
    QNetworkInfo::NetworkStatus networkStatus(QNetworkInfo::NetworkMode mode, int interface);
#ifndef QT_NO_NETWORKINTERFACE
    QNetworkInterface interfaceForMode(QNetworkInfo::NetworkMode mode, int interface);
#endif // QT_NO_NETWORKINTERFACE
    QString cellId(int interface);
    QString currentMobileCountryCode(int interface);
    QString currentMobileNetworkCode(int interface);
    QString homeMobileCountryCode(int interface);
    QString homeMobileNetworkCode(int interface);
    QString imsi(int interface);
    QString locationAreaCode(int interface);
    QString macAddress(QNetworkInfo::NetworkMode mode, int interface);
    QString networkName(QNetworkInfo::NetworkMode mode, int interface);

Q_SIGNALS:
    void cellIdChanged(int interface, const QString &id);
    void currentCellDataTechnologyChanged(int interface, QNetworkInfo::CellDataTechnology tech);
    void currentMobileCountryCodeChanged(int interface, const QString &mcc);
    void currentMobileNetworkCodeChanged(int interface, const QString &mnc);
    void currentNetworkModeChanged(QNetworkInfo::NetworkMode mode);
    void locationAreaCodeChanged(int interface, const QString &lac);
    void networkInterfaceCountChanged(QNetworkInfo::NetworkMode mode, int count);
    void networkNameChanged(QNetworkInfo::NetworkMode mode, int interface, const QString &name);
    void networkSignalStrengthChanged(QNetworkInfo::NetworkMode mode, int interface, int strength);
    void networkStatusChanged(QNetworkInfo::NetworkMode mode, int interface, QNetworkInfo::NetworkStatus status);

protected:
    void connectNotify(const QMetaMethod &signal);
    void disconnectNotify(const QMetaMethod &signal);

private:
    QNetworkInfo * const q_ptr;
    Q_DECLARE_PUBLIC(QNetworkInfo)

    QNetworkInfoSimulatorBackend *networkInfoSimulatorBackend;

#if !defined(QT_NO_OFONO)
    QNetworkInfoPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QNetworkInfo)

public Q_SLOTS:
    void onCurrentNetworkModeChanged(QNetworkInfo::NetworkMode mode);
    void onNetworkInterfaceCountChanged(QNetworkInfo::NetworkMode mode, int count);
    void onNetworkNameChanged(QNetworkInfo::NetworkMode mode, int interface, const QString &name);
    void onNetworkSignalStrengthChanged(QNetworkInfo::NetworkMode mode, int interface, int strength);
    void onNetworkStatusChanged(QNetworkInfo::NetworkMode mode, int interface, QNetworkInfo::NetworkStatus status);
#endif
};

QT_END_NAMESPACE

#endif /*QSYSTEMINFO_SIMULATOR_P_H*/
