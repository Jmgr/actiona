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

#include "qbatteryinfo_mac_p.h"

#include <QtCore/qmetaobject.h>
#include <QtCore/qnumeric.h>
#include <QDebug>

#include <Foundation/NSAutoreleasePool.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/ps/IOPSKeys.h>
#include <Foundation/NSDictionary.h>
#include <Foundation/NSString.h>
#include <Foundation/NSKeyValueCoding.h>
#include <IOKit/IOKitLib.h>

QT_BEGIN_NAMESPACE

void powerInfoChanged(void* context)
{
    QBatteryInfoPrivate *sys = reinterpret_cast<QBatteryInfoPrivate *>(context);
    if (sys) {
        sys->currentChargingState();
    }
}

void batteryInfoChanged(void* context)
{
    QBatteryInfoPrivate *bat = reinterpret_cast<QBatteryInfoPrivate *>(context);
    if (bat) {
        bat->getBatteryInfo();
    }
}

QBatteryInfoPrivate::QBatteryInfoPrivate(QBatteryInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , currentBatLevel(0)
    , currentVoltage(-1)
    , dischargeRate(0)
    , capacity(-1)
    , timeToFull(-1)
    , remainingEnergy(-1)
    , numberOfBatteries(0)
    , index(0)
{
    initialize();
}

QBatteryInfoPrivate::QBatteryInfoPrivate(int batteryIndex, QBatteryInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
    , currentBatLevel(0)
    , currentVoltage(-1)
    , dischargeRate(0)
    , capacity(-1)
    , timeToFull(-1)
    , remainingEnergy(-1)
    , numberOfBatteries(0)
    , index(batteryIndex)
{
    initialize();
}

void QBatteryInfoPrivate::initialize()
{
    getBatteryInfo();
    NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];

    CFRunLoopSourceRef runLoopSource = (CFRunLoopSourceRef)IOPSNotificationCreateRunLoopSource(batteryInfoChanged, this);
    if (runLoopSource) {
        CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopDefaultMode);
        CFRelease(runLoopSource);
    }
    [autoreleasepool release];
    currentChargingState();
    getBatteryInfo();
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
    return (index >= 0) && (numberOfBatteries > index);
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

        Q_EMIT batteryIndexChanged(index);
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
    if (battery < 0)
        battery = 0;
    return currentFlows.value(battery);
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
    if (battery < 0)
        battery = 0;
    return maximumCapacities.value(battery);
}

int QBatteryInfoPrivate::maximumCapacity()
{
    return maximumCapacity(index);
}

int QBatteryInfoPrivate::remainingCapacity(int battery)
{
    if (battery < 0)
        battery = 0;
    return remainingCapacities.value(battery);
}

int QBatteryInfoPrivate::remainingCapacity()
{
    return remainingCapacity(index);
}

int QBatteryInfoPrivate::remainingChargingTime(int battery)
{
    if (battery < 0)
        battery = 0;
    return remainingChargingTimes.value(battery);
}

int QBatteryInfoPrivate::remainingChargingTime()
{
    return remainingChargingTime(index);
}

int QBatteryInfoPrivate::voltage(int battery)
{
    if (battery < 0)
        battery = 0;
    return voltages.value(battery);
}

int QBatteryInfoPrivate::voltage()
{
    return voltage(index);
}

QBatteryInfo::ChargerType QBatteryInfoPrivate::chargerType()
{
    return curChargeType;
}

QBatteryInfo::ChargingState QBatteryInfoPrivate::chargingState(int battery)
{
    if (battery < 0)
        battery = 0;
    return chargingStates.value(battery);
}

QBatteryInfo::ChargingState QBatteryInfoPrivate::chargingState()
{
    return chargingState(index);
}

QBatteryInfo::LevelStatus QBatteryInfoPrivate::levelStatus(int battery)
{
    if (battery < 0)
        battery = 0;
    return levelStatuss.value(battery);
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

void QBatteryInfoPrivate::connectNotify(const QMetaMethod &signal)
{
   // static const QMetaMethod batteryCountChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoPrivate::batteryCountChanged);
    static const QMetaMethod chargingStateChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoPrivate::chargingStateChanged);
    static const QMetaMethod currentFlowChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoPrivate::currentFlowChanged);
    static const QMetaMethod levelStatusChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoPrivate::levelStatusChanged);
