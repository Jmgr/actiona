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

	Contact : jmgr@jmgr.info
*/

#pragma once

#include <QObject>

class SystemSession
{
public:
#ifdef Q_OS_UNIX
	enum Capabilities
	{
		GnomeSessionManager = 1<<0,
		GnomeScreenSaver = 1<<1,
		FreedesktopConsoleKit = 1<<2,
		FreedesktopScreenSaver = 1<<3,
		FreedesktopHal = 1<<4,
		FreedesktopUPower = 1<<5,
		FreedesktopDeviceKit = 1<<6,
		KdeScreenSaver = 1<<7,
        KdeKSMServer = 1<<8,
        FreedesktopLogind = 1<<9
	};
#endif

    SystemSession();

	bool logout(bool force) const;
	bool restart(bool force) const;
	bool shutdown(bool force) const;
	bool suspend(bool force) const;
	bool hibernate(bool force) const;
	bool lockScreen() const;
	bool startScreenSaver() const;

private:
#ifdef Q_OS_UNIX
	bool checkForDBusInterface(const QString &service, const QString &path, const QString &interface, const QString &testMethod, bool systemBus) const;
	bool checkForDBusInterface(const QString &service, const QString &path, const QString &testMethod, bool systemBus) const;
	void checkOperatingSystemCapabilities();

	static int mCapabilities;
#endif

	Q_DISABLE_COPY(SystemSession)
};

