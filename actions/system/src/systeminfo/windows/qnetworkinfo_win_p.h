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

#ifndef QNETWORKINFO_WIN_P_H
#define QNETWORKINFO_WIN_P_H

#include <QBasicTimer>

#include "qnetworkinfo.h"

QT_BEGIN_NAMESPACE

class QNetworkInfoPrivate : public QObject
{
    Q_OBJECT

public:
    QNetworkInfoPrivate(QNetworkInfo *parent = 0);
    ~QNetworkInfoPrivate();

    int networkInterfaceCount(QNetworkInfo::NetworkMode mode);
    int networkSignalStrength(QNetworkInfo::NetworkMode mode, int netInterface);
    QNetworkInfo::CellDataTechnology currentCellDataTechnology(int netInterface);
    QNetworkInfo::NetworkMode currentNetworkMode();
    QNetworkInfo::NetworkStatus networkStatus(QNetworkInfo::NetworkMode mode, int netInterface);
#ifndef QT_NO_NETWORKINTERFACE
    QNetworkInterface interfaceForMode(QNetworkInfo::NetworkMode mode, int netInterface);
#endif // QT_NO_NETWORKINTERFACE
    QString cellId(int netInterface);
    QString currentMobileCountryCode(int netInterface);
    QString currentMobileNetworkCode(int netInterface);
    QString homeMobileCountryCode(int netInterface);
    QString homeMobileNetworkCode(int netInterface);
    QString imsi(int netInterface);
    QString locationAreaCode(int netInterface);
    QString macAddress(QNetworkInfo::NetworkMode mode, int netInterface);
    QString networkName(QNetworkInfo::NetworkMode mode, int netInterface);

    void emitNetworkStatusChanged(QNetworkInfo::NetworkMode, QNetworkInfo::NetworkStatus);
    void emitNetworkSignalStrengthChanged(QNetworkInfo::NetworkMode,int);

    static QNetworkInfoPrivate *instance();
    static unsigned wifiInterfaceCount();

Q_SIGNALS:
    void cellIdChanged(int netInterface, const QString &id);
    void currentCellDataTechnologyChanged(int netInterface, QNetworkInfo::CellDataTechnology tech);
    void currentMobileCountryCodeChanged(int netInterface, const QString &mcc);
    void currentMobileNetworkCodeChanged(int netInterface, const QString &mnc);
    void currentNetworkModeChanged(QNetworkInfo::NetworkMode mode);
    void locationAreaCodeChanged(int netInterface, const QString &lac);
    void networkInterfaceCountChanged(QNetworkInfo::NetworkMode mode, int count);
    void networkNameChanged(QNetworkInfo::NetworkMode mode, int netInterface, const QString &name);
    void networkSignalStrengthChanged(QNetworkInfo::NetworkMode mode, int netInterface, int strength);
    void networkStatusChanged(QNetworkInfo::NetworkMode mode, int netInterface, QNetworkInfo::NetworkStatus status);

protected:
    void connectNotify(const QMetaMethod &signal);
    void disconnectNotify(const QMetaMethod &signal);
    void timerEvent(QTimerEvent *event);

private:
    QNetworkInfo * const q_ptr;
    Q_DECLARE_PUBLIC(QNetworkInfo);

    void startWifiCallback();
    bool isDefaultMode(QNetworkInfo::NetworkMode mode);

    quint32 wifiStrength;
    quint32 ethStrength;
    Qt::HANDLE hWlan;
    int timerMs;
    QBasicTimer netStrengthTimer;
    bool wlanCallbackInitialized;

private Q_SLOTS:
    void networkStrengthTimeout();
    void networkStatusTimeout();
};

QT_END_NAMESPACE


#endif // QNETWORKINFO_WIN_P_H
