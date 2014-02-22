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

#include "windowhandle.h"
#include "crossplatform.h"

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#endif

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

namespace ActionTools
{
	static QList<WindowHandle> gWindowList;

	QString WindowHandle::title() const
	{
#ifdef Q_WS_X11
		QString name;
		char *str = 0;

		if(XFetchName(QX11Info::display(), mValue, &str))
			name = QString::fromLatin1(str);

		XFree(str);
		return name;
#endif
#ifdef Q_WS_WIN
		QString title;

		int titleLength = GetWindowTextLength(mValue);

		if(titleLength >= 0)
		{
			wchar_t *titleName = new wchar_t[titleLength + 1];

			titleLength = GetWindowText(mValue, titleName, titleLength + 1);
			title = QString::fromWCharArray(titleName);

			delete[] titleName;
		}

		return title;
#endif
	}

	QString WindowHandle::classname() const
	{
#ifdef Q_WS_X11
		XClassHint *hint = XAllocClassHint();
		QString back;

		if(XGetClassHint(QX11Info::display(), mValue, hint))
			back = QString::fromLatin1(hint->res_class);

		XFree(hint);

		return back;
#endif
#ifdef Q_WS_WIN
		wchar_t className[255];

		GetClassName(mValue, className, sizeof(className)-1);

		return QString::fromWCharArray(className);
#endif
	}

