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

#include "qdeviceinfo_win_p.h"
#include "qnetworkinfo_win_p.h"
#include "qscreensaver_win_p.h"

#include <QtCore/qsettings.h>

#include <winsock2.h>
#include <qt_windows.h>
#include <vfw.h>
#if !defined(Q_CC_MINGW) || defined(__MINGW64_VERSION_MAJOR)
#  include <ws2bth.h>
#  include <bthsdpdef.h>
#  include <bluetoothapis.h>
#endif

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC_WITH_ARGS(const QString, REGISTRY_BIOS_PATH, (QLatin1String("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\BIOS")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, REGISTRY_BIOS_PATH2, (QLatin1String("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Control\\SystemInformation")))

Q_GLOBAL_STATIC_WITH_ARGS(const QString, REGISTRY_CURRENT_VERSION_PATH, (QLatin1String("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, REGISTRY_MANUFACTURER_KEY, (QLatin1String("SystemManufacturer")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, REGISTRY_PRODUCTNAME_KEY, (QLatin1String("SystemProductName")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, REGISTRY_PRODUCTID_KEY, (QLatin1String("ProductId")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, REGISTRY_OSNAME_KEY, (QLatin1String("ProductName")))
Q_GLOBAL_STATIC_WITH_ARGS(const QString, REGISTRY_BIOSVERSION_KEY, (QLatin1String("BIOSVersion")))

QDeviceInfoPrivate::QDeviceInfoPrivate(QDeviceInfo *parent)
    : q_ptr(parent)
{
}

bool QDeviceInfoPrivate::hasFeature(QDeviceInfo::Feature feature)
{
    switch (feature) {
#if !defined(Q_CC_MINGW) || defined(__MINGW64_VERSION_MAJOR)
    case QDeviceInfo::BluetoothFeature: {
        BLUETOOTH_DEVICE_SEARCH_PARAMS searchParameter;
        searchParameter.dwSize = sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS);
        searchParameter.fReturnAuthenticated = TRUE;
        searchParameter.fReturnRemembered = TRUE;
        searchParameter.fReturnUnknown = TRUE;
        searchParameter.fReturnConnected = TRUE;
        searchParameter.fIssueInquiry = TRUE;
        searchParameter.cTimeoutMultiplier = 1;
        searchParameter.hRadio = NULL;

        BLUETOOTH_DEVICE_INFO deviceInfo;
        HBLUETOOTH_DEVICE_FIND handle = BluetoothFindFirstDevice(&searchParameter, &deviceInfo);
        if (handle) {
            BluetoothFindDeviceClose(handle);
            return true;
        } else {
            return false;
        }
    }

    case QDeviceInfo::InfraredFeature: {
        WSADATA wsaData;
        if (0 == WSAStartup(MAKEWORD(1,1), &wsaData)) {
            SOCKET irdaSocket = socket(AF_IRDA, SOCK_STREAM, 0);
            if (INVALID_SOCKET != irdaSocket) {
                closesocket(irdaSocket);
                return true;
            }
        }
        return false;
    }

    case QDeviceInfo::CameraFeature: {
        char name[256];
        char version[256];
        for (WORD i = 0; i < 10; i++) {
            if (capGetDriverDescriptionA(i, name, 256, version, 256))
                return true;
        }
        return false;
    }
#else // !defined(Q_CC_MINGW) || defined(__MINGW64_VERSION_MAJOR)
    case QDeviceInfo::BluetoothFeature:
    case QDeviceInfo::InfraredFeature:
    case QDeviceInfo::CameraFeature:
        Q_UNIMPLEMENTED();
        return false;
#endif
    case QDeviceInfo::WlanFeature:
        return QNetworkInfoPrivate::wifiInterfaceCount() > 0;

    case QDeviceInfo::VideoOutFeature:
        return (GetSystemMetrics(SM_CMONITORS) > 0);

//    not sure if we can use WDK, thus not implemented as of now
//    case QDeviceInfo::MemoryCard:
//    case QDeviceInfo::Usb:

//    case QDeviceInfo::Led:
//    case QDeviceInfo::Positioning:
//    case QDeviceInfo::FmRadio:
//    case QDeviceInfo::FmTransmitter:
//    case QDeviceInfo::Vibration:
//    case QDeviceInfo::Sim:
//    case QDeviceInfo::Haptics:
//    case QDeviceInfo::Nfc:
    default:
        return false;
    }
}

int QDeviceInfoPrivate::imeiCount()
{
    return -1;
}

QDeviceInfo::LockTypeFlags QDeviceInfoPrivate::activatedLocks()
{
    QDeviceInfo::LockTypeFlags types = QDeviceInfo::NoLock;

    bool value(false);
    SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &value, 0);
    if (value)
        types |= QDeviceInfo::TouchOrKeyboardLock;

    if (const HDESK desktop = OpenDesktopA(const_cast<char *>("Default"), 0, false,
                                           DESKTOP_SWITCHDESKTOP)) {
        if (0 == SwitchDesktop(desktop))
            types |= QDeviceInfo::PinLock;
        CloseDesktop(desktop);
    }

    return types;
}

QDeviceInfo::LockTypeFlags QDeviceInfoPrivate::enabledLocks()
{
    QDeviceInfo::LockTypeFlags types = QDeviceInfo::PinLock; // We can always lock the computer

    QScreenSaverPrivate screenSaverPrivate(0);
    if (screenSaverPrivate.screenSaverEnabled())
        types |= QDeviceInfo::TouchOrKeyboardLock;

    return types;
}

QDeviceInfo::ThermalState QDeviceInfoPrivate::thermalState()
{
    return QDeviceInfo::UnknownThermal;
}

QString QDeviceInfoPrivate::imei(int /* interfaceNumber */)
{
    return QString();
}

QString QDeviceInfoPrivate::manufacturer()
{
    if (systemManufacturerName.isEmpty()) {
        QSettings manufacturerSetting(*REGISTRY_BIOS_PATH(), QSettings::NativeFormat);
        systemManufacturerName = manufacturerSetting.value(*REGISTRY_MANUFACTURER_KEY()).toString();
    }
    if (systemManufacturerName.isEmpty()) {
        QSettings manufacturerSetting2(*REGISTRY_BIOS_PATH2(), QSettings::NativeFormat);
        systemManufacturerName = manufacturerSetting2.value(*REGISTRY_MANUFACTURER_KEY()).toString();
    }
    return systemManufacturerName;
}

QString QDeviceInfoPrivate::model()
{
    return productName();
}

QString QDeviceInfoPrivate::productName()
{
    if (systemProductName.isEmpty()) {
        QSettings productNameSetting(*REGISTRY_BIOS_PATH(), QSettings::NativeFormat);
        systemProductName = productNameSetting.value(*REGISTRY_PRODUCTNAME_KEY()).toString();
    }
    if (systemProductName.isEmpty()) {
        QSettings productNameSetting2(*REGISTRY_BIOS_PATH2(), QSettings::NativeFormat);
        systemProductName = productNameSetting2.value(*REGISTRY_PRODUCTNAME_KEY()).toString();
    }
    return systemProductName;
}

QString QDeviceInfoPrivate::uniqueDeviceID()
{
    if (deviceID.isEmpty()) {
        QSettings deviceIDSetting(*REGISTRY_CURRENT_VERSION_PATH(), QSettings::NativeFormat);
        deviceID = deviceIDSetting.value(*REGISTRY_PRODUCTID_KEY()).toString();
    }
    return deviceID;
}

QString QDeviceInfoPrivate::version(QDeviceInfo::Version type)
{
    switch (type) {
    case QDeviceInfo::Os:
        if (osVersion.isEmpty()) {
            OSVERSIONINFOEX versionInfo;
            versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
            if (GetVersionEx((_OSVERSIONINFOW*)&versionInfo)) {
                osVersion = QString::number(versionInfo.dwMajorVersion) + QLatin1Char('.') + QString::number(versionInfo.dwMinorVersion) + QLatin1Char('.')
                            + QString::number(versionInfo.dwBuildNumber) + QLatin1Char('.') + QString::number(versionInfo.wServicePackMajor) + QLatin1Char('.')
                            + QString::number(versionInfo.wServicePackMinor);
            }
        }
        return osVersion;

    case QDeviceInfo::Firmware:
        {
            QSettings biosVersionSetting(*REGISTRY_BIOS_PATH2(), QSettings::NativeFormat);
            return biosVersionSetting.value(*REGISTRY_BIOSVERSION_KEY()).toString();
        }
    }
    return QString();
}

QString QDeviceInfoPrivate::operatingSystemName()
{
    QSettings deviceIDSetting(*REGISTRY_CURRENT_VERSION_PATH(), QSettings::NativeFormat);
    QString ver = deviceIDSetting.value(*REGISTRY_OSNAME_KEY()).toString();

    return ver;
}

QString QDeviceInfoPrivate::boardName()
{
    return productName();
}

bool QDeviceInfoPrivate::currentBluetoothPowerState()
{
    // windows still has fragmented bt API's
    return false;
}

QT_END_NAMESPACE
