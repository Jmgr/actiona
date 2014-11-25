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

#ifndef QNETWORKINFO_MAC_P_H
#define QNETWORKINFO_MAC_P_H

#include <qnetworkinfo.h>

#include <QtCore/qmap.h>
#include <QtCore/qthread.h>
#include <QtCore/qmutex.h>

#include <SystemConfiguration/SystemConfiguration.h>

QT_BEGIN_NAMESPACE

class QTimer;
class QRunLoopThread;

class QNetworkInfoPrivate : public QObject
{
    Q_OBJECT

public:
    QNetworkInfoPrivate(QNetworkInfo *parent = 0);
    ~QNetworkInfoPrivate();

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


    static QNetworkInfoPrivate *instance() {return self;}
    void wifiNetworkChanged(const QString &notification, const QString interfaceName);
    void ethernetChanged();
    QString getDefaultInterface();

public Q_SLOTS:
   void primaryInterface();

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
    static QNetworkInfoPrivate *self;
    bool hasWifi;
    QTimer *rssiTimer;
    int signalStrengthCache;

    QRunLoopThread * runloopThread;
    QString defaultInterface;
    QNetworkInfo::NetworkMode modeForInterface(QString interfaceName);
    void startNetworkChangeLoop();
    bool isInterfaceActive(const char* netInterface);
    bool networkThreadOk;

    void resetInterfaceCount();

    //    int getNetworkInterfaceCount(QNetworkInfo::NetworkMode mode);
//    int getNetworkSignalStrength(QNetworkInfo::NetworkMode mode, int interface);
//    QNetworkInfo::NetworkMode getCurrentNetworkMode();
//    QNetworkInfo::NetworkStatus getNetworkStatus(QNetworkInfo::NetworkMode mode, int interface);
//    QString getNetworkName(QNetworkInfo::NetworkMode mode, int interface);

//    bool watchCurrentNetworkMode;
//    bool watchNetworkInterfaceCount;
//    bool watchNetworkSignalStrength;
//    bool watchNetworkStatus;
//    bool watchNetworkName;
//    QNetworkInfo::NetworkMode currentMode;
    QMap<QNetworkInfo::NetworkMode, int> networkInterfaceCounts;
    QMap<QPair<QNetworkInfo::NetworkMode, int>, int> networkSignalStrengths;
    QMap<QPair<QNetworkInfo::NetworkMode, int>, QNetworkInfo::NetworkStatus> networkStatuses;
    QMap<QPair<QNetworkInfo::NetworkMode, int>, QString> networkNames;

private Q_SLOTS:
    void rssiTimeout();



};

class QRunLoopThread : public QObject
{
    Q_OBJECT

public:
    QRunLoopThread(QObject *parent = 0);
    ~QRunLoopThread();
    bool keepRunning;
    void stop();

public Q_SLOTS:
   void doWork();

protected:
   QThread t;
private:
    void startNetworkChangeLoop();
    QMutex mutex;
    SCDynamicStoreRef storeSession;
    CFRunLoopSourceRef runloopSource;

private Q_SLOTS:
};

QT_END_NAMESPACE

#endif // QNETWORKINFO_MAC_P_H
