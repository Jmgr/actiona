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

#include "backend/system-windows.hpp"
#include "backend/backend.hpp"
#include "backend/backend-windows.hpp"

#include <Windows.h>
#include <powrprof.h>
#include <lmcons.h>

#include <array>

namespace Backend
{
    void logoutWindows(bool force)
    {
        if(!ExitWindowsEx(EWX_LOGOFF | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER))
            throw BackendError(lastErrorString());
    }

    void restartWindows(bool force)
    {
        if(!ExitWindowsEx(EWX_REBOOT | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER))
            throw BackendError(lastErrorString());
    }

    void shutdownWindows(bool force)
    {
        if(!ExitWindowsEx(EWX_POWEROFF | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER))
            throw BackendError(lastErrorString());
    }

    void suspendWindows(bool force)
    {
        if(!SetSuspendState(false, force, true))
            throw BackendError(lastErrorString());
    }

    void hibernateWindows(bool force)
    {
        if(!SetSuspendState(true, force, false))
            throw BackendError(lastErrorString());
    }

    void lockScreenWindows()
    {
        if(!LockWorkStation())
            throw BackendError(lastErrorString());
    }

    void startScreenSaverWindows()
    {
        SendMessage(GetDesktopWindow(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);
    }

    QString getUsernameWindows()
    {
        std::array<TCHAR, UNLEN+1> buffer;
        DWORD size = buffer.size();
        if(!GetUserName(buffer.data(), &size))
            throw BackendError(lastErrorString());

        return QString::fromWCharArray(buffer.data());
    }
}

