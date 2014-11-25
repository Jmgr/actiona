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

#include "qnetworkinfo_win_p.h"

#include <QMutex>
#include <QLibraryInfo>
#include <QtCore/qlibrary.h>
#include <QtCore/QTimer>
#include <QtCore/QTimerEvent>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/qt_windows.h>

#include <QtCore/qmetaobject.h>

#if !defined( Q_CC_MINGW)
#ifndef Q_OS_WINCE
#include "qwmihelper_win_p.h"

enum NDIS_MEDIUM {
    NdisMedium802_3 = 0,
    NdisMediumWirelessWan = 9,
};

enum NDIS_PHYSICAL_MEDIUM {
    NdisPhysicalMediumUnspecified = 0,
    NdisPhysicalMediumWirelessLan = 1,
    NdisPhysicalMediumBluetooth = 10,
    NdisPhysicalMediumWiMax = 12,
};

#define OID_GEN_MEDIA_SUPPORTED 0x00010103
#define OID_GEN_PHYSICAL_MEDIUM 0x00010202

#define IOCTL_NDIS_QUERY_GLOBAL_STATS \
    CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, 0, METHOD_OUT_DIRECT, FILE_ANY_ACCESS)

#endif
#endif


#define WLAN_MAX_NAME_LENGTH 256
#define DOT11_SSID_MAX_LENGTH 32
#define WLAN_NOTIFICATION_SOURCE_ALL 0x0000ffff

enum WLAN_INTF_OPCODE {
    wlan_intf_opcode_autoconf_start = 0x000000000,
    wlan_intf_opcode_autoconf_enabled,
    wlan_intf_opcode_background_scan_enabled,
    wlan_intf_opcode_media_streaming_mode,
    wlan_intf_opcode_radio_state,
    wlan_intf_opcode_batteryStatuss_type,
    wlan_intf_opcode_interface_state,
    wlan_intf_opcode_current_connection,
    wlan_intf_opcode_channel_number,
    wlan_intf_opcode_supported_infrastructure_auth_cipher_pairs,
    wlan_intf_opcode_supported_adhoc_auth_cipher_pairs,
    wlan_intf_opcode_supported_country_or_region_string_list,
    wlan_intf_opcode_current_operation_mode,
    wlan_intf_opcode_supported_safe_mode,
    wlan_intf_opcode_certified_safe_mode,
    wlan_intf_opcode_autoconf_end = 0x0fffffff,
    wlan_intf_opcode_msm_start = 0x10000100,
    wlan_intf_opcode_statistics,
    wlan_intf_opcode_rssi,
    wlan_intf_opcode_msm_end = 0x1fffffff,
    wlan_intf_opcode_security_start = 0x20010000,
    wlan_intf_opcode_security_end = 0x2fffffff,
    wlan_intf_opcode_ihv_start = 0x30000000,
    wlan_intf_opcode_ihv_end = 0x3fffffff
};

enum WLAN_OPCODE_VALUE_TYPE {
    wlan_opcode_value_type_query_only = 0,
    wlan_opcode_value_type_set_by_group_policy,
    wlan_opcode_value_type_set_by_user,
    wlan_opcode_value_type_invalid
};

enum WLAN_INTERFACE_STATE {
    wlan_interface_state_not_ready = 0,
    wlan_interface_state_connected,
    wlan_interface_state_ad_hoc_network_formed,
    wlan_interface_state_disconnecting,
    wlan_interface_state_disconnected,
    wlan_interface_state_associating,
    wlan_interface_state_discovering,
    wlan_interface_state_authenticating
};

struct WLAN_INTERFACE_INFO {
    GUID InterfaceGuid;
    WCHAR strInterfaceDescription[WLAN_MAX_NAME_LENGTH];
    WLAN_INTERFACE_STATE isState;
};

struct WLAN_INTERFACE_INFO_LIST {
    DWORD dwNumberOfItems;
    DWORD dwIndex;
    WLAN_INTERFACE_INFO InterfaceInfo[1];
};

struct WLAN_NOTIFICATION_DATA {
    DWORD NotificationSource;
    DWORD NotificationCode;
    GUID InterfaceGuid;
    DWORD dwDataSize;
    PVOID pData;
};

enum WLAN_CONNECTION_MODE {
    wlan_connection_mode_profile = 0,
    wlan_connection_mode_temporary_profile,
    wlan_connection_mode_discovery_secure,
    wlan_connection_mode_discovery_unsecure,
    wlan_connection_mode_auto,
    wlan_connection_mode_invalid
};

enum DOT11_PHY_TYPE {
        dot11_phy_type_unknown = 0,
        dot11_phy_type_any = dot11_phy_type_unknown,
        dot11_phy_type_fhss = 1,
        dot11_phy_type_dsss = 2,
        dot11_phy_type_irbaseband = 3,
        dot11_phy_type_ofdm = 4,
        dot11_phy_type_hrdsss = 5,
        dot11_phy_type_erp = 6,
        dot11_phy_type_ht = 7,
        dot11_phy_type_IHV_start = 0x80000000,
        dot11_phy_type_IHV_end = 0xffffffff
    };


