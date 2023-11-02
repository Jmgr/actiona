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

#include "actiontools/windowhandle.hpp"
#include "actiontools/crossplatform.hpp"

#include <QRegularExpression>

#ifdef Q_OS_UNIX
#include "actiontools/x11info.hpp"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include <memory>

namespace ActionTools
{
	static QList<WindowHandle> gWindowList;

#ifdef Q_OS_UNIX
    // from wmctrl 1.07 (http://tomas.styblo.name/wmctrl/) by Tomas Styblo + UBUNTU PATCH for 64bit
    // from giuspen-x-osk by Giuseppe Penone
    #define MAX_PROPERTY_VALUE_LEN 4096

    QString get_property(Display *disp, Window win, Atom xa_prop_type, const char *prop_name)
    {
        Atom           xa_prop_name;
        Atom           xa_ret_type;
        int            ret_format;
        unsigned long  ret_nitems;
        unsigned long  ret_bytes_after;
        unsigned long  tmp_size;

        xa_prop_name = XInternAtom(disp, prop_name, False);

        /* MAX_PROPERTY_VALUE_LEN / 4 explanation (XGetWindowProperty manpage):
         *
         * long_length = Specifies the length in 32-bit multiples of the
         *               data to be retrieved.
         *
         * NOTE:  see
         * http://mail.gnome.org/archives/wm-spec-list/2003-March/msg00067.html
         * In particular:
         *
         * 	When the X window system was ported to 64-bit architectures, a
         * rather peculiar design decision was made. 32-bit quantities such
         * as Window IDs, atoms, etc, were kept as longs in the client side
         * APIs, even when long was changed to 64 bits.
         *
         */
        unsigned char *property{nullptr};

        if(XGetWindowProperty(disp, win, xa_prop_name, 0, MAX_PROPERTY_VALUE_LEN / 4, False,
                              xa_prop_type, &xa_ret_type, &ret_format,
                              &ret_nitems, &ret_bytes_after, &property) != Success)
        {
            XFree(property);

            qDebug("Cannot get %s property.\n", prop_name);

            return {};
        }

        auto ret_prop = std::unique_ptr<unsigned char, void(*)(unsigned char *)>(property, [](unsigned char *pointer){ XFree(pointer); });

        if(xa_ret_type != xa_prop_type)
        {
            qDebug("Invalid type of %s property.\n", prop_name);

            return {};
        }

        /* null terminate the result to make string handling easier */
        tmp_size = (ret_format / 8) * ret_nitems;

        /* UBUNTU PATCH Correct 64 Architecture implementation of 32 bit data */
        if(ret_format==32) tmp_size *= sizeof(long)/4;

        return QString::fromLocal8Bit(reinterpret_cast<const char*>(ret_prop.get()), tmp_size);
    }

    QString get_window_title(Display *disp, Window win)
    {
        QString wm_name = get_property(disp, win, XA_STRING, "WM_NAME");
        QString net_wm_name = get_property(disp, win, XInternAtom(disp, "UTF8_STRING", False), "_NET_WM_NAME");

        if(!net_wm_name.isNull())
            return net_wm_name;
        else
            return wm_name;
    }

    QString get_window_class(Display *disp, Window win)
    {
        return get_property(disp, win, XA_STRING, "WM_CLASS");
    }
#endif

	QString WindowHandle::title() const
	{
#ifdef Q_OS_UNIX
        return get_window_title(X11Info::display(), mValue);
#endif
#ifdef Q_OS_WIN
		QString title;

        int titleLength = GetWindowTextLength(reinterpret_cast<HWND>(mValue));

		if(titleLength >= 0)
		{
			wchar_t *titleName = new wchar_t[titleLength + 1];

            GetWindowText(reinterpret_cast<HWND>(mValue), titleName, titleLength + 1);
			title = QString::fromWCharArray(titleName);

			delete[] titleName;
		}

		return title;
#endif
	}

