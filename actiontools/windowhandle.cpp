/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#ifndef Q_WS_MAC
#include <QxtWindowSystem>
#endif

#ifdef Q_WS_X11
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#endif

#ifdef Q_WS_WIN
#include <windows.h>
#endif

namespace ActionTools
{
	QString WindowHandle::title() const
	{
#ifdef Q_WS_MAC
		return "";//TODO_MAC
#else
		return QxtWindowSystem::windowTitle(mValue);
#endif
	}

	QString WindowHandle::classname() const
	{
#ifdef Q_WS_WIN
		wchar_t className[255];

		GetClassName(mValue, className, sizeof(className)-1);

		return QString::fromWCharArray(className);
#endif
#ifdef Q_WS_X11
		XClassHint *hint = XAllocClassHint();
		QString back ;

		if(XGetClassHint(QX11Info::display(), mValue, hint))
			back = QString::fromUtf8(hint->res_class);

		XFree(hint);

		return back;
#endif
#ifdef Q_WS_MAC
		return "";//TODO_MAC
#endif
	}

	QRect WindowHandle::rect() const
	{
#ifdef Q_WS_MAC
		return QRect();//TODO_MAC
#else
		return QxtWindowSystem::windowGeometry(mValue);
#endif
	}

	int WindowHandle::processId() const
	{
#ifdef Q_WS_WIN
		DWORD procID;

		GetWindowThreadProcessId(mValue, &procID);

		return procID;
#endif
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
#ifdef Q_WS_MAC
		return 0;//TODO_MAC
#endif
	}

	bool WindowHandle::close() const
	{
#ifdef Q_WS_WIN
		return SendNotifyMessage(mValue, WM_CLOSE, 0, 0);
#endif
#ifdef Q_WS_X11
		return (XDestroyWindow(QX11Info::display(), mValue) == Success);
#endif
	}

	bool WindowHandle::killCreator() const
	{
#ifdef Q_WS_WIN
		int id = processId();

		return CrossPlatform::killProcess(id);
#endif
#ifdef Q_WS_X11
		return (XKillClient(QX11Info::display(), mValue) == Success);
#endif
	}
	
	bool WindowHandle::setForeground() const
	{
#ifdef Q_WS_X11
		return (XRaiseWindow(QX11Info::display(), mValue) == Success);
#endif
#ifdef Q_WS_WIN
		if(IsIconic(mValue))
			ShowWindow(mValue, SW_RESTORE);
		else
		{
			if(!SetForegroundWindow(mValue))
				return false;

			if(!SetWindowPos(mValue, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
				return false;
		}
		
		return true;
#endif
	}
	
	bool WindowHandle::minimize() const
	{
#ifdef Q_WS_X11
		return false;
#endif
#ifdef Q_WS_WIN
		return SendMessage(mValue, WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
	}
	
	bool WindowHandle::maximize() const
	{
#ifdef Q_WS_X11
		return false;
#endif
#ifdef Q_WS_WIN
		return SendMessage(mValue, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
#endif
	}
	
	bool WindowHandle::move(QPoint position) const
	{
#ifdef Q_WS_X11
		return (XMoveWindow(QX11Info::display(), mValue, position.x(), position.y()) == Success);
#endif
#ifdef Q_WS_WIN
		return SetWindowPos(mValue, 0, position.x(), position.y(), 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
#endif
	}
	
	bool WindowHandle::resize(QSize size) const
	{
#ifdef Q_WS_X11
		return (XResizeWindow(QX11Info::display(), mValue, size.width(), size.height()) == Success);
#endif
#ifdef Q_WS_WIN
		return SetWindowPos(mValue, 0, 0, 0, size.width(), size.height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
#endif
	}
	
	bool WindowHandle::isVisible() const
	{
#ifdef Q_WS_X11
		XWindowAttributes attributes;
		if(XGetWindowAttributes(QX11Info::display(), mValue, &attributes) != Success)
			return false;
		
		return (attributes.map_state == IsViewable);
#endif
#ifdef Q_WS_WIN
		return IsWindowVisible(mValue);
#endif
	}

	WindowHandle WindowHandle::foregroundWindow()
	{
#ifdef Q_WS_MAC
		return WindowHandle();//TODO_MAC
#else
		return QxtWindowSystem::activeWindow();
#endif
	}
	
	WindowHandle WindowHandle::findWindow(const QString &title)
	{
		return QxtWindowSystem::findWindow(title);
	}
}
