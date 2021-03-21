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

#include "backend/windowchooser-x11.hpp"
#include "backend/windowhandle.hpp"

#include <QApplication>

#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <xcb/xcb.h>

namespace Backend
{
    WindowChooserX11::WindowChooserX11(QObject *parent):
        WindowChooser(parent),
        mTargetCursor(XCreateFontCursor(QX11Info::display(), XC_target))
	{
	}

    WindowChooserX11::~WindowChooserX11()
	{
        stopMouseCapture();

        XFreeCursor(QX11Info::display(), mTargetCursor);
	}

    void WindowChooserX11::choose()
    {
        QCoreApplication::instance()->installNativeEventFilter(this);

        mSearching = true;

        if(XGrabPointer(QX11Info::display(), QX11Info::appRootWindow(), True, ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
            None, mTargetCursor, CurrentTime) != GrabSuccess)
            emit canceled();// TODO: Error
    }

    void WindowChooserX11::stopMouseCapture()
	{
		if(!mSearching)
			return;

		mSearching = false;;

		XUngrabPointer(QX11Info::display(), CurrentTime);
        XFlush(QX11Info::display());

        QCoreApplication::instance()->removeNativeEventFilter(this);
    }

    WId windowAtPointer()
	{
        Window window = QX11Info::appRootWindow();
		Window back = None;

		while(window)
		{
			Window root, child;
			int x, y, rx, ry;
			unsigned int mask;

			back = window;

			XQueryPointer(QX11Info::display(), window, &root, &child, &rx, &ry, &x, &y, &mask);

			window = child;
		}

		return back;
	}

    bool isWindowValid(const WindowHandle &handle)    {
        if(!handle.isValid())
            return false;

        const auto widgets = QApplication::allWidgets();
        for(QWidget *widget: widgets)
        {
            if(widget->winId() == handle.value())
                return false;
        }

        return true;
    }

    bool WindowChooserX11::nativeEventFilter(const QByteArray &eventType, void *message, long *)
    {
        if(eventType != "xcb_generic_event_t")
            return true;

        auto* event = static_cast<xcb_generic_event_t *>(message);

        switch(event->response_type)
        {
        case XCB_BUTTON_RELEASE:
        {
            Window window = windowAtPointer();
            if(window == None || !isWindowValid(window))
            {
                emit canceled();

                return false;
            }

            stopMouseCapture();

            emit done(window);

            return false;
        }
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
}
