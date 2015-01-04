/*
    Actiona
    Copyright (C) 2008-2015 Jonathan Mercier-Ganady

    Actiona is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Actiona is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    Contact : jmgr@jmgr.info
*/

#ifndef TARGETWINDOW_H
#define TARGETWINDOW_H

#include <QWidget>
#include <QTimer>

#include "actiontools_global.h"

#ifdef Q_OS_LINUX
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QAbstractNativeEventFilter>
#else
#include "nativeeventfilter.h"
#endif
#endif

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT TargetWindow : public QWidget
#ifdef Q_OS_LINUX
#if (QT_VERSION >= 0x050000)//BUG: Cannot use QT_VERSION_CHECK here, or the MOC will consider the condition to be true
            , public QAbstractNativeEventFilter
#else
            , public NativeEventFilter
#endif
#endif
    {
        Q_OBJECT
    public:
        TargetWindow();
        virtual ~TargetWindow();

    signals:
        void rectangleSelected(const QRect &rect);

    protected:
#ifdef Q_OS_WIN
        virtual void keyPressEvent(QKeyEvent *event);
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
#endif
        virtual void paintEvent(QPaintEvent *event);
        virtual void showEvent(QShowEvent *event);
        virtual void hideEvent(QHideEvent *event);

    private slots:
        void update();

    private:
#ifdef Q_OS_LINUX
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        bool x11EventFilter(XEvent *event);
#else
        bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif
        void ungrab();
#endif
        void mouseButtonReleased();

        QTimer mUpdateTimer;
        QPoint mMouseClickPosition;
        bool mMousePressed;
        QRect mResult;
#ifdef Q_OS_LINUX
        bool mGrabbingPointer;
        bool mGrabbingKeyboard;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        unsigned long mCrossCursor;
#endif
#endif
    };
}

#endif // TARGETWINDOW_H