enum DOT11_AUTH_ALGORITHM {
    DOT11_AUTH_ALGO_80211_OPEN = 1,
    DOT11_AUTH_ALGO_80211_SHARED_KEY = 2,
    DOT11_AUTH_ALGO_WPA = 3,
    DOT11_AUTH_ALGO_WPA_PSK = 4,
    DOT11_AUTH_ALGO_WPA_NONE = 5,
    DOT11_AUTH_ALGO_RSNA = 6,
    DOT11_AUTH_ALGO_RSNA_PSK = 7,
    DOT11_AUTH_ALGO_IHV_START = 0x80000000,
    DOT11_AUTH_ALGO_IHV_END = 0xffffffff
};


enum DOT11_CIPHER_ALGORITHM {
    DOT11_CIPHER_ALGO_NONE = 0x00,
    DOT11_CIPHER_ALGO_WEP40 = 0x01,
    DOT11_CIPHER_ALGO_TKIP = 0x02,
    DOT11_CIPHER_ALGO_CCMP = 0x04,
    DOT11_CIPHER_ALGO_WEP104 = 0x05,
    DOT11_CIPHER_ALGO_WPA_USE_GROUP = 0x100,
    DOT11_CIPHER_ALGO_RSN_USE_GROUP = 0x100,
    DOT11_CIPHER_ALGO_WEP = 0x101,
    DOT11_CIPHER_ALGO_IHV_START = 0x80000000,
    DOT11_CIPHER_ALGO_IHV_END = 0xffffffff
};


enum DOT11_batteryStatusS_TYPE {
    dot11_batteryStatusS_type_infrastructure = 1,
    dot11_batteryStatusS_type_independent = 2,
    dot11_batteryStatusS_type_any = 3
};


struct DOT11_SSID {
    ULONG uSSIDLength;
    UCHAR ucSSID[DOT11_SSID_MAX_LENGTH];
};

typedef UCHAR DOT11_MAC_ADDRESS[6];

struct WLAN_ASSOCIATION_ATTRIBUTES {
    DOT11_SSID dot11Ssid;
    DOT11_batteryStatusS_TYPE dot11batteryStatussType;
    DOT11_MAC_ADDRESS dot11batteryStatussid;
    DOT11_PHY_TYPE dot11PhyType;
    ULONG uDot11PhyIndex;
    ULONG wlanSignalQuality;
    ULONG ulRxRate;
    ULONG ulTxRate;
};

struct WLAN_SECURITY_ATTRIBUTES {
    BOOL batteryStatusecurityEnabled;
    BOOL bOneXEnabled;
    DOT11_AUTH_ALGORITHM dot11AuthAlgorithm;
    DOT11_CIPHER_ALGORITHM dot11CipherAlgorithm;
};

struct WLAN_CONNECTION_ATTRIBUTES {
    WLAN_INTERFACE_STATE isState;
    WLAN_CONNECTION_MODE wlanConnectionMode;
    WCHAR strProfileName[WLAN_MAX_NAME_LENGTH];
    WLAN_ASSOCIATION_ATTRIBUTES wlanAssociationAttributes;
    WLAN_SECURITY_ATTRIBUTES wlanSecurityAttributes;
};

enum WLAN_NOTIFICATION_ACM {
    wlan_notification_acm_start = 0,
    wlan_notification_acm_autoconf_enabled,
    wlan_notification_acm_autoconf_disabled,
    wlan_notification_acm_background_scan_enabled,
    wlan_notification_acm_background_scan_disabled,
    wlan_notification_acm_batteryStatuss_type_change,
    wlan_notification_acm_power_setting_change,
    wlan_notification_acm_scan_complete,
    wlan_notification_acm_scan_fail,
    wlan_notification_acm_connection_start,
    wlan_notification_acm_connection_complete,
    wlan_notification_acm_connection_attempt_fail,
    wlan_notification_acm_filter_list_change,
    wlan_notification_acm_interface_arrival,
    wlan_notification_acm_interface_removal,
    wlan_notification_acm_profile_change,
    wlan_notification_acm_profile_name_change,
    wlan_notification_acm_profiles_exhausted,
    wlan_notification_acm_network_not_available,
    wlan_notification_acm_network_available,
    wlan_notification_acm_disconnecting,
    wlan_notification_acm_disconnected,
    wlan_notification_acm_adhoc_network_state_change,
    wlan_notification_acm_end
};

enum _WLAN_NOTIFICATION_MSM {
    wlan_notification_msm_start = 0,
    wlan_notification_msm_associating,
    wlan_notification_msm_associated,
    wlan_notification_msm_authenticating,
    wlan_notification_msm_connected,
    wlan_notification_msm_roaming_start,
    wlan_notification_msm_roaming_end,
    wlan_notification_msm_radio_state_change,
    wlan_notification_msm_signal_quality_change,
    wlan_notification_msm_disassociating,
    wlan_notification_msm_disconnected,
    wlan_notification_msm_peer_join,
    wlan_notification_msm_peer_leave,
    wlan_notification_msm_adapter_removal,
    wlan_notification_msm_adapter_operation_mode_change,
    wlan_notification_msm_end
};