	QRect WindowHandle::rect(bool useBorders) const
	{
#ifdef Q_WS_X11
		XWindowAttributes windowAttributes;

		if(!XGetWindowAttributes(QX11Info::display(), mValue, &windowAttributes))
			return QRect();

		Window unused;
		int positionX, positionY;

		XTranslateCoordinates(QX11Info::display(), mValue, windowAttributes.root, -windowAttributes.border_width, -windowAttributes.border_width,
							  &positionX, &positionY, &unused);

		if(useBorders)
			return QRect(positionX, positionY, windowAttributes.width + windowAttributes.border_width, windowAttributes.height + windowAttributes.border_width);
		else
			return QRect(positionX, positionY, windowAttributes.width, windowAttributes.height);
#endif
#ifdef Q_WS_WIN
		RECT rc;
		QRect rect;

		if(useBorders)
		{
			if(GetWindowRect(mValue, &rc))
			{
				rect.setTop(rc.top);
				rect.setBottom(rc.bottom);
				rect.setLeft(rc.left);
				rect.setRight(rc.right);
			}
		}
		else
		{
			if(GetClientRect(mValue, &rc))
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
#ifdef Q_WS_X11
		static Atom atomPid = None;
		if(atomPid == None)
			atomPid = XInternAtom(QX11Info::display(), "_NET_WM_PID", True);

		if(atomPid == None)
			return -1;

		Atom type;
		int format;
		unsigned long items;
		unsigned long bytesAfter;
		unsigned char *propPID = 0;
		int back = -1;

		if(XGetWindowProperty(QX11Info::display(), mValue, atomPid, 0, 1, False, XA_CARDINAL,
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
#ifdef Q_WS_WIN
		DWORD procID;

		GetWindowThreadProcessId(mValue, &procID);

		return procID;
#endif
	}

	bool WindowHandle::close() const
	{
#ifdef Q_WS_X11
		return XDestroyWindow(QX11Info::display(), mValue);
#endif
#ifdef Q_WS_WIN
		return SendNotifyMessage(mValue, WM_CLOSE, 0, 0);
#endif
	}

	bool WindowHandle::killCreator() const
	{
#ifdef Q_WS_X11
		return XKillClient(QX11Info::display(), mValue);
#endif
#ifdef Q_WS_WIN
		int id = processId();

		return CrossPlatform::killProcess(id);
#endif
	}

	bool WindowHandle::setForeground() const
	{
#ifdef Q_WS_X11
		static Atom atomActiveWindow = None;
		if(atomActiveWindow == None)
			atomActiveWindow = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", False);

		if(atomActiveWindow == None)
			return false;

		XEvent event;
		memset(&event, 0, sizeof(event));
		event.type = ClientMessage;
		event.xclient.display = QX11Info::display();
		event.xclient.window = mValue;
		event.xclient.message_type = atomActiveWindow;
		event.xclient.format = 32;
		event.xclient.data.l[0] = 1;//Message from a normal application
		event.xclient.data.l[1] = CurrentTime;

		XWindowAttributes windowAttributes;
		if(!XGetWindowAttributes(QX11Info::display(), mValue, &windowAttributes))
			return false;

		return XSendEvent(QX11Info::display(), windowAttributes.screen->root, False, SubstructureNotifyMask | SubstructureRedirectMask, &event);
#endif
#ifdef Q_WS_WIN
		if(IsIconic(mValue))
			ShowWindow(mValue, SW_RESTORE);
		else
		{
			if(!SetForegroundWindow(mValue))
				return false;

			if(!SetWindowPos(mValue, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
				return false;
		}

		return true;
#endif
	}

	bool WindowHandle::minimize() const
	{
#ifdef Q_WS_X11
		return XIconifyWindow(QX11Info::display(), mValue, DefaultScreen(QX11Info::display()));
#endif
#ifdef Q_WS_WIN
		return (!SendMessage(mValue, WM_SYSCOMMAND, SC_MINIMIZE, 0));
#endif
	}

	bool WindowHandle::maximize() const
	{
#ifdef Q_WS_X11
		static Atom atomState = None;
		if(atomState == None)
			atomState = XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
		static Atom atomMaximizeVert = None;
		if(atomMaximizeVert == None)
			atomMaximizeVert = XInternAtom(QX11Info::display(), "_NET_WM_STATE_MAXIMIZED_VERT", False);
		static Atom atomMaximizeHorz = None;
		if(atomMaximizeHorz == None)
			atomMaximizeHorz = XInternAtom(QX11Info::display(), "_NET_WM_STATE_MAXIMIZED_HORZ", False);

		if(atomState == None || atomMaximizeVert == None || atomMaximizeHorz == None)
			return false;

		XEvent event;
		memset(&event, 0, sizeof(event));
		event.type = ClientMessage;
		event.xclient.display = QX11Info::display();
		event.xclient.window = mValue;
		event.xclient.message_type = atomState;
		event.xclient.format = 32;
		event.xclient.data.l[0] = 1;//_NET_WM_STATE_ADD
		event.xclient.data.l[1] = atomMaximizeVert;
		event.xclient.data.l[2] = atomMaximizeHorz;
		event.xclient.data.l[3] = 1;//Message from a normal application

		XWindowAttributes windowAttributes;
		if(!XGetWindowAttributes(QX11Info::display(), mValue, &windowAttributes))
			return false;

		return XSendEvent(QX11Info::display(), windowAttributes.screen->root, False, SubstructureNotifyMask | SubstructureRedirectMask, &event);
#endif
#ifdef Q_WS_WIN
		return (!SendMessage(mValue, WM_SYSCOMMAND, SC_MAXIMIZE, 0));
#endif
	}

	bool WindowHandle::move(QPoint position) const
	{
#ifdef Q_WS_X11
		return XMoveWindow(QX11Info::display(), mValue, position.x(), position.y());
#endif
#ifdef Q_WS_WIN
		return SetWindowPos(mValue, 0, position.x(), position.y(), 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
#endif
	}

	bool WindowHandle::resize(QSize size, bool useBorders) const
	{
#ifdef Q_WS_X11
		if(useBorders)
		{
			XWindowAttributes windowAttributes;

			if(!XGetWindowAttributes(QX11Info::display(), mValue, &windowAttributes))
				return false;

			size.rwidth() += windowAttributes.border_width;
			size.rheight() += windowAttributes.border_width;
		}

		return XResizeWindow(QX11Info::display(), mValue, size.width(), size.height());
#endif
#ifdef Q_WS_WIN
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

		return SetWindowPos(mValue, 0, 0, 0, size.width(), size.height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
#endif
	}

	bool WindowHandle::isActive() const
	{
		return (foregroundWindow() == *this);
	}

	WindowHandle WindowHandle::foregroundWindow()
	{
#ifdef Q_WS_X11
		Window focus;
		int revert = 0;

		XGetInputFocus(QX11Info::display(), &focus, &revert);

		return focus;
#endif
#ifdef Q_WS_WIN
		return GetForegroundWindow();
#endif
	}

#ifdef Q_WS_WIN
	BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
	{
		Q_UNUSED(lParam)

		if(IsWindowVisible(hwnd))
			gWindowList.append(hwnd);

		return true;
	}
#endif

	QList<WindowHandle> WindowHandle::windowList()
	{
		gWindowList.clear();

#ifdef Q_WS_X11
		static Atom net_clients = None;
		if(!net_clients)
			net_clients = XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", True);

		int count = 0;
		Window* list = 0;
		Atom type = 0;
		int format = 0;
		unsigned long after = 0;
		XGetWindowProperty(QX11Info::display(), QX11Info::appRootWindow(), net_clients, 0, 256 * sizeof(Window), False, AnyPropertyType,
						   &type, &format, reinterpret_cast<unsigned long*>(&count), &after, reinterpret_cast<unsigned char**>(&list));

		for (int i = 0; i < count; ++i)
			gWindowList.append(list[i]);

		XFree(list);
#endif
#ifdef Q_WS_WIN
		HDESK hdesk = OpenInputDesktop(0, false, DESKTOP_READOBJECTS);

		EnumDesktopWindows(hdesk, EnumWindowsProc, 0);

		CloseDesktop(hdesk);
#endif

		return gWindowList;
	}

	WindowHandle WindowHandle::findWindow(const QString &title)
	{
		foreach(const WindowHandle &windowHandle, windowList())
		{
			if(windowHandle.title() == title)
				return windowHandle;
		}

		return WindowHandle();
	}

	WindowHandle WindowHandle::findWindow(const QRegExp &regExp)
	{
		if(!regExp.isValid())
			return WindowHandle();

		foreach(const WindowHandle &windowHandle, windowList())
		{
			if(regExp.exactMatch(windowHandle.title()))
				return windowHandle;
		}

		return WindowHandle();
	}

	QList<WindowHandle> WindowHandle::findWindows(const QString &title)
	{
		QList<WindowHandle> back;

		foreach(const WindowHandle &windowHandle, windowList())
		{
			if(windowHandle.title() == title)
				back.append(windowHandle);
		}

		return back;
	}

	QList<WindowHandle> WindowHandle::findWindows(const QRegExp &regExp)
	{
		QList<WindowHandle> back;

		foreach(const WindowHandle &windowHandle, windowList())
		{
			if(regExp.exactMatch(windowHandle.title()))
				back.append(windowHandle);
		}

		return back;
	}
}
