/*
    Actionaz
    Copyright (C) 2008-2013 Jonathan Mercier-Ganady

    Actionaz is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Actionaz is distributed in the hope that it will be useful,
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
#include "nativeeventfilter.h"

namespace ActionTools
{
    class ACTIONTOOLSSHARED_EXPORT TargetWindow : public QWidget, public NativeEventFilter
    {
        Q_OBJECT
    public:
        TargetWindow();
        virtual ~TargetWindow();

    signals:
        void rectangleSelected(const QRect &rect);

    protected:
#ifdef Q_WS_WIN
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
#ifdef Q_WS_X11
        virtual bool x11EventFilter(XEvent *event);
        void ungrab();
#endif
        void mouseButtonReleased();

        QTimer mUpdateTimer;
        QPoint mMouseClickPosition;
        bool mMousePressed;
        QRect mResult;
#ifdef Q_WS_X11
        bool mGrabbingPointer;
        bool mGrabbingKeyboard;
#endif
    };
}

#endif // TARGETWINDOW_H