	QString WindowHandle::classname() const
	{
#ifdef Q_OS_UNIX
        return get_window_class(X11Info::display(), mValue);
#endif
#ifdef Q_OS_WIN
		wchar_t className[255];

        GetClassName(reinterpret_cast<HWND>(mValue), className, sizeof(className)-1);

		return QString::fromWCharArray(className);
#endif
	}

	QRect WindowHandle::rect(bool useBorders) const
	{
#ifdef Q_OS_UNIX
		XWindowAttributes windowAttributes;

		if(!XGetWindowAttributes(X11Info::display(), mValue, &windowAttributes))
			return {};

		Window unused;
		int positionX, positionY;

		XTranslateCoordinates(X11Info::display(), mValue, windowAttributes.root, -windowAttributes.border_width, -windowAttributes.border_width,
							  &positionX, &positionY, &unused);

		if(useBorders)
			return QRect(positionX, positionY, windowAttributes.width + windowAttributes.border_width, windowAttributes.height + windowAttributes.border_width);
		else
			return QRect(positionX, positionY, windowAttributes.width, windowAttributes.height);
#endif
#ifdef Q_OS_WIN
		RECT rc;
		QRect rect;

		if(useBorders)
		{
            if(GetWindowRect(reinterpret_cast<HWND>(mValue), &rc))
			{
				rect.setTop(rc.top);
				rect.setBottom(rc.bottom);
				rect.setLeft(rc.left);
				rect.setRight(rc.right);
			}
		}
		else
		{
            if(GetClientRect(reinterpret_cast<HWND>(mValue), &rc))
			{
				rect.setTop(rc.top);
				rect.setBottom(rc.bottom);
				rect.setLeft(rc.left);
				rect.setRight(rc.right);
			}
		}

		return rect;
#endif
	}

	int WindowHandle::processId() const
	{
#ifdef Q_OS_UNIX
		static Atom atomPid = None;
		if(atomPid == None)
			atomPid = XInternAtom(X11Info::display(), "_NET_WM_PID", True);

		if(atomPid == None)
			return -1;

		Atom type;
		int format;
		unsigned long items;
		unsigned long bytesAfter;
		unsigned char *propPID = nullptr;
		int back = -1;

		if(XGetWindowProperty(X11Info::display(), mValue, atomPid, 0, 1, False, XA_CARDINAL,
							  &type, &format, &items, &bytesAfter, &propPID) == Success)
		{
			if(propPID)
			{
				back = *(reinterpret_cast<unsigned long *>(propPID));

				XFree(propPID);
			}
		}

		return back;
#endif
#ifdef Q_OS_WIN
		DWORD procID;

        GetWindowThreadProcessId(reinterpret_cast<HWND>(mValue), &procID);

		return procID;
#endif
	}

	bool WindowHandle::close() const
	{
#ifdef Q_OS_UNIX
		return XDestroyWindow(X11Info::display(), mValue);
#endif
#ifdef Q_OS_WIN
        return SendNotifyMessage(reinterpret_cast<HWND>(mValue), WM_CLOSE, 0, 0);
#endif
	}

	bool WindowHandle::killCreator() const
	{
#ifdef Q_OS_UNIX
		return XKillClient(X11Info::display(), mValue);
#endif
#ifdef Q_OS_WIN
		int id = processId();

		return CrossPlatform::killProcess(id);
#endif
	}

