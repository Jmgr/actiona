/****************************************************************************
 **
 ** Copyright (C) Qxt Foundation. Some rights reserved.
 **
 ** This file is part of the QxtCore module of the Qxt library.
 **
 ** This library is free software; you can redistribute it and/or modify it
 ** under the terms of the Common Public License, version 1.0, as published
 ** by IBM, and/or under the terms of the GNU Lesser General Public License,
 ** version 2.1, as published by the Free Software Foundation.
 **
 ** This file is provided "AS IS", without WARRANTIES OR CONDITIONS OF ANY
 ** KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
 ** WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY OR
 ** FITNESS FOR A PARTICULAR PURPOSE.
 **
 ** You should have received a copy of the CPL and the LGPL along with this
 ** file. See the LICENSE file and the cpl1.0.txt/lgpl-2.1.txt files
 ** included with the source distribution for more information.
 ** If you did not receive a copy of the licenses, contact the Qxt Foundation.
 **
 ** <http://libqxt.org>  <foundation@libqxt.org>
 **
 ****************************************************************************/

/*!
\class QxtSignalWaiter

\inmodule QxtCore

\brief The QxtSignalWaiter class blocks and processes events until a signal is emitted

In many cases, writing code that assumes certain actions are synchronous is considerably simpler than breaking your function into
multiple blocks and using signals and slots to connect them all. Using this class, QSignalWaiter::wait will block until a certain
signal is emitted and then return. The return value is true if the signal was caught, or false if a user-specified timeout elapses
before catching the signal.

\code
void MyObject::myFunction() {
    QxtSignalWaiter waiter(myOtherObject, SIGNAL(longProcessFinished()));
    myOtherObject->longProcess();
    if(waiter.wait(5000)) {
        doSomething(myOtherObject->information());
    } else {
        QMessageBox::information(0, "MyObject", "Timed out while waiting on longProcessFinished()", QMessageBox::Ok);
    }
}
\endcode

\bold {Note:}
QxtSignalWaiter is not reentrant. In particular, only one QxtSignalWaiter object per thread can be safely waiting at a
time. If a second QxtSignalWaiter is used while the first is waiting, the first will not return until the second has
timed out or successfully caught its signal.
*/

#include "qxtsignalwaiter.h"
#include <QCoreApplication>
#include <QTimerEvent>

class QxtSignalWaiterPrivate : public QxtPrivate<QxtSignalWaiter>
{
public:
    QXT_DECLARE_PUBLIC(QxtSignalWaiter)

    QxtSignalWaiterPrivate()
    {
        ready = false;
        emitted = false;
        timeout = false;
        waiting = false;
    }

    bool ready, timeout, emitted, waiting;
    int timerID;

    void stopTimer()
    {
        if (timerID)
            qxt_p().killTimer(timerID);
        timerID = 0;
        waiting = false;
    }
};

/*!
 * Constructs a QxtSignalWaiter that will wait for \a signal from \a sender ie. sender::signal()
 * to be emitted. QxtSignalWaiter objects are intended to be created on the stack, therefore no
 * parent parameter is accepted.
 */
QxtSignalWaiter::QxtSignalWaiter(const QObject* sender, const char* signal) : QObject(nullptr)
{
    Q_ASSERT(sender && signal);
    QXT_INIT_PRIVATE(QxtSignalWaiter);
    connect(sender, signal, this, SLOT(signalCaught()));
}

/*!
 * This is an overloaded function provided for convenience. This version can be invoked without first instantiating
 * a QxtSignalWaiter object. Waits for \a signal from \a sender to be emitted within \a msec while processing events
 * according to \a flags. Returns \c true if the signal was caught, or \c false if the timeout elapsed.
 */
bool QxtSignalWaiter::wait(const QObject* sender, const char* signal, int msec, QEventLoop::ProcessEventsFlags flags)
{
    QxtSignalWaiter w(sender, signal);
    return w.wait(msec, flags);
}

/*!
 * Blocks the current function until sender::signal() is emitted. If msec is not -1, wait() will return before the
 * signal is emitted if the specified number of milliseconds have elapsed.
 * Returns \c true if the signal was caught, or \c false if the timeout elapsed.
 * Note that wait() may continue to block after the signal is emitted or the timeout elapses; the function only
 * guarantees that it will not return BEFORE one of these conditions has occurred. This function is not reentrant.
 */
bool QxtSignalWaiter::wait(int msec, QEventLoop::ProcessEventsFlags flags)
{
    QxtSignalWaiterPrivate& d = qxt_d();

    // Clear the emission status
    d.ready = false;
    d.emitted = false;

    // Check input parameters
    if (msec < -1 || msec == 0)
        return false;

    // activate the timeout
    if (msec != -1)
        d.timerID = startTimer(msec);
    else
        d.timerID = 0;

    // Make sure to wait for events
    flags |= QEventLoop::WaitForMoreEvents;

    // Begin waiting
    d.waiting = true;
    while (!d.ready && !d.timeout)
        QCoreApplication::processEvents(flags);

    // Clean up and return status
    qxt_d().stopTimer();
    d.emitted = d.ready;
    d.waiting = false;
    return d.ready;
}

/*!
 * Returns \c true if the desired signal was emitted during the last wait() call.
 */
bool QxtSignalWaiter::hasCapturedSignal() const
{
    return qxt_d().emitted;
}

/*!
 * Signals a waiting object to stop blocking because the desired signal was emitted.
 * QxtSignalWaiter::hasCapturedSignal() will return true after this slot is invoked.
 * Use this slot to allow QxtSignalWaiter to wait for the first of multiple signals.
 */
void QxtSignalWaiter::signalCaught()
{
    if (!qxt_d().waiting) return;
    qxt_d().ready = true;
    qxt_d().stopTimer();
}

/*!
 * \reimp
 */
void QxtSignalWaiter::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
    cancelWait();
}

/*!
 * Signals a waiting object to stop blocking because the timeout has elapsed.
 * QxtSignalWaiter::hasCapturedSignal() will return false after this slot is invoked.
 * Use this slot to allow QxtSignalWaiter to be interrupted for reasons other than
 * a timeout.
 */
void QxtSignalWaiter::cancelWait()
{
    if (!qxt_d().waiting) return;
    qxt_d().timeout = true;
    qxt_d().stopTimer();
}