//    static const QMetaMethod chargerTypeChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoPrivate::chargerTypeChanged);
//    static const QMetaMethod remainingCapacityChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoPrivate::remainingCapacityChanged);
//    static const QMetaMethod remainingChargingTimeChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoPrivate::remainingChargingTimeChanged);
//    static const QMetaMethod voltageChangedSignal = QMetaMethod::fromSignal(&QBatteryInfoPrivate::voltageChanged);

    if (signal == chargingStateChangedSignal
               || signal == currentFlowChangedSignal
               || signal == levelStatusChangedSignal) {

           NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];

           CFRunLoopSourceRef runLoopSource = (CFRunLoopSourceRef)IOPSNotificationCreateRunLoopSource(powerInfoChanged, this);
           if (runLoopSource) {
               CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopDefaultMode);
           }
           [autoreleasepool release];
           CFRelease(runLoopSource);
       }
}

void QBatteryInfoPrivate::disconnectNotify(const QMetaMethod &signal)
{
    Q_UNUSED(signal)
}

QBatteryInfo::ChargingState QBatteryInfoPrivate::currentChargingState()
{
    NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];
    QBatteryInfo::ChargingState state = QBatteryInfo::UnknownChargingState;
    QBatteryInfo::ChargerType cType = QBatteryInfo::UnknownCharger;

    CFDictionaryRef powerSourceDict = NULL;
    CFStringRef powerStateString;

    CFTypeRef powerSourcesInfo = IOPSCopyPowerSourcesInfo();
    CFArrayRef powerSourcesList = IOPSCopyPowerSourcesList(powerSourcesInfo);
    int batteries = CFArrayGetCount(powerSourcesList);

    if (batteries != numberOfBatteries) {
        bool validBefore = isValid();
        numberOfBatteries = batteries;
        bool validNow = isValid();
        if (validBefore != validNow)
            Q_EMIT validChanged(validNow);

        Q_EMIT batteryCountChanged(numberOfBatteries);
    }

    int i;
    for (i = 0; i < numberOfBatteries; i++) {
        powerSourceDict = IOPSGetPowerSourceDescription(powerSourcesInfo, CFArrayGetValueAtIndex(powerSourcesList, i));
        if (!powerSourceDict) {
            state = QBatteryInfo::UnknownChargingState;
            cType = QBatteryInfo::UnknownCharger;
            qDebug() << "unknown state";
            break;
        }

        powerStateString = (CFStringRef)CFDictionaryGetValue(powerSourceDict, CFSTR(kIOPSPowerSourceStateKey));
        if (CFStringCompare(powerStateString,CFSTR(kIOPSBatteryPowerValue),0)==kCFCompareEqualTo) {
            //has battery
            state = QBatteryInfo::Discharging;
            cType = QBatteryInfo::UnknownCharger;
        } else {

            NSDictionary *powerSourceInfo = nil;
            powerSourceInfo = [NSDictionary dictionaryWithDictionary:(NSDictionary*)powerSourceDict];
            bool charging = (bool)[[powerSourceInfo valueForKey:[NSString stringWithUTF8String:kIOPSIsChargingKey]] boolValue];
            if (charging ) {
                state = QBatteryInfo::Charging;
                cType = QBatteryInfo::WallCharger;
            } else {
                cType = QBatteryInfo::UnknownCharger;
                state = QBatteryInfo::IdleChargingState;
            }
        }

        if (cType != curChargeType) {
            curChargeType = cType;
            Q_EMIT chargerTypeChanged(curChargeType);
        }

        if ( chargingStates.value(i) != state) {
            chargingStates[i] = state;
            if (i == index)
                Q_EMIT chargingStateChanged(state);
        }
    }
    CFRelease(powerSourcesInfo);
    CFRelease(powerSourcesList);

    [autoreleasepool release];
    return state;
}