typedef void (WINAPI *WLAN_NOTIFICATION_CALLBACK) (WLAN_NOTIFICATION_DATA *, PVOID);

//#define _WCHAR_T_DEFINED
//#define _TIME64_T_DEFINED

typedef struct _BLUETOOTH_FIND_RADIO_PARAMS {
    DWORD   dwSize;             //  IN  sizeof this structure
} BLUETOOTH_FIND_RADIO_PARAMS;


typedef DWORD (WINAPI *WlanOpenHandleProto)
    (DWORD dwClientVersion, PVOID pReserved, PDWORD pdwNegotiatedVersion, PHANDLE phClientHandle);
static WlanOpenHandleProto local_WlanOpenHandle = 0;

typedef DWORD (WINAPI *WlanCloseHandleProto)(HANDLE hClientHandle, PVOID pReserved);
static WlanCloseHandleProto local_WlanCloseHandle = 0;

#if !defined(Q_OS_WINCE)
typedef DWORD (WINAPI *WlanEnumInterfacesProto)
    (HANDLE hClientHandle, PVOID pReserved, WLAN_INTERFACE_INFO_LIST **ppInterfaceList);
static WlanEnumInterfacesProto local_WlanEnumInterfaces = 0;

typedef DWORD (WINAPI *WlanQueryInterfaceProto)
    (HANDLE hClientHandle, const GUID *pInterfaceGuid, WLAN_INTF_OPCODE OpCode, PVOID pReserved,
     PDWORD pdwDataSize, PVOID *ppData, WLAN_OPCODE_VALUE_TYPE *pWlanOpcodeValueType);
static WlanQueryInterfaceProto local_WlanQueryInterface = 0;

typedef DWORD (WINAPI *WlanRegisterNotificationProto)
    (HANDLE hClientHandle, DWORD dwNotifSource, BOOL bIgnoreDuplicate,
     WLAN_NOTIFICATION_CALLBACK funcCallback, PVOID pCallbackContext,
     PVOID pReserved, PDWORD pdwPrevNotifSource);
static WlanRegisterNotificationProto local_WlanRegisterNotification = 0;
#endif

typedef VOID (WINAPI *WlanFreeMemoryProto)(PVOID pMemory);
static WlanFreeMemoryProto local_WlanFreeMemory = 0;

typedef BOOL (WINAPI *BluetoothFindRadioClose)(HANDLE hFind);
static BluetoothFindRadioClose local_BluetoothFindRadioClose=0;

typedef HANDLE (WINAPI *BluetoothFindFirstRadio)(const BLUETOOTH_FIND_RADIO_PARAMS * pbtfrp,HANDLE * phRadio);
static BluetoothFindFirstRadio local_BluetoothFindFirstRadio=0;


#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)
static WLAN_CONNECTION_ATTRIBUTES *getWifiConnectionAttributes()
{
    if (!local_WlanOpenHandle)
        return NULL;
    DWORD version =  0;
    HANDLE clientHandle = NULL;
    DWORD result;
    WLAN_CONNECTION_ATTRIBUTES  *connAtts = NULL;

    WLAN_INTERFACE_INFO_LIST *interfacesInfoList = NULL;
    result = local_WlanOpenHandle( 2, NULL, &version, &clientHandle );
    if ( result != ERROR_SUCCESS) {
        local_WlanFreeMemory(connAtts);
        local_WlanCloseHandle(clientHandle,  0);
        return NULL;
    }
    result = local_WlanEnumInterfaces(clientHandle, NULL, &interfacesInfoList);

    if ( result != ERROR_SUCCESS) {
        local_WlanCloseHandle(clientHandle, NULL);
        local_WlanFreeMemory(connAtts);
        return NULL;
    }

    for ( uint i = 0; i < interfacesInfoList->dwNumberOfItems; i++ ) {
        WLAN_INTERFACE_INFO *interfaceInfo = &interfacesInfoList->InterfaceInfo[i];
        GUID& guid = interfaceInfo->InterfaceGuid;
        WLAN_INTERFACE_STATE wlanInterfaceState = interfaceInfo->isState;

        if ( wlanInterfaceState == wlan_interface_state_not_ready ) {
            continue;
        }

        ULONG size = 0;
        result = local_WlanQueryInterface( clientHandle, &guid,  wlan_intf_opcode_current_connection, NULL, &size, (PVOID*) &connAtts, NULL );
        if ( result != ERROR_SUCCESS ) {
            continue;
        }
    }
    local_WlanCloseHandle(clientHandle, NULL);
    return connAtts;
}
#endif

#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)

