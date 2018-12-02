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

#include "systemsession.h"

#ifdef Q_OS_WIN
#include <Windows.h>
#include <powrprof.h>
#include <QMessageBox>
#endif

#ifdef Q_OS_UNIX
#include <QProcess>
#include <QDBusInterface>
#include <QDBusReply>
#endif

#ifdef Q_OS_UNIX
int SystemSession::mCapabilities = 0;
#endif

SystemSession::SystemSession()
{
#ifdef Q_OS_UNIX
    if(!mCapabilities)
        checkOperatingSystemCapabilities();
#endif
}

bool SystemSession::logout(bool force) const
{
#ifdef Q_OS_UNIX
	if(mCapabilities & GnomeSessionManager)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.gnome.SessionManager"), QStringLiteral("/org/gnome/SessionManager"), QStringLiteral("org.gnome.SessionManager"), QDBusConnection::sessionBus());
		unsigned int code = (force ? 2 : 1);
		QDBusMessage reply = dbusInterface.call(QStringLiteral("Logout"), code);

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & KdeKSMServer)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.kde.ksmserver"), QStringLiteral("/KSMServer"), QStringLiteral("org.kde.KSMServerInterface"), QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("logout"), 0, 3, (force ? 2 : 1));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_OS_WIN
	return ExitWindowsEx(EWX_LOGOFF | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
#endif
}

bool SystemSession::restart(bool force) const
{
#ifdef Q_OS_UNIX
	if(mCapabilities & GnomeSessionManager)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.gnome.SessionManager"), QStringLiteral("/org/gnome/SessionManager"), QStringLiteral("org.gnome.SessionManager"), QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("RequestReboot"));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & KdeKSMServer)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.kde.ksmserver"), QStringLiteral("/KSMServer"), QStringLiteral("org.kde.KSMServerInterface"), QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("logout"), 0, 1, (force ? 2 : 1));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
    if(mCapabilities & FreedesktopLogind)
    {
        QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.login1"), QStringLiteral("/org/freedesktop/login1"), QStringLiteral("org.freedesktop.login1.Manager"), QDBusConnection::systemBus());
        QDBusMessage reply = dbusInterface.call(QStringLiteral("Reboot"), !force);

        if(reply.type() != QDBusMessage::ErrorMessage)
            return true;
    }
	if(mCapabilities & FreedesktopConsoleKit)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.ConsoleKit"), QStringLiteral("/org/freedesktop/ConsoleKit/Manager"), QStringLiteral("org.freedesktop.ConsoleKit.Manager"), QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("Restart"));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopHal)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.Hal"), QStringLiteral("/org/freedesktop/Hal/devices/computer"), QStringLiteral("org.freedesktop.Hal.Device.SystemPowerManagement"), QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("Reboot"));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_OS_WIN
	return ExitWindowsEx(EWX_REBOOT | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
#endif
}

bool SystemSession::shutdown(bool force) const
{
#ifdef Q_OS_UNIX
	if(mCapabilities & GnomeSessionManager)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.gnome.SessionManager"), QStringLiteral("/org/gnome/SessionManager"), QStringLiteral("org.gnome.SessionManager"), QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("RequestShutdown"));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & KdeKSMServer)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.kde.ksmserver"), QStringLiteral("/KSMServer"), QStringLiteral("org.kde.KSMServerInterface"), QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("logout"), 0, 2, (force ? 2 : 1));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
    if(mCapabilities & FreedesktopLogind)
    {
        QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.login1"), QStringLiteral("/org/freedesktop/login1"), QStringLiteral("org.freedesktop.login1.Manager"), QDBusConnection::systemBus());
        QDBusMessage reply = dbusInterface.call(QStringLiteral("PowerOff"), !force);

        if(reply.type() != QDBusMessage::ErrorMessage)
            return true;
    }
	if(mCapabilities & FreedesktopConsoleKit)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.ConsoleKit"), QStringLiteral("/org/freedesktop/ConsoleKit/Manager"), QStringLiteral("org.freedesktop.ConsoleKit.Manager"), QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("Stop"));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopHal)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.Hal"), QStringLiteral("/org/freedesktop/Hal/devices/computer"), QStringLiteral("org.freedesktop.Hal.Device.SystemPowerManagement"), QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("Shutdown"));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_OS_WIN
	return ExitWindowsEx(EWX_POWEROFF | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER);
