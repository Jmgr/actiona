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

#include "systemsession.hpp"

#ifdef Q_OS_WIN
#include <Windows.h>
#include <powrprof.h>
#include <QMessageBox>
#endif

SystemSession::SystemSession()
{
}

bool SystemSession::logout(bool force) const
{
#ifdef Q_OS_WIN
	return ExitWindowsEx(EWX_LOGOFF | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
#endif
}

bool SystemSession::restart(bool force) const
{
#ifdef Q_OS_WIN
	return ExitWindowsEx(EWX_REBOOT | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
#endif
}

bool SystemSession::shutdown(bool force) const
{
#ifdef Q_OS_WIN
	return ExitWindowsEx(EWX_POWEROFF | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
#endif
}

bool SystemSession::suspend(bool force) const
{
#ifdef Q_OS_WIN
	return SetSuspendState(false, force, true);
#endif
}

bool SystemSession::hibernate(bool force) const
{
#ifdef Q_OS_WIN
	return SetSuspendState(true, force, false);
#endif
}

bool SystemSession::lockScreen() const
{
#ifdef Q_OS_WIN
	return LockWorkStation();
#endif
}

bool SystemSession::startScreenSaver() const
{
#ifdef Q_OS_WIN
	SendMessage(GetDesktopWindow(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);

	return true;
#endif
}
