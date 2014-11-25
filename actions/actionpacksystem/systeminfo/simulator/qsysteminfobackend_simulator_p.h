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

#ifndef QSYSTEMINFOBACKEND_SIMULATOR_P_H
#define QSYSTEMINFOBACKEND_SIMULATOR_P_H


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

#include "qsysteminfodata_simulator_p.h"

QT_BEGIN_NAMESPACE

// QBatteryInfoSimulatorBackend

class Q_SYSTEMINFO_EXPORT QBatteryInfoSimulatorBackend : public QObject
{
    Q_OBJECT

private:
    QBatteryInfoSimulatorBackend(QObject *parent = 0);
    Q_DISABLE_COPY(QBatteryInfoSimulatorBackend)

public:
    ~QBatteryInfoSimulatorBackend();
    static QBatteryInfoSimulatorBackend *getSimulatorBackend();

    int getBatteryCount();
    int getBatteryIndex() const;
    int getLevel(int battery);
    int getCurrentFlow(int battery);
    int getCycleCount(int battery);
    int getMaximumCapacity(int battery);
    int getRemainingCapacity(int battery);
    int getRemainingChargingTime(int battery);
    int getVoltage(int battery);
    QBatteryInfo::ChargingState getChargingState(int battery);
    QBatteryInfo::ChargerType getChargerType();
    QBatteryInfo::LevelStatus getLevelStatus(int battery);
    QBatteryInfo::Health getHealth(int battery);
    float getTemperature(int battery);

    void setBatteryIndex(int batteryIndex);
    void setCurrentFlow(int flow);
    void setCycleCount(int cycleCount);
    void setMaximumCapacity(int capacity);
    void setRemainingCapacity(int capacity);
    void setRemainingChargingTime(int time);
    void setVoltage(int vol);
    void setChargingState(QBatteryInfo::ChargingState state);
    void setChargerType(QBatteryInfo::ChargerType type);
    void setLevelStatus(QBatteryInfo::LevelStatus levelStatus);
    void setHealth(QBatteryInfo::Health health);
    void setTemperature(float temperature);

Q_SIGNALS:
    void batteryCountChanged(int count);
    void batteryIndexChanged(int batteryIndex);
    void levelChanged(int level);
    void currentFlowChanged(int flow);
    void cycleCountChanged(int cycleCount);
    void remainingCapacityChanged(int capacity);
    void remainingChargingTimeChanged(int seconds);
    void voltageChanged(int voltage);
    void chargingStateChanged(QBatteryInfo::ChargingState state);
    void chargerTypeChanged(QBatteryInfo::ChargerType type);
    void levelStatusChanged(QBatteryInfo::LevelStatus levelStatus);
    void healthChanged(QBatteryInfo::Health health);
    void temperatureChanged(float temperature);

private:
    static QBatteryInfoSimulatorBackend *globalSimulatorBackend;
    QBatteryInfoData data;
};


// QDeviceInfoSimulatorBackend

class Q_SYSTEMINFO_EXPORT QDeviceInfoSimulatorBackend : public QObject
{
    Q_OBJECT

private:
    QDeviceInfoSimulatorBackend(QObject *parent = 0);
    Q_DISABLE_COPY(QDeviceInfoSimulatorBackend)

public:
    ~QDeviceInfoSimulatorBackend();
    static QDeviceInfoSimulatorBackend *getSimulatorBackend();

    bool hasFeature(QDeviceInfo::Feature);
    int getImeiCount();
    QDeviceInfo::LockTypeFlags getActivatedLocks();
    QDeviceInfo::LockTypeFlags getEnabledLocks();
    QDeviceInfo::ThermalState getThermalState();
    QString getImei(int num);
    QString getManufacturer();
    QString getModel();
    QString getProductName();
    QString getUniqueDeviceID();
    QString getVersion(QDeviceInfo::Version);

    void setFeature(QDeviceInfo::Feature feature, bool enable);
    void setActivatedLocks(QDeviceInfo::LockTypeFlags flag);
    void setEnabledLocks(QDeviceInfo::LockTypeFlags flag);
    void setThermalState(QDeviceInfo::ThermalState state);
    void setImei(int num, QString imei);
    void setManufacturer(QString manufacturer);
    void setModel(QString model);
    void setProductName(QString name);
    void setUniqueDeviceID(QString id);
    void setVersion(QDeviceInfo::Version version, QString versionString);

Q_SIGNALS:
    void activatedLocksChanged(QDeviceInfo::LockTypeFlags);
    void enabledLocksChanged(QDeviceInfo::LockTypeFlags);
    void thermalStateChanged(QDeviceInfo::ThermalState);

private:
    QDeviceInfoData data;
    static QDeviceInfoSimulatorBackend *globalSimulatorBackend;