void wlanNotificationCallback(WLAN_NOTIFICATION_DATA *pNotifyData, QNetworkInfoPrivate *netInfo)
{
    // xp only supports disconnected and complete
    switch (pNotifyData->NotificationCode) {
    case wlan_notification_acm_scan_complete:
        break;
    case wlan_notification_acm_disconnecting:
        break;
    case wlan_notification_acm_disconnected:
    case wlan_notification_acm_connection_attempt_fail:
    case wlan_notification_acm_network_not_available:
        netInfo->emitNetworkStatusChanged(QNetworkInfo::WlanMode,
                                          QNetworkInfo::NoNetworkAvailable);
        break;
    case wlan_notification_acm_profile_name_change:
        break;
    case  wlan_notification_msm_connected:
        netInfo->emitNetworkStatusChanged(QNetworkInfo::WlanMode,
                                          QNetworkInfo::HomeNetwork);
        break;
    case wlan_notification_acm_connection_start:
        netInfo->emitNetworkStatusChanged(QNetworkInfo::WlanMode,
                                          QNetworkInfo::Searching);
        break;
    case wlan_notification_msm_signal_quality_change:
        netInfo->emitNetworkSignalStrengthChanged(QNetworkInfo::WlanMode,
                                                  reinterpret_cast<qint32>(pNotifyData->pData));
        break;
    default:
        break;
    };
}
#endif

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC_WITH_ARGS(QMutex, dynamicLoadMutex, (QMutex::Recursive));

static void resolveLibrary()
{
#if !defined( Q_OS_WINCE)
            static volatile bool triedResolve = false;

    if (!triedResolve) {
#ifndef QT_NO_THREAD
        QMutexLocker locker(dynamicLoadMutex());
#endif

        if (!triedResolve) {
            local_WlanOpenHandle = (WlanOpenHandleProto)
                                   QLibrary::resolve(QLatin1String("wlanapi.dll"), "WlanOpenHandle");
            local_WlanRegisterNotification = (WlanRegisterNotificationProto)
                                             QLibrary::resolve(QLatin1String("wlanapi.dll"), "WlanRegisterNotification");
            local_WlanEnumInterfaces = (WlanEnumInterfacesProto)
                                       QLibrary::resolve(QLatin1String("wlanapi.dll"), "WlanEnumInterfaces");
            local_WlanQueryInterface = (WlanQueryInterfaceProto)
                                       QLibrary::resolve(QLatin1String("wlanapi.dll"), "WlanQueryInterface");
            local_WlanFreeMemory = (WlanFreeMemoryProto)
                                   QLibrary::resolve(QLatin1String("wlanapi.dll"), "WlanFreeMemory");
            local_WlanCloseHandle = (WlanCloseHandleProto)
                                    QLibrary::resolve(QLatin1String("wlanapi.dll"), "WlanCloseHandle");


            local_BluetoothFindFirstRadio = (BluetoothFindFirstRadio)
                                            QLibrary::resolve(QLatin1String("Bthprops.cpl"), "BluetoothFindFirstRadio");
            local_BluetoothFindRadioClose = (BluetoothFindRadioClose)
                                            QLibrary::resolve(QLatin1String("Bthprops.cpl"), "BluetoothFindRadioClose");

            triedResolve = true;
        }
    }
#endif
}

