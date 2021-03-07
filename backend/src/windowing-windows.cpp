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

#include "backend/windowing-windows.hpp"
#include "backend/backend.hpp"

#include <QWidget>

#include <Windows.h>
#include <Tlhelp32.h>

namespace Backend
{
    void setForegroundWindowWindows(QWidget *window)
    {
        // TODO
        if(IsIconic(reinterpret_cast<HWND>(window->winId())))
            ShowWindow(reinterpret_cast<HWND>(window->winId()), SW_RESTORE);
                else
                {
            if(!SetForegroundWindow(reinterpret_cast<HWND>(window->winId())))
                ;//setupLastError();

            if(!SetWindowPos(reinterpret_cast<HWND>(window->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
                ;//setupLastError();
                }
    }
}