#endif
}

bool SystemSession::suspend(bool force) const
{
#ifdef Q_OS_UNIX
	Q_UNUSED(force)

	if(mCapabilities & FreedesktopUPower)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.UPower"), QStringLiteral("/org/freedesktop/UPower"), QStringLiteral("org.freedesktop.UPower"), QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("Suspend"));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopDeviceKit)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.DeviceKit.Power"), QStringLiteral("/org/freedesktop/DeviceKit/Power"), QStringLiteral("org.freedesktop.DeviceKit.Power"), QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("Suspend"));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopHal)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.Hal"), QStringLiteral("/org/freedesktop/Hal/devices/computer"), QStringLiteral("org.freedesktop.Hal.Device.SystemPowerManagement"), QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("Suspend"), 1);

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_OS_WIN
	return SetSuspendState(false, force, true);
#endif
}

bool SystemSession::hibernate(bool force) const
{
#ifdef Q_OS_UNIX
	Q_UNUSED(force)

	if(mCapabilities & FreedesktopUPower)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.UPower"), QStringLiteral("/org/freedesktop/UPower"), QStringLiteral("org.freedesktop.UPower"), QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("Hibernate"));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopDeviceKit)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.DeviceKit.Power"), QStringLiteral("/org/freedesktop/DeviceKit/Power"), QStringLiteral("org.freedesktop.DeviceKit.Power"), QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("Hibernate"));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopHal)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.Hal"), QStringLiteral("/org/freedesktop/Hal/devices/computer"), QStringLiteral("org.freedesktop.Hal.Device.SystemPowerManagement"), QDBusConnection::systemBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("Hibernate"));

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_OS_WIN
	return SetSuspendState(true, force, false);
#endif
}

bool SystemSession::lockScreen() const
{
#ifdef Q_OS_UNIX
	if(mCapabilities & GnomeScreenSaver)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.gnome.ScreenSaver"), QStringLiteral("/ScreenSaver"), QStringLiteral("org.gnome.ScreenSaver"), QDBusConnection::sessionBus());
		dbusInterface.asyncCall(QStringLiteral("Lock"));
		return true;
	}
	if(mCapabilities & FreedesktopScreenSaver)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.ScreenSaver"), QStringLiteral("/ScreenSaver"), QStringLiteral("org.freedesktop.ScreenSaver"), QDBusConnection::sessionBus());
		dbusInterface.asyncCall(QStringLiteral("Lock"));
		return true;
	}
	if(mCapabilities & KdeScreenSaver)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.kde.screensaver"), QStringLiteral("/ScreenSaver"), QStringLiteral("org.freedesktop.ScreenSaver"), QDBusConnection::sessionBus());
		dbusInterface.asyncCall(QStringLiteral("Lock"));
		return true;
	}

	return false;
#endif
#ifdef Q_OS_WIN
	return LockWorkStation();
#endif
}

bool SystemSession::startScreenSaver() const
{
#ifdef Q_OS_UNIX
	if(mCapabilities & GnomeScreenSaver)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.gnome.ScreenSaver"), QStringLiteral("/ScreenSaver"), QStringLiteral("org.gnome.ScreenSaver"), QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("SetActive"), true);

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & FreedesktopScreenSaver)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.freedesktop.ScreenSaver"), QStringLiteral("/ScreenSaver"), QStringLiteral("org.freedesktop.ScreenSaver"), QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("SetActive"), true);

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}
	if(mCapabilities & KdeScreenSaver)
	{
		QDBusInterface dbusInterface(QStringLiteral("org.kde.screensaver"), QStringLiteral("/ScreenSaver"), QStringLiteral("org.freedesktop.ScreenSaver"), QDBusConnection::sessionBus());
		QDBusMessage reply = dbusInterface.call(QStringLiteral("SetActive"), true);

		if(reply.type() != QDBusMessage::ErrorMessage)
			return true;
	}

	return false;
