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
#include "backend/x11.hpp"

#include <QApplication>
#include <QCursor>
#include <QX11Info>
#include <QMouseEvent>
#include <QWidget>
#include <QDebug>

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

    void WindowChooserX11::mousePressEvent(QMouseEvent *event)
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

    bool WindowChooserX11::nativeEventFilter(const QByteArray &eventType, void *message, long *)
    {
        if(eventType != "xcb_generic_event_t")
            return false;

        auto *event = static_cast<xcb_generic_event_t *>(message);

        switch(event->response_type)
        {
        case XCB_BUTTON_RELEASE:
        {
            auto window = windowAtPointer();
            if(window != None && isWindowValid(window))
                emit windowChosen(window);

            stopMouseCapture();

            return false;
        }
        }

        return false;
    }

    void WindowChooserX11::stopMouseCapture()
    {
        XUngrabPointer(QX11Info::display(), CurrentTime);
        XFlush(QX11Info::display());

        QApplication::instance()->removeNativeEventFilter(this);
    }

    bool WindowChooserX11::isWindowValid(WId windowId) const
    {
        const auto widgets = QApplication::allWidgets();
        for(auto *widget: widgets)
        {
            if(widget->winId() == windowId)
                return false;
        }

        return true;
    }
}
