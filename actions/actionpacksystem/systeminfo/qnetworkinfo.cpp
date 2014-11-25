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

#include "qnetworkinfo.h"

#if defined(QT_SIMULATOR)
#  include "simulator/qsysteminfo_simulator_p.h"
#elif defined(Q_OS_LINUX)
#  include "linux/qnetworkinfo_linux_p.h"
#elif defined(Q_OS_WIN)
#  include "windows/qnetworkinfo_win_p.h"
#elif defined(Q_OS_MAC)
#  include "mac/qnetworkinfo_mac_p.h"
#else
QT_BEGIN_NAMESPACE
class QNetworkInfoPrivate
{
public:
    QNetworkInfoPrivate(QNetworkInfo *) {}

    int networkInterfaceCount(QNetworkInfo::NetworkMode) { return -1; }
    int networkSignalStrength(QNetworkInfo::NetworkMode, int) { return -1; }
    QNetworkInfo::CellDataTechnology currentCellDataTechnology(int) { return QNetworkInfo::UnknownDataTechnology; }
    QNetworkInfo::NetworkMode currentNetworkMode() { return QNetworkInfo::UnknownMode; }
    QNetworkInfo::NetworkStatus networkStatus(QNetworkInfo::NetworkMode, int) { return QNetworkInfo::UnknownStatus; }
#ifndef QT_NO_NETWORKINTERFACE
    QNetworkInterface interfaceForMode(QNetworkInfo::NetworkMode, int) { return QNetworkInterface(); }
#endif // QT_NO_NETWORKINTERFACE
    QString cellId(int) { return QString(); }
    QString currentMobileCountryCode(int) { return QString(); }
    QString currentMobileNetworkCode(int) { return QString(); }
    QString homeMobileCountryCode(int) { return QString(); }
    QString homeMobileNetworkCode(int) { return QString(); }
    QString imsi(int) { return QString(); }
    QString locationAreaCode(int) { return QString(); }
    QString macAddress(QNetworkInfo::NetworkMode, int) { return QString(); }
    QString networkName(QNetworkInfo::NetworkMode, int) { return QString(); }
};
QT_END_NAMESPACE
#endif

#include <QtCore/qmetaobject.h>

QT_BEGIN_NAMESPACE

/*!
    \class QNetworkInfo
    \inmodule QtSystemInfo
    \brief The QNetworkInfo class provides various information about the network status.

    \ingroup systeminfo

    To support the cases where one has multiple interfaces / modems for the same network mode, you
    can specify which interface you refer to. For those cases, the 'interface' parameter is the index
    of the interface, starting from 0.
*/

/*!
    \enum QNetworkInfo::CellDataTechnology
    This enum describes the type of cellular technology.

    \value UnknownDataTechnology   The cellular technology is unknown or an error occured.
    \value GprsDataTechnology      General Packet Radio Service (GPRS) data service.
    \value EdgeDataTechnology      Enhanced Data Rates for GSM Evolution (EDGE) data service.
    \value UmtsDataTechnology      Universal Mobile Telecommunications System (UMTS) data service.
    \value HspaDataTechnology      High Speed Packet Access (HSPA) data service.
*/

/*!
    \enum QNetworkInfo::NetworkMode
    This enumeration describes the type of the network.

    \value UnknownMode     The network is unknown or an error occured.
    \value GsmMode         Global System for Mobile (GSM) network.
    \value CdmaMode        Code Division Multiple Access (CDMA) network.
    \value WcdmaMode       Wideband Code Division Multiple Access (WCDMA) network.
    \value WlanMode        Wireless local area network (WLAN) network.
    \value EthernetMode    Local area network (LAN), or Ethernet network.
    \value BluetoothMode   Bluetooth network.
    \value WimaxMode       Worldwide Interoperability for Microwave Access (WiMAX) network.
    \value LteMode         3GPP Long Term Evolution (LTE) network.
    \value TdscdmaMode     Time Division Synchronous Code Division Multiple Access (TD-SCDMA) network.
*/

/*!
    \enum QNetworkInfo::NetworkStatus
    This enumeration describes the status of the network.

    \value UnknownStatus        The status is unknown or an error occured.
    \value NoNetworkAvailable   There is no network available.
    \value EmergencyOnly        The network only allows emergency calls.
    \value Searching            The device is searching or connecting to the network.
    \value Busy                 The network is too busy to be connected.
    \value Denied               The connection to the network has been denied.
    \value HomeNetwork          The device is connected to the home network.
    \value Roaming              The device is connected to some roaming network.
*/