#endif
#ifdef Q_OS_WIN
	SendMessage(GetDesktopWindow(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);

	return true;
#endif
}

#ifdef Q_OS_UNIX
bool SystemSession::checkForDBusInterface(const QString &service, const QString &path, const QString &interface, const QString &testMethod, bool systemBus) const
{
	QDBusInterface dbusInterface(service, path, interface, systemBus ? QDBusConnection::systemBus() : QDBusConnection::sessionBus());
	QDBusMessage reply = dbusInterface.call(testMethod);

	return (reply.type() != QDBusMessage::ErrorMessage);
}

void SystemSession::checkOperatingSystemCapabilities()
{
	mCapabilities |= checkForDBusInterface(QStringLiteral("org.gnome.SessionManager"),
								  QStringLiteral("/org/gnome/SessionManager"),
								  QStringLiteral("org.gnome.SessionManager"),
								  QStringLiteral("CanShutdown"), false) ? GnomeSessionManager : 0;
	mCapabilities |= checkForDBusInterface(QStringLiteral("org.gnome.ScreenSaver"),
								  QStringLiteral("/ScreenSaver"),
								  QStringLiteral("org.gnome.ScreenSaver"),
								  QStringLiteral("GetActive"), false) ? GnomeScreenSaver : 0;
	mCapabilities |= checkForDBusInterface(QStringLiteral("org.freedesktop.ConsoleKit"),
								  QStringLiteral("/org/freedesktop/ConsoleKit/Manager"),
								  QStringLiteral("org.freedesktop.ConsoleKit.Manager"),
								  QStringLiteral("CanRestart"), true) ? FreedesktopConsoleKit : 0;
	mCapabilities |= checkForDBusInterface(QStringLiteral("org.freedesktop.ScreenSaver"),
								  QStringLiteral("/ScreenSaver"),
								  QStringLiteral("org.freedesktop.ScreenSaver"),
								  QStringLiteral("GetActive"), false) ? FreedesktopScreenSaver : 0;
	mCapabilities |= checkForDBusInterface(QStringLiteral("org.freedesktop.Hal"),
								  QStringLiteral("/org/freedesktop/Hal/devices/computer"),
								  QStringLiteral("org.freedesktop.Hal.Device.SystemPowerManagement"),
								  QStringLiteral("canShutdown"), true) ? FreedesktopHal : 0;
	mCapabilities |= checkForDBusInterface(QStringLiteral("org.freedesktop.UPower"),
								  QStringLiteral("/org/freedesktop/UPower"),
								  QStringLiteral("org.freedesktop.UPower"),
								  QStringLiteral("SuspendAllowed"), true) ? FreedesktopUPower : 0;
	mCapabilities |= checkForDBusInterface(QStringLiteral("org.freedesktop.DeviceKit.Power"),
								  QStringLiteral("/org/freedesktop/DeviceKit/Power"),
								  QStringLiteral("org.freedesktop.DeviceKit.Power"),
								  QStringLiteral("EnumerateDevices"), true) ? FreedesktopDeviceKit : 0;
	mCapabilities |= checkForDBusInterface(QStringLiteral("org.kde.screensaver"),
								  QStringLiteral("/ScreenSaver"),
								  QStringLiteral("org.freedesktop.ScreenSaver"),
								  QStringLiteral("GetActive"), false) ? KdeScreenSaver : 0;
	mCapabilities |= checkForDBusInterface(QStringLiteral("org.kde.ksmserver"),
								  QStringLiteral("/KSMServer"),
								  QStringLiteral("org.kde.KSMServerInterface"),
								  QStringLiteral("canShutdown"), false) ? KdeKSMServer : 0;
    mCapabilities |= checkForDBusInterface(QStringLiteral("org.freedesktop.login1"),
                                  QStringLiteral("/org/freedesktop/login1"),
                                  QStringLiteral("org.freedesktop.login1.Manager"),
                                  QStringLiteral("CanPowerOff"), true) ? FreedesktopLogind : 0;
}
#endif
