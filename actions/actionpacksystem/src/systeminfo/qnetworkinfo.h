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

#ifndef QNETWORKINFO_H
#define QNETWORKINFO_H

#include "qsysteminfoglobal.h"
#include <QtCore/qobject.h>
#include <QtNetwork/qnetworkinterface.h>

QT_BEGIN_NAMESPACE

#if !defined(QT_SIMULATOR)
class QNetworkInfoPrivate;
#else
class QNetworkInfoSimulator;
#endif // QT_SIMULATOR

class Q_SYSTEMINFO_EXPORT QNetworkInfo : public QObject
{
    Q_OBJECT

public:
    enum CellDataTechnology {
        UnknownDataTechnology = 0,
        GprsDataTechnology,
        EdgeDataTechnology,
        UmtsDataTechnology,
        HspaDataTechnology
    };

    enum NetworkMode {
        UnknownMode = 0,
        GsmMode,
        CdmaMode,
        WcdmaMode,
        WlanMode,
        EthernetMode,
        BluetoothMode,
        WimaxMode,
        LteMode,
        TdscdmaMode
    };

    enum NetworkStatus {
        UnknownStatus = 0,
        NoNetworkAvailable,
        EmergencyOnly,
        Searching,
        Busy,
        Denied,
        HomeNetwork,
        Roaming
        // ,Connected //desktop
    };

    QNetworkInfo(QObject *parent = nullptr);
    ~QNetworkInfo() override;

    int networkInterfaceCount(QNetworkInfo::NetworkMode mode) const;
    int networkSignalStrength(QNetworkInfo::NetworkMode mode, int interface) const;
    QNetworkInfo::CellDataTechnology currentCellDataTechnology(int interface) const;
    QNetworkInfo::NetworkMode currentNetworkMode() const;
    QNetworkInfo::NetworkStatus networkStatus(QNetworkInfo::NetworkMode mode, int interface) const;
#ifndef QT_NO_NETWORKINTERFACE
    QNetworkInterface interfaceForMode(QNetworkInfo::NetworkMode mode, int interface) const;
#endif // QT_NO_NETWORKINTERFACE
    QString cellId(int interface) const;
    QString currentMobileCountryCode(int interface) const;
    QString currentMobileNetworkCode(int interface) const;
    QString homeMobileCountryCode(int interface) const;
    QString homeMobileNetworkCode(int interface) const;
    QString imsi(int interface) const;
    QString locationAreaCode(int interface) const;
    QString macAddress(QNetworkInfo::NetworkMode mode, int interface) const;
    QString networkName(QNetworkInfo::NetworkMode mode, int interface) const;

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
    void connectNotify(const QMetaMethod &signal) override;
    void disconnectNotify(const QMetaMethod &signal) override;

private:
    Q_DISABLE_COPY(QNetworkInfo)
#if !defined(QT_SIMULATOR)
    QNetworkInfoPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QNetworkInfo)
#else
    QNetworkInfoSimulator * const d_ptr;
#endif // QT_SIMULATOR
};

QT_END_NAMESPACE

#endif // QNETWORKINFO_H
