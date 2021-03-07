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

#include "actiontools/crossplatform.hpp"
#include "actiontools/windowhandle.hpp"

#include <QWidget>
#include <QElapsedTimer>
#include <QDebug>
#include <QDir>

#ifdef Q_OS_UNIX
#include <QX11Info>
#include <X11/Xlib.h>
#include <cerrno>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#include <Tlhelp32.h>
#endif

namespace ActionTools
{
    // TODO
	void CrossPlatform::setForegroundWindow(QWidget *window)
	{
#ifdef Q_OS_UNIX
		XRaiseWindow(QX11Info::display(), window->winId());
#endif
#ifdef Q_OS_WIN
        if(IsIconic(reinterpret_cast<HWND>(window->winId())))
            ShowWindow(reinterpret_cast<HWND>(window->winId()), SW_RESTORE);
		else
		{
            if(!SetForegroundWindow(reinterpret_cast<HWND>(window->winId())))
                ;//setupLastError();

            if(!SetWindowPos(reinterpret_cast<HWND>(window->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
                ;//setupLastError();
		}
#endif
	}
}