/*!
    \fn void QNetworkInfo::cellIdChanged(int interface, const QString &id)

    This signal is emitted whenever the cell ID for \a interface has changed to \a id.
*/

/*!
    \fn void QNetworkInfo::currentCellDataTechnologyChanged(int interface, QNetworkInfo::CellDataTechnology tech)

    This signal is emitted whenever the current cell data technology for \a interface has changed to \a tech.
*/

/*!
    \fn void QNetworkInfo::currentMobileCountryCodeChanged(int interface, const QString &mcc)

    This signal is emitted whenever the current Mobile Country Code (MCC) for \a interface has changed
    to \a mcc.
*/

/*!
    \fn void QNetworkInfo::currentMobileNetworkCodeChanged(int interface, const QString &mnc)

    This signal is emitted whenever the current Mobile Network Code (MNC) for \a interface has changed
    to \a mnc.
*/

/*!
    \fn void QNetworkInfo::currentNetworkModeChanged(QNetworkInfo::NetworkMode mode)

    This signal is emitted whenever the current network has changed to \a mode.
*/

/*!
    \fn void QNetworkInfo::locationAreaCodeChanged(int interface, const QString &lac)

    This signal is emitted whenever the location area code for \a interface has changed to \a lac.
*/

/*!
    \fn void QNetworkInfo::networkInterfaceCountChanged(QNetworkInfo::NetworkMode mode, int count)

    This signal is emitted whenever the number of interfaces for the \a mode has changed to \a count.
*/

/*!
    \fn void QNetworkInfo::networkNameChanged(QNetworkInfo::NetworkMode mode, int interface, const QString &name)

    This signal is emitted whenever the name for the \a interface of \a mode has changed to \a name.
*/

/*!
    \fn void QNetworkInfo::networkSignalStrengthChanged(QNetworkInfo::NetworkMode mode, int interface, int strength)

    This signal is emitted whenever the signal strength for the \a interface of \a mode has changed
    to \a strength.
*/

/*!
    \fn void QNetworkInfo::networkStatusChanged(QNetworkInfo::NetworkMode mode, int interface, QNetworkInfo::NetworkStatus status)

    This signal is emitted whenever the status for the \a interface of \a mode has changed to \a status.
*/

/*!
    Constructs a QNetworkInfo object with the given \a parent.
*/
QNetworkInfo::QNetworkInfo(QObject *parent)
    : QObject(parent)
#if !defined(QT_SIMULATOR)
    , d_ptr(new QNetworkInfoPrivate(this))
#else
    , d_ptr(new QNetworkInfoSimulator(this))
#endif // QT_SIMULATOR
{
}

/*!
    Destroys the object
*/
QNetworkInfo::~QNetworkInfo()
{
    delete d_ptr;
}

/*!
    Returns the number of interfaces for the \a mode. If the information is not available, or error
    occurs, -1 is returned.
*/
int QNetworkInfo::networkInterfaceCount(QNetworkInfo::NetworkMode mode) const
{
    return d_ptr->networkInterfaceCount(mode);
}

/*!
    Returns the signal strength for \a interfaceDevice of \a mode, in 0 - 100 scale. If the information
    is not available, or error occurs, -1 is returned.
*/
int QNetworkInfo::networkSignalStrength(QNetworkInfo::NetworkMode mode, int interfaceDevice) const
{
    return d_ptr->networkSignalStrength(mode, interfaceDevice);
}

/*!
    Returns the current cell data technology used for \a interfaceDevice.
*/
QNetworkInfo::CellDataTechnology QNetworkInfo::currentCellDataTechnology(int interfaceDevice) const
{
    return d_ptr->currentCellDataTechnology(interfaceDevice);
}

/*!
    Returns the current active network mode. If there are more than one modes activated, the preferred
    one is returned.
*/
QNetworkInfo::NetworkMode QNetworkInfo::currentNetworkMode() const
{
    return d_ptr->currentNetworkMode();
}

/*!
    Returns the current status for \a interfaceDevice of \a mode.
*/
QNetworkInfo::NetworkStatus QNetworkInfo::networkStatus(QNetworkInfo::NetworkMode mode, int interfaceDevice) const
{
    return d_ptr->networkStatus(mode, interfaceDevice);
}