void QBatteryInfoPrivate::getBatteryInfo()
{
    int cEnergy = 0;
    int cVoltage = 0;
    int cTime = 0;
    int rEnergy = 0;

    CFTypeRef info;
    CFArrayRef list;
    CFDictionaryRef battery;

    info = IOPSCopyPowerSourcesInfo();
    if (info == NULL) {
        qDebug() << "IOPSCopyPowerSourcesInfo error";
        return;
    }
    list = IOPSCopyPowerSourcesList(info);
    if (list == NULL) {
        CFRelease(info);
        qDebug() << "IOPSCopyPowerSourcesList error";
        return;
    }

    CFMutableDictionaryRef matching = NULL;
    CFMutableDictionaryRef batDoctionary = NULL;
    io_registry_entry_t entry = 0;
    matching = IOServiceMatching("IOPMPowerSource");
    entry = IOServiceGetMatchingService(kIOMasterPortDefault,matching);
    IORegistryEntryCreateCFProperties(entry, &batDoctionary,NULL,0);

    int batteries = CFArrayGetCount(list);
    for (int i = 0; i < batteries; i++) {

        battery = IOPSGetPowerSourceDescription(info, CFArrayGetValueAtIndex(list, i));

        int curCapacityPercent = 0;
        int maxCapacity = 0;

        const void *psValue;
        psValue = CFDictionaryGetValue(battery, CFSTR(kIOPSCurrentCapacityKey));
        CFNumberGetValue((CFNumberRef)psValue, kCFNumberSInt32Type, &curCapacityPercent);

        psValue = CFDictionaryGetValue(battery, CFSTR(kIOPSMaxCapacityKey));
        CFNumberGetValue((CFNumberRef)psValue, kCFNumberSInt32Type, &maxCapacity);

        QBatteryInfo::LevelStatus stat = QBatteryInfo::LevelUnknown;

        if (curCapacityPercent < 2) {
            stat = QBatteryInfo::LevelEmpty;
        } else if (curCapacityPercent < 11) {
             stat =  QBatteryInfo::LevelLow;
        } else if (curCapacityPercent > 10 && curCapacityPercent < 100) {
             stat = QBatteryInfo::LevelOk;
        } else if (curCapacityPercent == 100) {
             stat = QBatteryInfo::LevelFull;
        }
        if (levelStatuss.value(i) != stat) {
            levelStatuss[i] = stat;
            if (i == index)
                Q_EMIT levelStatusChanged(stat);
        }

        cVoltage = [[(NSDictionary*)batDoctionary objectForKey:@kIOPSVoltageKey] intValue];

        if (cVoltage != voltages.value(i) && cVoltage != 0) {
            voltages[i] = cVoltage;
            if (i == index)
                Q_EMIT(voltageChanged( cVoltage));
        }

        cEnergy = [[(NSDictionary*)batDoctionary objectForKey:@kIOPSCurrentKey] doubleValue];

        if (cEnergy != currentFlows.value(i) && cEnergy != 0) {
            currentFlows[i] = cEnergy;
            if (i == index)
                Q_EMIT currentFlowChanged(cEnergy);
        }

        cTime = [[(NSDictionary*)batDoctionary objectForKey:@kIOPSTimeToFullChargeKey] intValue];

        if (cTime != remainingChargingTimes.value(i)) {
            remainingChargingTimes[i] = cTime * 60;
            if (i == index)
                Q_EMIT remainingChargingTimeChanged(remainingChargingTimes[i]);
        }

        rEnergy = [[(NSDictionary*)batDoctionary objectForKey:@"CurrentCapacity"] intValue];

        if (rEnergy != remainingCapacities.value(i) && rEnergy != 0) {
            remainingCapacities[i] = rEnergy;
            if (i == index)
                Q_EMIT remainingCapacityChanged(remainingCapacities[i]);
        }
        int max = rEnergy / ((qreal)(curCapacityPercent) / 100);
        maximumCapacities[i] = max;
    }

    if (batteries == 0) {
        maximumCapacities[0] = -1;
        remainingCapacities[0] = -1;
        remainingChargingTimes[0] = -1;
        voltages[0] = -1;
        chargingStates[0] = QBatteryInfo::UnknownChargingState;
        levelStatuss[0] = QBatteryInfo::LevelUnknown;
    }
    if (batDoctionary != NULL)
        CFRelease(batDoctionary);
    CFRelease(info);
    CFRelease(list);
}

QT_END_NAMESPACE
