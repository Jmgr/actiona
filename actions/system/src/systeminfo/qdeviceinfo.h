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

#ifndef QDEVICEINFO_H
#define QDEVICEINFO_H

#include "qsysteminfoglobal.h"
#include <QtCore/qobject.h>

QT_BEGIN_NAMESPACE

#if !defined(QT_SIMULATOR)
class QDeviceInfoPrivate;
#else
class QDeviceInfoSimulator;
#endif // QT_SIMULATOR

class Q_SYSTEMINFO_EXPORT QDeviceInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(LockTypeFlags activatedLocks READ activatedLocks NOTIFY activatedLocksChanged)
    Q_PROPERTY(LockTypeFlags enabledLocks READ enabledLocks NOTIFY enabledLocksChanged)
    Q_PROPERTY(ThermalState thermalState READ thermalState NOTIFY thermalStateChanged)
    Q_PROPERTY(bool currentBluetoothPowerState READ currentBluetoothPowerState NOTIFY bluetoothStateChanged)

public:
    enum Feature {
        BluetoothFeature = 0,
        CameraFeature,
        FmRadioFeature,
        FmTransmitterFeature,
        InfraredFeature,
        LedFeature,
        MemoryCardFeature,
        UsbFeature,
        VibrationFeature,
        WlanFeature,
        SimFeature,
        PositioningFeature,
        VideoOutFeature,
        HapticsFeature,
        NfcFeature
    };
    Q_ENUM(Feature)

    enum LockType {
        NoLock = 0,
        PinLock = 0x0000001,
        TouchOrKeyboardLock = 0x0000002,
        UnknownLock = 0x0000004
    };
    Q_DECLARE_FLAGS(LockTypeFlags, LockType)
    Q_FLAG(LockTypeFlags)

    enum ThermalState {
        UnknownThermal = 0,
        NormalThermal,
        WarningThermal,
        AlertThermal,
        ErrorThermal
    };
    Q_ENUM(ThermalState)

    enum Version {
        Os = 0,
        Firmware
    };
    Q_ENUM(Version)

    QDeviceInfo(QObject *parent = nullptr);
    ~QDeviceInfo() override;

    QDeviceInfo::LockTypeFlags activatedLocks() const;
    QDeviceInfo::LockTypeFlags enabledLocks() const;
    QDeviceInfo::ThermalState thermalState() const;

    Q_INVOKABLE bool hasFeature(QDeviceInfo::Feature feature) const;
    Q_INVOKABLE int imeiCount() const;
    Q_INVOKABLE QString imei(int interfaceNumber) const;
    Q_INVOKABLE QString manufacturer() const;
    Q_INVOKABLE QString model() const;
    Q_INVOKABLE QString productName() const;
    Q_INVOKABLE QString uniqueDeviceID() const;
    Q_INVOKABLE QString version(QDeviceInfo::Version type) const;
    Q_INVOKABLE QString operatingSystemName() const;
    Q_INVOKABLE QString boardName() const;

    bool currentBluetoothPowerState();

Q_SIGNALS:
    void activatedLocksChanged(QDeviceInfo::LockTypeFlags types);
    void enabledLocksChanged(QDeviceInfo::LockTypeFlags types);
    void thermalStateChanged(QDeviceInfo::ThermalState state);
    void bluetoothStateChanged(bool on);

protected:
    void connectNotify(const QMetaMethod &signal) override;
    void disconnectNotify(const QMetaMethod &signal) override;

private:
    Q_DISABLE_COPY(QDeviceInfo)
#if !defined(QT_SIMULATOR)
    QDeviceInfoPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QDeviceInfo)
#else
    QDeviceInfoSimulator * const d_ptr;
#endif // QT_SIMULATOR
};

QT_END_NAMESPACE

#endif // QDEVICEINFO_H
