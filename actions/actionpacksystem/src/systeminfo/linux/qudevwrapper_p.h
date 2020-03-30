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

#ifndef QUDEVWRAPPER_P_H
#define QUDEVWRAPPER_P_H

#include <QtCore/QObject>

#include <libudev.h>

#if !defined(QT_NO_UDEV)

QT_BEGIN_NAMESPACE

class QSocketNotifier;

class QUDevWrapper: public QObject
{
    Q_OBJECT

public:
    QUDevWrapper(QObject *parent = 0);

Q_SIGNALS:
    void driveChanged();
    void batteryDataChanged(int battery, const QByteArray &attribute, const QByteArray &value);
    void chargerTypeChanged(const QByteArray &value, bool enabled);

protected:
    void connectNotify(const QMetaMethod &signal);
    void disconnectNotify(const QMetaMethod &signal);

private:
    Q_DISABLE_COPY(QUDevWrapper)

    struct udev *udev;
    struct udev_monitor *udevMonitor;
    int udevFd;
    QSocketNotifier *notifier;
    bool watcherEnabled;
    bool watchPowerSupply;
    bool watchDrives;

    bool addUDevWatcher(const QByteArray &subsystem);
    bool removeAllUDevWatcher();

private Q_SLOTS:
    void onUDevChanges();
};

QT_END_NAMESPACE

#endif // QT_NO_UDEV

#endif /* QUDEVWRAPPER_P_H */
