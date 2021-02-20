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

#include "qbatteryinfo_win_p.h"

#include <qt_windows.h>

#include <powrprof.h>
#include <setupapi.h>

#if !defined (Q_CC_MINGW) || defined(__MINGW64_VERSION_MAJOR)
#  include <batclass.h>
#  include <Winioctl.h>
#endif

#include <QtCore/qmetaobject.h>
#include <QtCore/QTimer>
#include <QtCore/QUuid>
#include <QtCore/qnumeric.h>

#ifdef Q_CC_MSVC
#  pragma comment (lib, "Setupapi.lib")
#endif

QT_BEGIN_NAMESPACE

QBatteryInfoPrivate::QBatteryInfoPrivate(QBatteryInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , timeToFull(0)
    , numberOfBatteries(0)
    , index(0)
{
    initialize();
}

QBatteryInfoPrivate::QBatteryInfoPrivate(int batteryIndex, QBatteryInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , timeToFull(0)
    , numberOfBatteries(0)
    , index(batteryIndex)
{
    initialize();
}

void QBatteryInfoPrivate::initialize()
{
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(getBatteryStatus()));
    timer->start(1000);
    getBatteryStatus();
}

QBatteryInfoPrivate::~QBatteryInfoPrivate()
{
}

int QBatteryInfoPrivate::batteryCount()
{
    return numberOfBatteries;
}

int QBatteryInfoPrivate::batteryIndex() const
{
    return index;
}

bool QBatteryInfoPrivate::isValid()
{
    // valid if the index < total count.
    return (index >= 0) && (index < batteryCount());
}

void QBatteryInfoPrivate::setBatteryIndex(int batteryIndex)
{
    if (index != batteryIndex) {
        bool validBefore = isValid();
        int oldIndex = index;
        index = batteryIndex;
        bool validNow = isValid();
        if (validBefore != validNow)
            Q_EMIT validChanged(validNow);

        if (validNow) {
            if (validBefore) {
                // valid now, valid before so we have to check individual values

                // ignore chargerType - it won't change based on battery index
                //emit chargerTypeChanged(newChargerType);

                QBatteryInfo::ChargingState newChargingState = chargingState();
                if (newChargingState != chargingState(oldIndex))
                    emit chargingStateChanged(newChargingState);

                int newValue = level();
                if (newValue != level(oldIndex))
                    emit levelChanged(newValue);

                newValue = currentFlow();
                if (newValue != currentFlow(oldIndex))
                    emit currentFlowChanged(newValue);

                newValue = cycleCount();
                if (newValue != cycleCount(oldIndex))
                    emit cycleCountChanged(newValue);

                newValue = remainingCapacity();
                if (newValue != remainingCapacity(oldIndex))
                    emit remainingCapacityChanged(newValue);

                newValue = remainingChargingTime();
                if (newValue != remainingChargingTime(oldIndex))
                    emit remainingChargingTimeChanged(newValue);

                newValue = voltage();
                if (newValue != voltage(oldIndex))
                    emit voltageChanged(newValue);

                QBatteryInfo::LevelStatus newLevelStatus = levelStatus();
                if (newLevelStatus != levelStatus(oldIndex))
                    emit levelStatusChanged(newLevelStatus);

                QBatteryInfo::Health newHealth = health();
                if (newHealth != health(oldIndex))
                    emit healthChanged(newHealth);

                float newTemperature = temperature();
                if (!qFuzzyCompare(newTemperature, temperature(oldIndex)))
                    emit temperatureChanged(newTemperature);
            } else {
                // it wasn't valid before so everything is changed

                // ignore chargerType - it won't change based on battery index
                //emit chargerTypeChanged(newChargerType);

                emit chargingStateChanged(chargingState());
                emit levelChanged(level());
                emit currentFlowChanged(currentFlow());
                emit cycleCountChanged(cycleCount());
                emit remainingCapacityChanged(remainingCapacity());
                emit remainingChargingTimeChanged(remainingChargingTime());
                emit voltageChanged(voltage());
                emit levelStatusChanged(levelStatus());
                emit healthChanged(health());
                emit temperatureChanged(temperature());
            }
        }

        emit batteryIndexChanged(index);
    }
}

int QBatteryInfoPrivate::level(int battery)
{
    int maxCapacity = maximumCapacity(battery);
    int remCapacity = remainingCapacity(battery);

    if (maxCapacity == 0)
        return -1;

    return remCapacity * 100 / maxCapacity;
}

int QBatteryInfoPrivate::level()
{
    return level(index);
}

int QBatteryInfoPrivate::currentFlow(int battery)
{
    return currentFlows[battery];
}

int QBatteryInfoPrivate::currentFlow()
{
    return currentFlow(index);
}

int QBatteryInfoPrivate::cycleCount(int battery)
{
    Q_UNUSED(battery)

    return -1;
}

int QBatteryInfoPrivate::cycleCount()
{
    return cycleCount(index);
}

int QBatteryInfoPrivate::maximumCapacity(int battery)
{
    return maximumCapacities[battery];
}

int QBatteryInfoPrivate::maximumCapacity()
{
    return maximumCapacity(index);
}

int QBatteryInfoPrivate::remainingCapacity(int battery)
{
    return remainingCapacities[battery];
}

int QBatteryInfoPrivate::remainingCapacity()
{
    return remainingCapacity(index);
}

int QBatteryInfoPrivate::remainingChargingTime(int battery)
{
    Q_UNUSED(battery)
    SYSTEM_BATTERY_STATE systemBatteryState;
    CallNtPowerInformation(SystemBatteryState,NULL,0,&systemBatteryState,sizeof(systemBatteryState));

   int cTime = systemBatteryState.EstimatedTime;
    if (cTime != timeToFull) {
        timeToFull = cTime;
        emit remainingChargingTimeChanged(timeToFull);
    }

    return timeToFull;
}

int QBatteryInfoPrivate::remainingChargingTime()
{
    return remainingChargingTime(index);
}

int QBatteryInfoPrivate::voltage(int battery)
{
    return voltages[battery];
}

int QBatteryInfoPrivate::voltage()
{
    return voltage(index);
}

QBatteryInfo::ChargerType QBatteryInfoPrivate::chargerType()
{
    return currentChargerType;
}

QBatteryInfo::ChargingState QBatteryInfoPrivate::chargingState(int battery)
{
    return chargingStates[battery];
}

QBatteryInfo::ChargingState QBatteryInfoPrivate::chargingState()
{
    return chargingState(index);
}

QBatteryInfo::LevelStatus QBatteryInfoPrivate::levelStatus(int battery)
{
    return levelStatuss[battery];
}

QBatteryInfo::LevelStatus QBatteryInfoPrivate::levelStatus()
{
    return levelStatus(index);
}

QBatteryInfo::Health QBatteryInfoPrivate::health(int battery)
{
    Q_UNUSED(battery)

    return QBatteryInfo::HealthUnknown;
}

QBatteryInfo::Health QBatteryInfoPrivate::health()
{
    return health(index);
}

float QBatteryInfoPrivate::temperature(int battery)
{
    Q_UNUSED(battery)

    return qQNaN();
}

float QBatteryInfoPrivate::temperature()
{
    return temperature(index);
}

void QBatteryInfoPrivate::getBatteryStatus()
{
#if !defined (Q_CC_MINGW) || defined(__MINGW64_VERSION_MAJOR)
    SYSTEM_BATTERY_STATE systemBatteryState;
    CallNtPowerInformation(SystemBatteryState,NULL,0,&systemBatteryState,sizeof(systemBatteryState));

    int cTime = systemBatteryState.EstimatedTime;
    if (cTime != timeToFull) {
        timeToFull = cTime;
        emit remainingChargingTimeChanged(timeToFull);
    }

    int batteryNumber = 0;

    QUuid guidDeviceBattery(0x72631e54L,0x78A4,0x11d0,0xbc,0xf7,0x00,0xaa,0x00,0xb7,0xb3,0x2a);
    GUID GUID_DEVICE_BATTERY = static_cast<GUID>(guidDeviceBattery);

    HDEVINFO hdevInfo = SetupDiGetClassDevs(&GUID_DEVICE_BATTERY,0,0,DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (INVALID_HANDLE_VALUE != hdevInfo) {
        for (int i = 0; i < 100; i++) {
            SP_DEVICE_INTERFACE_DATA deviceInterfaceData = {0};
            deviceInterfaceData.cbSize = sizeof(deviceInterfaceData);

            if (SetupDiEnumDeviceInterfaces(hdevInfo,0,&GUID_DEVICE_BATTERY,i,&deviceInterfaceData)){
                DWORD cbRequired = 0;

                SetupDiGetDeviceInterfaceDetail(hdevInfo, &deviceInterfaceData,0, 0, &cbRequired, 0);
                if (ERROR_INSUFFICIENT_BUFFER == GetLastError()) {
                    PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetail =
                            (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR, cbRequired);
                    if (deviceInterfaceDetail){
                        deviceInterfaceDetail->cbSize = sizeof(*deviceInterfaceDetail);
                        if (SetupDiGetDeviceInterfaceDetail(hdevInfo, &deviceInterfaceData, deviceInterfaceDetail, cbRequired, &cbRequired, 0)) {

                            batteryNumber++; //new battery
                            HANDLE batteryHandle = CreateFile(deviceInterfaceDetail->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                                         NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                            if (INVALID_HANDLE_VALUE != batteryHandle){

                                BATTERY_QUERY_INFORMATION batteryQueryInfo = {0};

                                DWORD inBuf = 0;
                                DWORD dwOut;

                                if (DeviceIoControl(batteryHandle,IOCTL_BATTERY_QUERY_TAG, &inBuf, sizeof(inBuf), &batteryQueryInfo.BatteryTag, sizeof(batteryQueryInfo.BatteryTag), &dwOut, NULL)
                                        && batteryQueryInfo.BatteryTag) {

                                    BATTERY_INFORMATION batteryInfo = {0};
                                    batteryQueryInfo.InformationLevel = BatteryInformation;

                                    if (DeviceIoControl(batteryHandle, IOCTL_BATTERY_QUERY_INFORMATION,
                                                        &batteryQueryInfo, sizeof(batteryQueryInfo),
                                                        &batteryInfo, sizeof(batteryInfo), &dwOut, NULL)) {

                                        maximumCapacities.insert(batteryNumber, -1);

                                        if (batteryInfo.Capabilities & BATTERY_SYSTEM_BATTERY) {
                                            if (!(batteryInfo.Capabilities & BATTERY_IS_SHORT_TERM)) {
 //                                                dwResult |= HASBATTERY;

                                                BATTERY_WAIT_STATUS batteryWaitStatus = {0};
                                                batteryWaitStatus.BatteryTag = batteryQueryInfo.BatteryTag;

                                                BATTERY_STATUS batteryStatus;
                                                if (DeviceIoControl(batteryHandle, IOCTL_BATTERY_QUERY_STATUS,
                                                                    &batteryWaitStatus, sizeof(batteryWaitStatus),
                                                                    &batteryStatus, sizeof(batteryStatus), &dwOut, NULL)) {

                                                    QBatteryInfo::ChargerType chargerType = QBatteryInfo::UnknownCharger;

                                                    if (batteryStatus.PowerState & BATTERY_POWER_ON_LINE) {
                                                        chargerType = QBatteryInfo::WallCharger;
                                                    }
                                                    if (currentChargerType != chargerType) {
                                                        currentChargerType = chargerType;
                                                        Q_EMIT chargerTypeChanged(chargerType);
                                                    }

                                                    QBatteryInfo::ChargingState chargingState{QBatteryInfo::UnknownChargingState};
                                                    if (batteryStatus.PowerState & BATTERY_CHARGING)
                                                        chargingState = QBatteryInfo::Charging;
                                                    if (batteryStatus.PowerState & BATTERY_DISCHARGING)
                                                         chargingState = QBatteryInfo::Discharging;

                                                    if (chargingStates[batteryNumber] != chargingState) {
                                                        chargingStates.insert(batteryNumber, chargingState);
                                                        if (batteryNumber == index)
                                                            emit chargingStateChanged(chargingState);
                                                    }

                                                    if (voltages[batteryNumber] !=  batteryStatus.Voltage) {
                                                        voltages.insert(batteryNumber, batteryStatus.Voltage);
                                                        if (batteryNumber == index)
                                                            Q_EMIT voltageChanged(batteryStatus.Voltage);
                                                    }
                                                    if (currentFlows[batteryNumber] != batteryStatus.Rate) {
                                                        currentFlows.insert(batteryNumber,batteryStatus.Rate);
                                                        if (batteryNumber == index)
                                                            Q_EMIT currentFlowChanged(batteryStatus.Rate);
                                                    }
                                                    if (batteryStatus.Voltage == BATTERY_UNKNOWN_VOLTAGE) {
                                                        // If we don't have the voltage then we can't convert from mWh to mAh
                                                        batteryStatus.Capacity = -1;
                                                        maximumCapacities.insert(batteryNumber, -1);
                                                    } else {
                                                        // Convert from mWh to mAh
                                                        batteryStatus.Capacity = batteryStatus.Capacity / batteryStatus.Voltage;
                                                        maximumCapacities.insert(batteryNumber, batteryInfo.FullChargedCapacity / batteryStatus.Voltage);
                                                    }
                                                    if (remainingCapacities[batteryNumber] != batteryStatus.Capacity) {
                                                        remainingCapacities.insert(batteryNumber, batteryStatus.Capacity);
                                                        if (batteryNumber == index)
                                                            Q_EMIT remainingCapacityChanged(batteryStatus.Capacity);
                                                    }
                                                    ///
                                                    int level = batteryInfo.FullChargedCapacity / batteryStatus.Capacity;
                                                    QBatteryInfo::LevelStatus batStatus = QBatteryInfo::LevelUnknown;

                                                    if (batteryStatus.PowerState & BATTERY_CRITICAL) {
                                                        batStatus =QBatteryInfo::LevelEmpty ;
                                                    } else if (level < 67) {
                                                        batStatus = QBatteryInfo::LevelLow;
                                                    } else if (level > 66) {
                                                       batStatus = QBatteryInfo::LevelOk;
                                                    } else if (level == 100) {
                                                        batStatus = QBatteryInfo::LevelFull;
                                                    }

                                                    if (levelStatuss[batteryNumber] != batStatus) {
                                                        levelStatuss.insert(batteryNumber,batStatus);
                                                        if (batteryNumber == index)
                                                            Q_EMIT levelStatusChanged(batStatus);
                                                    }
                                                }
                                            }
                                        }
                                    } //end IOCTL_BATTERY_QUERY_INFORMATION
                                } // end BATTERY_INFORMATION

                                CloseHandle(batteryHandle);
                            }
                        }
                        LocalFree(deviceInterfaceDetail);
                    }
                }
            }
            else  if (ERROR_NO_MORE_ITEMS == GetLastError()) {
                break;
            }
        }
        SetupDiDestroyDeviceInfoList(hdevInfo);
    }
    bool validBefore = isValid();
    numberOfBatteries = batteryNumber;
    bool validNow = isValid();
    if (validBefore != validNow)
        Q_EMIT validChanged(validNow);

#else // !defined (Q_CC_MINGW) || defined(__MINGW64_VERSION_MAJOR)
    numberOfBatteries = 0;
    Q_UNIMPLEMENTED();
#endif
}

QT_END_NAMESPACE
