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

#include <QCoreApplication>
#include <QCursor>
#include <QX11Info>

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

    void PositionChooserX11::choose()
    {
        QCoreApplication::instance()->installNativeEventFilter(this);

        if(XGrabPointer(QX11Info::display(), QX11Info::appRootWindow(), True, ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
                        None, mTargetCursor, CurrentTime) != GrabSuccess)
            throw BackendError(QStringLiteral("failed to grab the mouse pointer"));
    }

    bool PositionChooserX11::nativeEventFilter(const QByteArray &eventType, void *message, long *)
    {
        if(eventType != "xcb_generic_event_t")
            return true;

        auto *event = static_cast<xcb_generic_event_t *>(message);

        switch(event->response_type)
        {
        case XCB_BUTTON_RELEASE:
            emit done(QCursor::pos());

            stopMouseCapture();

            return false;
        case XCB_KEY_PRESS:
        {
            auto keyPressEvent = reinterpret_cast<xcb_key_press_event_t *>(event);
            if(keyPressEvent->detail == 0x09) // Escape
            {
                emit canceled();

                stopMouseCapture();

                return false;
            }
            return true;
        }
        }

        return true;
    }

    void PositionChooserX11::stopMouseCapture()
	{
        XUngrabPointer(QX11Info::display(), CurrentTime);
        XFlush(QX11Info::display());

        QCoreApplication::instance()->removeNativeEventFilter(this);
    }
}
