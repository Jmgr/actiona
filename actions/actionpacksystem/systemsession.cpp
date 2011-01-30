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

#include "systemsession.h"

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <powrprof.h>
#include <QMessageBox>
#endif

#ifdef Q_WS_X11
#include <QProcess>
#include <QDBusInterface>
#include <QDBusReply>
#endif

#ifdef Q_WS_X11
int SystemSession::mCapabilities = 0;
#endif

SystemSession::SystemSession()
{
#ifdef Q_WS_X11
		if(!mCapabilities)
			checkOperatingSystemCapabilities();
#endif
}

bool SystemSession::logout(bool force) const
{
#ifdef Q_WS_X11
	if(mCapabilities & GnomeSessionManager)
	{
		QDBusInterface dbusInterface("org.gnome.SessionManager", "/org/gnome/SessionManager", "org.gnome.SessionManager", QDBusConnection::sessionBus());
		unsigned int code = (force ? 2 : 1);
		QDBusMessage reply = dbusInterface.call("Logout", code);

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & KdeKSMServer)
	{
		QDBusInterface dbusInterface("org.kde.ksmserver", "/KSMServer", "org.kde.KSMServerInterface", QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call("logout", 0, 3, (force ? 2 : 1));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_WS_WIN
	return ExitWindowsEx(EWX_LOGOFF | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
#endif
}

bool SystemSession::restart(bool force) const
{
#ifdef Q_WS_X11
	if(mCapabilities & GnomeSessionManager)
	{
		QDBusInterface dbusInterface("org.gnome.SessionManager", "/org/gnome/SessionManager", "org.gnome.SessionManager", QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call("RequestReboot");

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & KdeKSMServer)
	{
		QDBusInterface dbusInterface("org.kde.ksmserver", "/KSMServer", "org.kde.KSMServerInterface", QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call("logout", 0, 1, (force ? 2 : 1));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopConsoleKit)
	{
		QDBusInterface dbusInterface("org.freedesktop.ConsoleKit", "/org/freedesktop/ConsoleKit/Manager", "org.freedesktop.ConsoleKit.Manager", QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call("Restart");

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopHal)
	{
		QDBusInterface dbusInterface("org.freedesktop.Hal", "/org/freedesktop/Hal/devices/computer", "org.freedesktop.Hal.Device.SystemPowerManagement", QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call("Reboot");

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_WS_WIN
	return ExitWindowsEx(EWX_REBOOT | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
#endif
}

bool SystemSession::shutdown(bool force) const
{
#ifdef Q_WS_X11
	if(mCapabilities & GnomeSessionManager)
	{
		QDBusInterface dbusInterface("org.gnome.SessionManager", "/org/gnome/SessionManager", "org.gnome.SessionManager", QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call("RequestShutdown");

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & KdeKSMServer)
	{
		QDBusInterface dbusInterface("org.kde.ksmserver", "/KSMServer", "org.kde.KSMServerInterface", QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call("logout", 0, 2, (force ? 2 : 1));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopConsoleKit)
	{
		QDBusInterface dbusInterface("org.freedesktop.ConsoleKit", "/org/freedesktop/ConsoleKit/Manager", "org.freedesktop.ConsoleKit.Manager", QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call("Stop");

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopHal)
	{
		QDBusInterface dbusInterface("org.freedesktop.Hal", "/org/freedesktop/Hal/devices/computer", "org.freedesktop.Hal.Device.SystemPowerManagement", QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call("Shutdown");

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_WS_WIN
	return ExitWindowsEx(EWX_POWEROFF | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
#endif
}

bool SystemSession::suspend(bool force) const
{
#ifdef Q_WS_X11
	Q_UNUSED(force)

	if(mCapabilities & FreedesktopUPower)
	{
		QDBusInterface dbusInterface("org.freedesktop.UPower", "/org/freedesktop/UPower", "org.freedesktop.UPower", QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call("Suspend");

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopDeviceKit)
	{
		QDBusInterface dbusInterface("org.freedesktop.DeviceKit.Power", "/org/freedesktop/DeviceKit/Power", "org.freedesktop.DeviceKit.Power", QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call("Suspend");

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopHal)
	{
		QDBusInterface dbusInterface("org.freedesktop.Hal", "/org/freedesktop/Hal/devices/computer", "org.freedesktop.Hal.Device.SystemPowerManagement", QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call("Suspend", 1);

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_WS_WIN
	return SetSuspendState(false, force, true);
#endif
}

bool SystemSession::hibernate(bool force) const
{
#ifdef Q_WS_X11
	Q_UNUSED(force)

	if(mCapabilities & FreedesktopUPower)
	{
		QDBusInterface dbusInterface("org.freedesktop.UPower", "/org/freedesktop/UPower", "org.freedesktop.UPower", QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call("Hibernate");

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopDeviceKit)
	{
		QDBusInterface dbusInterface("org.freedesktop.DeviceKit.Power", "/org/freedesktop/DeviceKit/Power", "org.freedesktop.DeviceKit.Power", QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call("Hibernate");

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopHal)
	{
		QDBusInterface dbusInterface("org.freedesktop.Hal", "/org/freedesktop/Hal/devices/computer", "org.freedesktop.Hal.Device.SystemPowerManagement", QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call("Hibernate");

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_WS_WIN
	return SetSuspendState(true, force, false);
#endif
}

bool SystemSession::lockScreen() const
{
#ifdef Q_WS_X11
	if(mCapabilities & GnomeScreenSaver)
	{
		QDBusInterface dbusInterface("org.gnome.ScreenSaver", "/ScreenSaver", "org.gnome.ScreenSaver", QDBusConnection::sessionBus());
		dbusInterface.asyncCall("Lock");
		return true;
	}
	if(mCapabilities & FreedesktopScreenSaver)
	{
		QDBusInterface dbusInterface("org.freedesktop.ScreenSaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
		dbusInterface.asyncCall("Lock");
		return true;
	}
	if(mCapabilities & KdeScreenSaver)
	{
		QDBusInterface dbusInterface("org.kde.screensaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
		dbusInterface.asyncCall("Lock");
		return true;
	}

	return false;
#endif
#ifdef Q_WS_WIN
	return LockWorkStation();
#endif
}

bool SystemSession::startScreenSaver() const
{
#ifdef Q_WS_X11
	if(mCapabilities & GnomeScreenSaver)
	{
		QDBusInterface dbusInterface("org.gnome.ScreenSaver", "/ScreenSaver", "org.gnome.ScreenSaver", QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call("SetActive", true);

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopScreenSaver)
	{
		QDBusInterface dbusInterface("org.freedesktop.ScreenSaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call("SetActive", true);

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & KdeScreenSaver)
	{
		QDBusInterface dbusInterface("org.kde.screensaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call("SetActive", true);

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_WS_WIN
	SendMessage(GetDesktopWindow(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);

	return true;
#endif
}

#ifdef Q_WS_X11
bool SystemSession::checkForDBusInterface(const QString &service, const QString &path, const QString &interface, const QString &testMethod, bool systemBus) const
{
	QDBusInterface dbusInterface(service, path, interface, systemBus ? QDBusConnection::systemBus() : QDBusConnection::sessionBus());
	QDBusMessage reply = dbusInterface.call(testMethod);

	return (reply.type() != QDBusMessage::ErrorMessage);
}

void SystemSession::checkOperatingSystemCapabilities()
{
	mCapabilities |= checkForDBusInterface("org.gnome.SessionManager",
								  "/org/gnome/SessionManager",
								  "org.gnome.SessionManager",
								  "CanShutdown", false) ? GnomeSessionManager : 0;
	mCapabilities |= checkForDBusInterface("org.gnome.ScreenSaver",
								  "/ScreenSaver",
								  "org.gnome.ScreenSaver",
								  "GetActive", false) ? GnomeScreenSaver : 0;
	mCapabilities |= checkForDBusInterface("org.freedesktop.ConsoleKit",
								  "/org/freedesktop/ConsoleKit/Manager",
								  "org.freedesktop.ConsoleKit.Manager",
								  "CanRestart", true) ? FreedesktopConsoleKit : 0;
	mCapabilities |= checkForDBusInterface("org.freedesktop.ScreenSaver",
								  "/ScreenSaver",
								  "org.freedesktop.ScreenSaver",
								  "GetActive", false) ? FreedesktopScreenSaver : 0;
	mCapabilities |= checkForDBusInterface("org.freedesktop.Hal",
								  "/org/freedesktop/Hal/devices/computer",
								  "org.freedesktop.Hal.Device.SystemPowerManagement",
								  "canShutdown", true) ? FreedesktopHal : 0;
	mCapabilities |= checkForDBusInterface("org.freedesktop.UPower",
								  "/org/freedesktop/UPower",
								  "org.freedesktop.UPower",
								  "SuspendAllowed", true) ? FreedesktopUPower : 0;
	mCapabilities |= checkForDBusInterface("org.freedesktop.DeviceKit.Power",
								  "/org/freedesktop/DeviceKit/Power",
								  "org.freedesktop.DeviceKit.Power",
								  "EnumerateDevices", true) ? FreedesktopDeviceKit : 0;
	mCapabilities |= checkForDBusInterface("org.kde.screensaver",
								  "/ScreenSaver",
								  "org.freedesktop.ScreenSaver",
								  "GetActive", false) ? KdeScreenSaver : 0;
	mCapabilities |= checkForDBusInterface("org.kde.ksmserver",
								  "/KSMServer",
								  "org.kde.KSMServerInterface",
								  "canShutdown", false) ? KdeKSMServer : 0;
}
#endif