    void initFeatureList();
    void initImeiList();
    void initVersionList();
};

// QNetworkInfoSimulatorBackend

class Q_SYSTEMINFO_EXPORT QNetworkInfoSimulatorBackend : public QObject
{
    Q_OBJECT

private:
    QNetworkInfoSimulatorBackend(QNetworkInfo *parent = 0);
    Q_DISABLE_COPY(QNetworkInfoSimulatorBackend)

public:
    ~QNetworkInfoSimulatorBackend();
    static QNetworkInfoSimulatorBackend *getSimulatorBackend();

    int getNetworkInterfaceCount(QNetworkInfo::NetworkMode mode);
    int getNetworkSignalStrength(QNetworkInfo::NetworkMode mode, int interface);
    QNetworkInfo::CellDataTechnology getCurrentCellDataTechnology(int interface);
    QNetworkInfo::NetworkMode getCurrentNetworkMode();
    QString getImsi(int interface);
    QNetworkInfo::NetworkStatus getNetworkStatus(QNetworkInfo::NetworkMode mode, int interface);
#ifndef QT_NO_NETWORKINTERFACE
    QNetworkInterface getInterfaceForMode(QNetworkInfo::NetworkMode mode, int interface);
#endif // QT_NO_NETWORKINTERFACE
    QString getCellId(int interface);
    QString getCurrentMobileCountryCode(int interface);
    QString getCurrentMobileNetworkCode(int interface);
    QString getHomeMobileCountryCode(int interface);
    QString getHomeMobileNetworkCode(int interface);
    QString getLocationAreaCode(int interface);
    QString getMacAddress(QNetworkInfo::NetworkMode mode, int interface);
    QString getNetworkName(QNetworkInfo::NetworkMode mode, int interface);
    QNetworkInfo::NetworkMode getMode(QNetworkInfo::NetworkMode mode, int interface);

    void setImsi(int interface, const QString &id);
    void setCellId(int interface, const QString &id);
    void setLocationAreaCode(int interface, const QString &code);
    void setCurrentMobileCountryCode( int interface, const QString &code);
    void setCurrentMobileNetworkCode(int interface, const QString &code);
    void setHomeMobileCountryCode(int interface, const QString &code);
    void setHomeMobileNetworkCode(int interface, const QString &code);
    void setCellDataTechnology(int interface, QNetworkInfo::CellDataTechnology cellData);
    void setMode(int interface, QNetworkInfo::NetworkMode mode);
    void setNetworkName(QNetworkInfo::NetworkMode mode, int interface, const QString &name);
    void setNetworkMacAddress(QNetworkInfo::NetworkMode mode, int interface, const QString &mac);
    void setNetworkSignalStrength(QNetworkInfo::NetworkMode mode, int interface, int strength);
    void setNetworkStatus(QNetworkInfo::NetworkMode mode, int interface, QNetworkInfo::NetworkStatus status);

    void addEthernetInterface(QNetworkInfoData::EthernetInfo info);
    void addWlanInterface(QNetworkInfoData::WLanInfo info);
    void addCellularInterface(QNetworkInfoData::CellularInfo info);
    void addBluetoothInterface(QNetworkInfoData::BluetoothInfo info);
    void removeInterface(QNetworkInfo::NetworkMode mode, int interface);
    void clearInterface(QNetworkInfo::NetworkMode mode);

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

private:
    static QNetworkInfoSimulatorBackend *globalSimulatorBackend;
    QNetworkInfoData data;

    QNetworkInfoData::BasicNetworkInfo *basicNetworkInfo(QNetworkInfo::NetworkMode mode, int interface);
    bool isValidInterface(QNetworkInfo::NetworkMode mode, int interface);
    void clearOrRemoveInterface(QNetworkInfo::NetworkMode mode, int interface, bool clear);
};

QT_END_NAMESPACE

#endif // QSYSTEMINFOBACKEND_SIMULATOR_P_H