QNetworkInfoPrivate::QNetworkInfoPrivate(QNetworkInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
{
    wlanCallbackInitialized = false;

    resolveLibrary();

    startWifiCallback();

     timerMs = 5000;
     switch (QSysInfo::WindowsVersion) {
     case  QSysInfo::WV_VISTA:
     case QSysInfo::WV_WINDOWS7:
         break;
     default:
         {
             if (local_WlanOpenHandle)
                 QTimer::singleShot(timerMs, this, SLOT(networkStrengthTimeout()));
         }
         break;
     };
}
QNetworkInfoPrivate::~QNetworkInfoPrivate()
{
#if !defined( Q_OS_WINCE)
    if (hWlan != 0)
         local_WlanCloseHandle(hWlan, 0);
#endif
}

QNetworkInfoPrivate *QNetworkInfoPrivate::instance()
{
    resolveLibrary();
    static QNetworkInfoPrivate *self = 0;
    if (!self) {
        self = new QNetworkInfoPrivate;
    }
    return self;
}

// Called by the feature detection code.
unsigned QNetworkInfoPrivate::wifiInterfaceCount()
{
#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)
    resolveLibrary();
    if (!local_WlanOpenHandle)
        return 0;
    DWORD negotiatedVersion;
    HANDLE handle;
    if (ERROR_SUCCESS != local_WlanOpenHandle(1, NULL, &negotiatedVersion, &handle))
        return 0;
    WLAN_INTERFACE_INFO_LIST *list;
    unsigned result = 0;
    if (ERROR_SUCCESS == local_WlanEnumInterfaces(handle, NULL, &list)) {
        result = list->dwNumberOfItems;
        local_WlanFreeMemory(list);
    }
    local_WlanCloseHandle(handle, NULL);
    return result;
#else // !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)
    return 0;
#endif
}

void QNetworkInfoPrivate::startWifiCallback()
{
#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)
    if (networkStatus(QNetworkInfo::WlanMode, 1) != QNetworkInfo::HomeNetwork
    && wlanCallbackInitialized){
        return;
    }
    DWORD version =  0;
    hWlan = NULL;
    DWORD result;
    if (local_WlanOpenHandle)
        result = local_WlanOpenHandle(2, NULL, &version, &hWlan );
    if ( result != ERROR_SUCCESS ) {
        qDebug() << "Error opening Wlanapi 2" << result ;
        return ;
    }
    if ( result != ERROR_SUCCESS) {
        qDebug() << "Error in enumerating wireless interfaces" << result;
        return ;
    }
    if (ERROR_SUCCESS != local_WlanRegisterNotification(hWlan,
                                                  WLAN_NOTIFICATION_SOURCE_ALL,
                                                  true,
                                                  WLAN_NOTIFICATION_CALLBACK(wlanNotificationCallback),
                                                  this, 0, 0)) {
        qDebug() << "failed";
    } else {
        wlanCallbackInitialized = true;
    }
#endif
}


void QNetworkInfoPrivate::emitNetworkStatusChanged(QNetworkInfo::NetworkMode mode,
                                                         QNetworkInfo::NetworkStatus status)
{
    emit networkStatusChanged(mode, 1, status);
    if (status == QNetworkInfo::NoNetworkAvailable
        || status == QNetworkInfo::HomeNetwork) {
        startWifiCallback();
        emit networkNameChanged(mode, 1, networkName(mode, 1));
        if (isDefaultMode(mode)) {
            emit currentNetworkModeChanged(mode);
        }
    } else {
        networkSignalStrengthChanged(mode, 1, 0);
    }
}

void QNetworkInfoPrivate::emitNetworkSignalStrengthChanged(QNetworkInfo::NetworkMode mode,int /*strength*/)
{
    switch (QSysInfo::WindowsVersion) {
    case QSysInfo::WV_VISTA:
    case QSysInfo::WV_WINDOWS7:
        break;
    default:
        QTimer::singleShot(timerMs, this, SLOT(networkStrengthTimeout()));
        break;
    };
    networkSignalStrength(mode, 1);
}

void QNetworkInfoPrivate::timerEvent(QTimerEvent *event)
{
   if (event->timerId() == netStrengthTimer.timerId()) {
      networkStrengthTimeout();
   } else {
      QObject::timerEvent(event);
   }
}


void QNetworkInfoPrivate::networkStrengthTimeout()
{
    QList<QNetworkInfo::NetworkMode> modeList;
    modeList << QNetworkInfo::GsmMode;
    modeList << QNetworkInfo::CdmaMode;
    modeList << QNetworkInfo::WcdmaMode;
    modeList << QNetworkInfo::WlanMode;
    modeList << QNetworkInfo::EthernetMode;
    modeList << QNetworkInfo::BluetoothMode;
    modeList << QNetworkInfo::WimaxMode;

    foreach (const QNetworkInfo::NetworkMode mode, modeList) {
        networkSignalStrength(mode, 1);
    }
    switch (QSysInfo::WindowsVersion) {
    case QSysInfo::WV_VISTA:
    case QSysInfo::WV_WINDOWS7:
        break;
    default:
        if (local_WlanOpenHandle)
            QTimer::singleShot(timerMs, this, SLOT(networkStrengthTimeout()));
        break;
    };
 }

void QNetworkInfoPrivate::networkStatusTimeout()
{
    QList<QNetworkInfo::NetworkMode> modeList;
    modeList << QNetworkInfo::GsmMode;
    modeList << QNetworkInfo::CdmaMode;
    modeList << QNetworkInfo::WcdmaMode;
    modeList << QNetworkInfo::WlanMode;
    modeList << QNetworkInfo::EthernetMode;
    modeList << QNetworkInfo::BluetoothMode;
    modeList << QNetworkInfo::WimaxMode;

    foreach (const QNetworkInfo::NetworkMode mode, modeList) {
        networkStatus(mode,1);
    }

 }

int QNetworkInfoPrivate::networkInterfaceCount(QNetworkInfo::NetworkMode mode)
{
    switch (mode) {
    case QNetworkInfo::WlanMode:
    case QNetworkInfo::EthernetMode:
    case QNetworkInfo::BluetoothMode:

//    case QNetworkInfo::GsmMode:
//    case QNetworkInfo::CdmaMode:
//    case QNetworkInfo::WcdmaMode:
//    case QNetworkInfo::WimaxMode:
//    case QNetworkInfo::LteMode:
//    case QNetworkInfo::TdscdmaMode:
    default:
        break;
    };

    return -1;
}

int QNetworkInfoPrivate::networkSignalStrength(QNetworkInfo::NetworkMode mode, int netInterface)
{
    Q_UNUSED(netInterface)

    switch (mode) {
    case QNetworkInfo::WlanMode:
    {

#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)
        if (local_WlanOpenHandle) {

            DWORD version =  0;
            DWORD result;

            WLAN_INTERFACE_INFO_LIST *interfacesInfoList = NULL;
            if (hWlan ==0) {
                result = local_WlanOpenHandle( 2, NULL, &version, &hWlan );
                if ( result != ERROR_SUCCESS ) {
                    qDebug() << "Error opening Wlanapi 3" << result ;
                    local_WlanCloseHandle(hWlan,  0);
                    return 0;
                }
            }
            result = local_WlanEnumInterfaces(hWlan, NULL, &interfacesInfoList);

            if ( result != ERROR_SUCCESS) {
                qDebug() << "Error in enumerating wireless interfaces" << result;
                local_WlanCloseHandle(hWlan,  0);
                return 0;
            }

            for ( uint i = 0; i < interfacesInfoList->dwNumberOfItems; i++ ) {
                WLAN_INTERFACE_INFO *interfaceInfo = &interfacesInfoList->InterfaceInfo[i];
                GUID& guid = interfaceInfo->InterfaceGuid;
                WLAN_INTERFACE_STATE wlanInterfaceState = interfaceInfo->isState;

                if ( wlanInterfaceState == wlan_interface_state_not_ready ) {
                    qDebug() << "Interface not ready";
                    continue;
                }

                ULONG size = 0;
                WLAN_CONNECTION_ATTRIBUTES  *connAtts = NULL;
                result = local_WlanQueryInterface( hWlan, &guid,
                                                   wlan_intf_opcode_current_connection, NULL, &size, (PVOID*) &connAtts, NULL );

                if ( result != ERROR_SUCCESS ) {
                    continue;
                }
                ulong sig =  connAtts->wlanAssociationAttributes.wlanSignalQuality;
                local_WlanFreeMemory(connAtts);
                local_WlanFreeMemory(interfacesInfoList);

                if (sig != wifiStrength) {
                    emit networkSignalStrengthChanged(mode,1, sig);
                    wifiStrength = sig;
                }
                return sig;
            }
        }
#endif
    }
    break;
    case QNetworkInfo::EthernetMode:
    {
#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)
        WMIHelper *wHelper;
        wHelper = new WMIHelper(this);
        wHelper->setWmiNamespace(QLatin1String("root/cimv2"));
        wHelper->setClassName(QLatin1String("Win32_NetworkAdapter"));
        wHelper->setClassProperty(QStringList() << QLatin1String("NetConnectionStatus"));
        QString cond = QLatin1String("WHERE MACAddress = '%1'");
        cond = cond.arg( interfaceForMode(mode, 1).hardwareAddress());
        wHelper->setConditional(cond);
        QVariant v = wHelper->getWMIData();
        quint32 strength = v.toUInt();
        quint32 tmpStrength;

        if ( strength == 2
           || strength == 9) {
            tmpStrength = 100;
        } else {
           tmpStrength = 0;
        }

        if (tmpStrength != ethStrength) {
            ethStrength = tmpStrength;
            emit networkSignalStrengthChanged(mode, 1, ethStrength);
        }

        return ethStrength;
#endif
    }
    break;
    case QNetworkInfo::BluetoothMode:

//    case QNetworkInfo::GsmMode:
//    case QNetworkInfo::CdmaMode:
//    case QNetworkInfo::WcdmaMode:
//    case QNetworkInfo::WimaxMode:
//    case QNetworkInfo::LteMode:
//    case QNetworkInfo::TdscdmaMode:
    default:
        break;
    };

    return -1;
}

QNetworkInfo::CellDataTechnology QNetworkInfoPrivate::currentCellDataTechnology(int netInterface)
{
    Q_UNUSED(netInterface)
    return QNetworkInfo::UnknownDataTechnology;
}

bool QNetworkInfoPrivate::isDefaultMode(QNetworkInfo::NetworkMode mode)
{
    bool isDefaultGateway = false;
    QNetworkInterface netInterface = interfaceForMode(mode,1);
    QString deviceNameReg;

    switch (QSysInfo::WindowsVersion) {
    case  QSysInfo::WV_VISTA:
    case QSysInfo::WV_WINDOWS7:
        deviceNameReg =  QLatin1String("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\%1\\");
        deviceNameReg = deviceNameReg.arg(netInterface.name());
        break;
    default:
        deviceNameReg = QLatin1String("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\%1\\Parameters\\Tcpip");
        deviceNameReg = deviceNameReg.arg(netInterface.name());
        break;
    };

    QSettings deviceSettings(deviceNameReg, QSettings::NativeFormat);
    QString dhcpGw = QLatin1String("DhcpDefaultGateway");
    QString gw = QLatin1String("DefaultGateway");
    if (!deviceSettings.value(dhcpGw).toStringList().isEmpty()
            || !deviceSettings.value(gw).toStringList().isEmpty()) {
        isDefaultGateway = true;
    }
        return isDefaultGateway;
}
QNetworkInfo::NetworkMode QNetworkInfoPrivate::currentNetworkMode()
{
    QList <QNetworkInfo::NetworkMode> modeList;
    modeList << QNetworkInfo::EthernetMode
            << QNetworkInfo::WlanMode
            << QNetworkInfo::BluetoothMode
            << QNetworkInfo::WcdmaMode
            << QNetworkInfo::CdmaMode
            << QNetworkInfo::GsmMode
            << QNetworkInfo::WimaxMode;

    for (int i = 0; i < modeList.size(); ++i) {
        if ( isDefaultMode(modeList.at(i)))
            return modeList.at(i);
    }
        return QNetworkInfo::UnknownMode;
}

QNetworkInfo::NetworkStatus QNetworkInfoPrivate::networkStatus(QNetworkInfo::NetworkMode mode, int netInterface)
{
    Q_UNUSED(netInterface)

    switch (mode) {
    case QNetworkInfo::WlanMode:
    {
#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)
        if (local_WlanOpenHandle) {
            WLAN_CONNECTION_ATTRIBUTES *connAtts = getWifiConnectionAttributes();
            if (connAtts != NULL) {
                if (connAtts->isState  == wlan_interface_state_authenticating) {
                    local_WlanFreeMemory(connAtts);
                    return QNetworkInfo::Searching;
                }
                if (connAtts->isState  == wlan_interface_state_connected) {
                    local_WlanFreeMemory(connAtts);
                    return QNetworkInfo::HomeNetwork;
                }
            }
            local_WlanFreeMemory(connAtts);
        }
#endif
    }
        break;
    case QNetworkInfo::EthernetMode:
    {
#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)
        WMIHelper *wHelper;
        wHelper = new WMIHelper(this);
        wHelper->setWmiNamespace(QLatin1String("root/cimv2"));
        wHelper->setClassName(QLatin1String("Win32_NetworkAdapter"));
        wHelper->setClassProperty(QStringList() << QLatin1String("NetConnectionStatus"));
        QString cond = QLatin1String("WHERE MACAddress = '%1'");
        cond = cond.arg(interfaceForMode(mode,1).hardwareAddress());
        wHelper->setConditional(cond);
        QVariant v = wHelper->getWMIData();
        if (v.toUInt() == 2) {
            return QNetworkInfo::HomeNetwork;
        }
#endif
    }
        break;
    case QNetworkInfo::BluetoothMode:
    {
#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)
        WMIHelper *wHelper;
        wHelper = new WMIHelper(this);
        wHelper->setWmiNamespace(QLatin1String("root/cimv2"));
        wHelper->setClassName(QLatin1String("Win32_NetworkAdapter"));
        wHelper->setClassProperty(QStringList() << QLatin1String("NetConnectionStatus"));
        QString cond = QLatin1String("WHERE MACAddress = '%1'");
        cond = cond.arg(interfaceForMode(mode, 1).hardwareAddress());
        wHelper->setConditional(cond);
        QVariant v = wHelper->getWMIData();
        if (v.toUInt() == 2) {
            return QNetworkInfo::HomeNetwork;
        }
#endif
    }
        break;
//    case QNetworkInfo::GsmMode:
//    case QNetworkInfo::CdmaMode:
//    case QNetworkInfo::WcdmaMode:
//    case QNetworkInfo::WimaxMode:
//    case QNetworkInfo::LteMode:
//    case QNetworkInfo::TdscdmaMode:
        break;

    default:
        break;
    };

    return QNetworkInfo::UnknownStatus;
}

#ifndef QT_NO_NETWORKINTERFACE
QNetworkInterface QNetworkInfoPrivate::interfaceForMode(QNetworkInfo::NetworkMode mode, int netInterface)
{
    Q_UNUSED(netInterface)
    QList<QNetworkInterface> interfaceList;
    interfaceList = QNetworkInterface::allInterfaces();
    QListIterator<QNetworkInterface> i(interfaceList);
    while (i.hasNext()) {
       QNetworkInterface netInterface = i.next();
        if (!netInterface.isValid()
            || (netInterface.flags() & QNetworkInterface::IsLoopBack)
            || !(netInterface.flags() & QNetworkInterface::IsUp)
            || !(netInterface.flags() & QNetworkInterface::IsRunning)
            || netInterface.addressEntries().isEmpty()) {
            continue;
        }

#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)

        if (local_WlanOpenHandle) {
            unsigned long oid;
            DWORD bytesWritten;

            NDIS_MEDIUM medium ;
            NDIS_PHYSICAL_MEDIUM physicalMedium = NdisPhysicalMediumUnspecified;
            QString filestr = QLatin1String("\\\\.\\%1");
            HANDLE handle = CreateFile((TCHAR *)filestr.arg(netInterface.name()).utf16(), 0,
                                       FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

            if (handle == INVALID_HANDLE_VALUE) {
                continue;
            }

            oid = OID_GEN_MEDIA_SUPPORTED;
            bytesWritten;
            bool result = DeviceIoControl(handle, IOCTL_NDIS_QUERY_GLOBAL_STATS, &oid, sizeof(oid),
                                          &medium, sizeof(medium), &bytesWritten, 0);
            if (!result) {
                CloseHandle(handle);
                qDebug() << "DeviceIo result is false";
                return QNetworkInterface();
                continue;
            }

            oid = OID_GEN_PHYSICAL_MEDIUM;
            bytesWritten = 0;
            result = DeviceIoControl(handle, IOCTL_NDIS_QUERY_GLOBAL_STATS, &oid, sizeof(oid),
                                     &physicalMedium, sizeof(physicalMedium), &bytesWritten, 0);

            if (!result) {
                CloseHandle(handle);
                if (medium == NdisMedium802_3 && mode == QNetworkInfo::EthernetMode) {
                    return netInterface;
                } else {
                    continue;
                }
            }

            CloseHandle(handle);
            if (physicalMedium == NdisMediumWirelessWan && mode == QNetworkInfo::WlanMode) {
                //some wifi devices show up here
                return netInterface;
            }

            if (medium == NdisMedium802_3) {
                switch (physicalMedium) {
                case NdisPhysicalMediumUnspecified:
                    {
                        if (mode == QNetworkInfo::EthernetMode) {
                            return netInterface;
                        }
                    }
                    break;
                case NdisPhysicalMediumWirelessLan:
                    {
                        if (mode == QNetworkInfo::WlanMode) {
                            return netInterface;
                        }
                    }
                    break;
                case NdisPhysicalMediumBluetooth:
                    {
                        if (mode == QNetworkInfo::BluetoothMode) {
                            return netInterface;
                        }
                    }
                    break;
#ifdef NDIS_SUPPORT_NDIS6
                case NdisPhysicalMediumWiMax:
                    {
                        if (mode == QNetworkInfo::WimaxMode) {
                            return netInterface;
                        }
                    }
                    break;
#endif
                };
            }
        }
#endif
    }
//    Q_UNUSED(interface)

//    switch (mode) {
//    case QNetworkInfo::WlanMode:
//    case QNetworkInfo::EthernetMode:

////    case QNetworkInfo::BluetoothMode:
////    case QNetworkInfo::GsmMode:
////    case QNetworkInfo::CdmaMode:
////    case QNetworkInfo::WcdmaMode:
////    case QNetworkInfo::WimaxMode:
////    case QNetworkInfo::LteMode:
////    case QNetworkInfo::TdscdmaMode:
//    default:
//        break;
//    };

    return QNetworkInterface();
}
#endif // QT_NO_NETWORKINTERFACE

QString QNetworkInfoPrivate::cellId(int netInterface)
{
    Q_UNUSED(netInterface)
    return QString();
}

QString QNetworkInfoPrivate::currentMobileCountryCode(int netInterface)
{
    Q_UNUSED(netInterface)
    return QString();
}

QString QNetworkInfoPrivate::currentMobileNetworkCode(int netInterface)
{
    Q_UNUSED(netInterface)
    return QString();
}

QString QNetworkInfoPrivate::homeMobileCountryCode(int netInterface)
{
    Q_UNUSED(netInterface)
    return QString();
}

QString QNetworkInfoPrivate::homeMobileNetworkCode(int netInterface)
{
    Q_UNUSED(netInterface)
    return QString();
}

QString QNetworkInfoPrivate::imsi(int netInterface)
{
    Q_UNUSED(netInterface)
    return QString();
}

QString QNetworkInfoPrivate::locationAreaCode(int netInterface)
{
    Q_UNUSED(netInterface)
    return QString();
}

QString QNetworkInfoPrivate::macAddress(QNetworkInfo::NetworkMode mode, int netInterface)
{
    Q_UNUSED(netInterface)
    QString mac = interfaceForMode(mode,1).hardwareAddress();
    return mac;

//    switch (mode) {
//    case QNetworkInfo::WlanMode:
//    case QNetworkInfo::EthernetMode:
//    case QNetworkInfo::BluetoothMode:

////    case QNetworkInfo::GsmMode:
////    case QNetworkInfo::CdmaMode:
////    case QNetworkInfo::WcdmaMode:
////    case QNetworkInfo::WimaxMode:
////    case QNetworkInfo::LteMode:
////    case QNetworkInfo::TdscdmaMode:
//    default:
//        break;
//    };

    return QString();
}

QString QNetworkInfoPrivate::networkName(QNetworkInfo::NetworkMode mode, int netInterface)
{
    Q_UNUSED(netInterface)
    QString  netname;

    switch (mode) {
    case QNetworkInfo::WlanMode:
    {
#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)
        if (local_WlanOpenHandle) {
            WLAN_CONNECTION_ATTRIBUTES *connAtts = getWifiConnectionAttributes();
            if (connAtts != NULL) {
                DOT11_SSID ssid;
                ssid = connAtts->wlanAssociationAttributes.dot11Ssid;
                for (uint i = 0; i < ssid.uSSIDLength;i++) {
                    QString temp = ssid.ucSSID[i];
                    netname += temp;
                }
            }
            local_WlanFreeMemory(connAtts);
        }
#endif
    }
    break;
    case QNetworkInfo::EthernetMode:
    {
#if !defined( Q_CC_MINGW) && !defined( Q_OS_WINCE)
        WMIHelper *wHelper;
        wHelper = new WMIHelper(this);
        wHelper->setWmiNamespace(QLatin1String("root/cimv2"));
        wHelper->setClassName(QLatin1String("Win32_ComputerSystem"));
        wHelper->setClassProperty(QStringList() << QLatin1String("Domain"));

        QVariant v = wHelper->getWMIData();
        netname = v.toString();
#endif
    }
    break;
    case QNetworkInfo::BluetoothMode:

//    case QNetworkInfo::GsmMode:
//    case QNetworkInfo::CdmaMode:
//    case QNetworkInfo::WcdmaMode:
//    case QNetworkInfo::WimaxMode:
//    case QNetworkInfo::LteMode:
//    case QNetworkInfo::TdscdmaMode:
    default:
        break;
    };

    return netname;
}

void QNetworkInfoPrivate::connectNotify(const QMetaMethod &signal)
{
    Q_UNUSED(signal)
}

void QNetworkInfoPrivate::disconnectNotify(const QMetaMethod &signal)
{
    Q_UNUSED(signal)
}

QT_END_NAMESPACE