	bool WindowHandle::setForeground() const
	{
#ifdef Q_OS_UNIX
		static Atom atomActiveWindow = None;
		if(atomActiveWindow == None)
			atomActiveWindow = XInternAtom(X11Info::display(), "_NET_ACTIVE_WINDOW", False);

		if(atomActiveWindow == None)
			return false;

		XEvent event;
		memset(&event, 0, sizeof(event));
		event.type = ClientMessage;
		event.xclient.display = X11Info::display();
		event.xclient.window = mValue;
		event.xclient.message_type = atomActiveWindow;
		event.xclient.format = 32;
		event.xclient.data.l[0] = 1;//Message from a normal application
		event.xclient.data.l[1] = CurrentTime;

		XWindowAttributes windowAttributes;
		if(!XGetWindowAttributes(X11Info::display(), mValue, &windowAttributes))
			return false;

		return XSendEvent(X11Info::display(), windowAttributes.screen->root, False, SubstructureNotifyMask | SubstructureRedirectMask, &event);
#endif
#ifdef Q_OS_WIN
        if(IsIconic(reinterpret_cast<HWND>(mValue)))
            ShowWindow(reinterpret_cast<HWND>(mValue), SW_RESTORE);
		else
		{
            if(!SetForegroundWindow(reinterpret_cast<HWND>(mValue)))
				return false;

            if(!SetWindowPos(reinterpret_cast<HWND>(mValue), HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
				return false;
		}

		return true;
#endif
	}

	bool WindowHandle::minimize() const
	{
#ifdef Q_OS_UNIX
		return XIconifyWindow(X11Info::display(), mValue, DefaultScreen(X11Info::display()));
#endif
#ifdef Q_OS_WIN
        return (!SendMessage(reinterpret_cast<HWND>(mValue), WM_SYSCOMMAND, SC_MINIMIZE, 0));
#endif
	}

	bool WindowHandle::maximize() const
	{
#ifdef Q_OS_UNIX
		static Atom atomState = None;
		if(atomState == None)
			atomState = XInternAtom(X11Info::display(), "_NET_WM_STATE", False);
		static Atom atomMaximizeVert = None;
		if(atomMaximizeVert == None)
			atomMaximizeVert = XInternAtom(X11Info::display(), "_NET_WM_STATE_MAXIMIZED_VERT", False);
		static Atom atomMaximizeHorz = None;
		if(atomMaximizeHorz == None)
			atomMaximizeHorz = XInternAtom(X11Info::display(), "_NET_WM_STATE_MAXIMIZED_HORZ", False);

		if(atomState == None || atomMaximizeVert == None || atomMaximizeHorz == None)
			return false;

		XEvent event;
		memset(&event, 0, sizeof(event));
		event.type = ClientMessage;
		event.xclient.display = X11Info::display();
		event.xclient.window = mValue;
		event.xclient.message_type = atomState;
		event.xclient.format = 32;
		event.xclient.data.l[0] = 1;//_NET_WM_STATE_ADD
		event.xclient.data.l[1] = atomMaximizeVert;
		event.xclient.data.l[2] = atomMaximizeHorz;
		event.xclient.data.l[3] = 1;//Message from a normal application

		XWindowAttributes windowAttributes;
		if(!XGetWindowAttributes(X11Info::display(), mValue, &windowAttributes))
			return false;

		return XSendEvent(X11Info::display(), windowAttributes.screen->root, False, SubstructureNotifyMask | SubstructureRedirectMask, &event);
#endif
#ifdef Q_OS_WIN
        return (!SendMessage(reinterpret_cast<HWND>(mValue), WM_SYSCOMMAND, SC_MAXIMIZE, 0));
#endif
	}

	bool WindowHandle::move(QPoint position) const
	{
#ifdef Q_OS_UNIX
		return XMoveWindow(X11Info::display(), mValue, position.x(), position.y());
#endif
#ifdef Q_OS_WIN
        return SetWindowPos(reinterpret_cast<HWND>(mValue), 0, position.x(), position.y(), 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
#endif
	}

	bool WindowHandle::resize(QSize size, bool useBorders) const
	{
#ifdef Q_OS_UNIX
		if(useBorders)
		{
			XWindowAttributes windowAttributes;

			if(!XGetWindowAttributes(X11Info::display(), mValue, &windowAttributes))
				return false;

			size.rwidth() += windowAttributes.border_width;
			size.rheight() += windowAttributes.border_width;
		}

		return XResizeWindow(X11Info::display(), mValue, size.width(), size.height());
#endif
#ifdef Q_OS_WIN
		if(!useBorders)
		{
			const QRect &sizeWithBorders = rect(true);
			const QRect &sizeWithoutBorders = rect(false);

			int borderWidth = sizeWithBorders.width() - sizeWithoutBorders.width();
			int borderHeight = sizeWithBorders.height() - sizeWithoutBorders.height();

			if(borderWidth < 0)
				borderWidth = 0;
			if(borderHeight < 0)
				borderHeight = 0;

			size.rwidth() += borderWidth;
			size.rheight() += borderHeight;
		}

        return SetWindowPos(reinterpret_cast<HWND>(mValue), 0, 0, 0, size.width(), size.height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
#endif
	}

	bool WindowHandle::isActive() const
	{
		return (foregroundWindow() == *this);
	}

	WindowHandle WindowHandle::foregroundWindow()
	{
#ifdef Q_OS_UNIX
		Window focus;
		int revert = 0;

		XGetInputFocus(X11Info::display(), &focus, &revert);

		return focus;
#endif
#ifdef Q_OS_WIN
        return reinterpret_cast<WId>(GetForegroundWindow());
#endif
	}

#ifdef Q_OS_WIN
	BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
	{
		Q_UNUSED(lParam)

		if(IsWindowVisible(hwnd))
            gWindowList.append(reinterpret_cast<WId>(hwnd));

		return true;
	}
#endif

	QList<WindowHandle> WindowHandle::windowList()
	{
		gWindowList.clear();

#ifdef Q_OS_UNIX
		static Atom net_clients = None;
		if(!net_clients)
            net_clients = XInternAtom(X11Info::display(), "_NET_CLIENT_LIST_STACKING", True);

		int count = 0;
		Window* list = nullptr;
		Atom type = 0;
		int format = 0;
		unsigned long after = 0;
        XGetWindowProperty(X11Info::display(), X11Info::appRootWindow(), net_clients, 0, 256 * sizeof(Window), False, AnyPropertyType,
						   &type, &format, reinterpret_cast<unsigned long*>(&count), &after, reinterpret_cast<unsigned char**>(&list));

		for (int i = 0; i < count; ++i)
			gWindowList.append(list[i]);

		XFree(list);
#endif
#ifdef Q_OS_WIN
		HDESK hdesk = OpenInputDesktop(0, false, DESKTOP_READOBJECTS);

		EnumDesktopWindows(hdesk, EnumWindowsProc, 0);

		CloseDesktop(hdesk);
#endif

        return gWindowList;
    }

    QStringList WindowHandle::windowTitles()
    {
        QStringList windowTitles;

        const auto windowHandles = windowList();
        for(const ActionTools::WindowHandle &windowHandle: windowHandles)
        {
            QString title = windowHandle.title();
            if(title.isEmpty())
                continue;

            windowTitles << title;
        }

        windowTitles.sort();

        return windowTitles;
    }

	WindowHandle WindowHandle::findWindow(const QString &title)
	{
        const auto windowHandles = windowList();
        for(const WindowHandle &windowHandle: windowHandles)
		{
			if(windowHandle.title() == title)
				return windowHandle;
		}

		return {};
	}

	WindowHandle WindowHandle::findWindow(const QRegularExpression &regExp)
	{
		if(!regExp.isValid())
			return WindowHandle();

        const auto windowHandles = windowList();
        for(const WindowHandle &windowHandle: windowHandles)
		{
            if(regExp.match(windowHandle.title()).hasMatch())
				return windowHandle;
		}

		return {};
	}

	QList<WindowHandle> WindowHandle::findWindows(const QString &title)
	{
		QList<WindowHandle> back;

        const auto windowHandles = windowList();
        for(const WindowHandle &windowHandle: windowHandles)
		{
			if(windowHandle.title() == title)
				back.append(windowHandle);
		}

		return back;
	}

	QList<WindowHandle> WindowHandle::findWindows(const QRegularExpression &regExp)
	{
		QList<WindowHandle> back;

        const auto windowHandles = windowList();
        for(const WindowHandle &windowHandle: windowHandles)
		{
            if(regExp.match(windowHandle.title()).hasMatch())
				back.append(windowHandle);
		}

		return back;
	}
}
