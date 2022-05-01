/*
	Actiona
	Copyright (C) 2005 Jonathan Mercier-Ganady

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

	Contact: jmgr@jmgr.info
*/

#include "backend/positionchooser-x11.hpp"
#include "backend/x11.hpp"

#include <QApplication>
#include <QCursor>
#include <QX11Info>
#include <QMouseEvent>
#include <QDebug>

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <xcb/xcb.h>

namespace Backend
{
    PositionChooserX11::PositionChooserX11(QObject *parent):
        PositionChooser(parent),
        mTargetCursor(XCreateFontCursor(QX11Info::display(), XC_target))
	{
	}

    PositionChooserX11::~PositionChooserX11()
	{
        stopMouseCapture();

        XFreeCursor(QX11Info::display(), mTargetCursor);
    }

    void PositionChooserX11::mousePressEvent(QMouseEvent *event)
    {
        QApplication::instance()->installNativeEventFilter(this);

        auto result = XGrabPointer(QX11Info::display(), QX11Info::appRootWindow(), True, ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
                        None, mTargetCursor, CurrentTime);
        if(result != GrabSuccess)
        {
            event->ignore();
            emit errorOccurred(BackendError(QStringLiteral("failed to grab the mouse pointer: error is %1").arg(formatGrabError(result))));
        }
    }

    bool PositionChooserX11::nativeEventFilter(const QByteArray &eventType, void *message, long *)
    {
        if(eventType != "xcb_generic_event_t")
            return false;

        auto *event = static_cast<xcb_generic_event_t *>(message);

        switch(event->response_type)
        {
        case XCB_BUTTON_RELEASE:
            emit positionChosen(QCursor::pos());

            stopMouseCapture();

            return false;
        }

        return false;
    }

    void PositionChooserX11::stopMouseCapture()
    {
        XUngrabPointer(QX11Info::display(), CurrentTime);
        XFlush(QX11Info::display());

        QApplication::instance()->removeNativeEventFilter(this);
    }
}