#ifndef QT_NO_NETWORKINTERFACE
/*!
    Returns the first found interface for \a interfaceDevice of \a mode. If none is found, or it can't be
    represented by QNetworkInterface (e.g. Bluetooth), and empty object is returned.
*/
QNetworkInterface QNetworkInfo::interfaceForMode(QNetworkInfo::NetworkMode mode, int interfaceDevice) const
{
    return d_ptr->interfaceForMode(mode, interfaceDevice);
}
#endif // QT_NO_NETWORKINTERFACE

/*!
    Returns the cell ID of the connected tower or based station for \a interfaceDevice. If this information
    is not available or error occurs, an empty string is returned.
*/
QString QNetworkInfo::cellId(int interfaceDevice) const
{
    return d_ptr->cellId(interfaceDevice);
}

/*!
    Returns the current Mobile Country Code (MCC) for \a interfaceDevice. An empty string is returned if the
    information is not available or an error occurs.
*/
QString QNetworkInfo::currentMobileCountryCode(int interfaceDevice) const
{
    return d_ptr->currentMobileCountryCode(interfaceDevice);
}

/*!
    Returns the current Mobile Network Code (MNC) for \a interfaceDevice. An empty string is returned if the
    information is not available or an error occurs.
*/
QString QNetworkInfo::currentMobileNetworkCode(int interfaceDevice) const
{
    return d_ptr->currentMobileNetworkCode(interfaceDevice);
}

/*!
    Returns the home Mobile Country Code (MCC) for \a interfaceDevice. An empty string is returned if the
    information is not available or an error occurs.
*/
QString QNetworkInfo::homeMobileCountryCode(int interfaceDevice) const
{
    return d_ptr->homeMobileCountryCode(interfaceDevice);
}

/*!
    Returns the home Mobile Network Code (MNC) for \a interfaceDevice. An empty string is returned if the
    information is not available or an error occurs.
*/
QString QNetworkInfo::homeMobileNetworkCode(int interfaceDevice) const
{
    return d_ptr->homeMobileNetworkCode(interfaceDevice);
}

/*!
    Returns the International Mobile Subscriber Identity (IMSI) for \a interfaceDevice. If this information is
    not available, or error occurs, an empty string is returned.
*/
QString QNetworkInfo::imsi(int interfaceDevice) const
{
    return d_ptr->imsi(interfaceDevice);
}

/*!
    Returns the location area code of the current cellular radio network for \a interfaceDevice. If this information
    is not available or error occurs, an empty string is returned.
*/
QString QNetworkInfo::locationAreaCode(int interfaceDevice) const
{
    return d_ptr->locationAreaCode(interfaceDevice);
}

/*!
    Returns the MAC address for \a interfaceDevice of \a mode. If the MAC address is not available or error
    occurs, an empty string is returned.
*/
QString QNetworkInfo::macAddress(QNetworkInfo::NetworkMode mode, int interfaceDevice) const
{
    return d_ptr->macAddress(mode, interfaceDevice);
}

/*!
    Returns the name of the operator for \a interfaceDevice of \a mode. If the information is not available,
    or an error occurs, an empty string is returned.

    In case of WLAN, the SSID is returned; for Ethernet, the domain name is returned if available.
*/
QString QNetworkInfo::networkName(QNetworkInfo::NetworkMode mode, int interfaceDevice) const
{
    return d_ptr->networkName(mode, interfaceDevice);
}

extern QMetaMethod proxyToSourceSignal(const QMetaMethod &, QObject *);

/*!
    \internal
*/
void QNetworkInfo::connectNotify(const QMetaMethod &signal)
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN) || defined(QT_SIMULATOR) || defined(Q_OS_MAC)
    QMetaMethod sourceSignal = proxyToSourceSignal(signal, d_ptr);
    connect(d_ptr, sourceSignal, this, signal, Qt::UniqueConnection);
#else
    Q_UNUSED(signal)
#endif
}

/*!
    \internal
*/
void QNetworkInfo::disconnectNotify(const QMetaMethod &signal)
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN) || defined(QT_SIMULATOR) || defined(Q_OS_MAC)
    // We can only disconnect with the private implementation, when there is no receivers for the signal.
    if (isSignalConnected(signal))
        return;

    QMetaMethod sourceSignal = proxyToSourceSignal(signal, d_ptr);
    disconnect(d_ptr, sourceSignal, this, signal);
#else
    Q_UNUSED(signal)
#endif
}

QT_END_NAMESPACE
