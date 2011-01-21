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

#include "systeminstance.h"
#include "actiondefinition.h"

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

namespace Actions
{
	ActionTools::StringListPair SystemInstance::operations = qMakePair(
			QStringList() << "logout" << "reboot" << "shutdown" << "suspend" << "hibernate" << "lockscreen" << "startscreensaver",
			QStringList()
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", "Logout")
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", "Reboot")
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", "Shutdown")
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", "Suspend")
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", "Hibernate")
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", "Lock screen")
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", "Start screen saver"));

	ActionTools::StringListPair SystemInstance::modes = qMakePair(
			QStringList() << "normal" << "force",
			QStringList() << QT_TRANSLATE_NOOP("SystemInstance::modes", "Normal") << QT_TRANSLATE_NOOP("SystemInstance::modes", "Force"));

	void SystemInstance::startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		Operation operation;
		Mode mode;

		if(!actionInstanceExecutionHelper.evaluateListElement(operation, operations, "operation") ||
		   !actionInstanceExecutionHelper.evaluateListElement(mode, modes, "mode"))
			return;

#ifdef Q_WS_X11
		if(!mCapabilities)
			checkOperatingSystemCapabilities();
#endif

		switch(operation)
		{
		case Logout:
#ifdef Q_WS_X11
			{
				if(mCapabilities & GnomeSessionManager)
				{
					QDBusInterface dbusInterface("org.gnome.SessionManager", "/org/gnome/SessionManager", "org.gnome.SessionManager", QDBusConnection::sessionBus());
					unsigned int code = mode == Normal ? 1 : 2;
					QDBusMessage reply = dbusInterface.call("Logout", code);

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & KdeKSMServer)
				{
					QDBusInterface dbusInterface("org.kde.ksmserver", "/KSMServer", "org.kde.KSMServerInterface", QDBusConnection::sessionBus());
					QDBusMessage reply = dbusInterface.call("logout", 0, 3, mode == Force ? 2 : 1);

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}

				emit executionException(NotAvailable, tr("Logout is not available"));
				return;
			}
#endif
#ifdef Q_WS_WIN
			if(!ExitWindowsEx(EWX_LOGOFF | (mode == Force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER))
			{
				emit executionException(NotEnoughRightsException, tr("Not enough rights to logout"));
				return;
			}
#endif
			break;
		case Restart:
#ifdef Q_WS_X11
			{
				if(mCapabilities & GnomeSessionManager)
				{
					QDBusInterface dbusInterface("org.gnome.SessionManager", "/org/gnome/SessionManager", "org.gnome.SessionManager", QDBusConnection::sessionBus());
					QDBusMessage reply = dbusInterface.call("RequestReboot");

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & KdeKSMServer)
				{
					QDBusInterface dbusInterface("org.kde.ksmserver", "/KSMServer", "org.kde.KSMServerInterface", QDBusConnection::sessionBus());
					QDBusMessage reply = dbusInterface.call("logout", 0, 1, mode == Force ? 2 : 1);

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & FreedesktopConsoleKit)
				{
					QDBusInterface dbusInterface("org.freedesktop.ConsoleKit", "/org/freedesktop/ConsoleKit/Manager", "org.freedesktop.ConsoleKit.Manager", QDBusConnection::systemBus());
					QDBusMessage reply = dbusInterface.call("Restart");

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & FreedesktopHal)
				{
					QDBusInterface dbusInterface("org.freedesktop.Hal", "/org/freedesktop/Hal/devices/computer", "org.freedesktop.Hal.Device.SystemPowerManagement", QDBusConnection::systemBus());
					QDBusMessage reply = dbusInterface.call("Reboot");

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}

				emit executionException(NotAvailable, tr("Restart is not available"));
				return;
			}
#endif
#ifdef Q_WS_WIN
			if(!ExitWindowsEx(EWX_REBOOT | (mode == Force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER))
			{
				emit executionException(NotEnoughRightsException, tr("Not enough rights to restart"));
				return;
			}
#endif
			break;
		case Shutdown:
#ifdef Q_WS_X11
			{
				if(mCapabilities & GnomeSessionManager)
				{
					QDBusInterface dbusInterface("org.gnome.SessionManager", "/org/gnome/SessionManager", "org.gnome.SessionManager", QDBusConnection::sessionBus());
					QDBusMessage reply = dbusInterface.call("RequestShutdown");

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & KdeKSMServer)
				{
					QDBusInterface dbusInterface("org.kde.ksmserver", "/KSMServer", "org.kde.KSMServerInterface", QDBusConnection::sessionBus());
					QDBusMessage reply = dbusInterface.call("logout", 0, 2, mode == Force ? 2 : 1);

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & FreedesktopConsoleKit)
				{
					QDBusInterface dbusInterface("org.freedesktop.ConsoleKit", "/org/freedesktop/ConsoleKit/Manager", "org.freedesktop.ConsoleKit.Manager", QDBusConnection::systemBus());
					QDBusMessage reply = dbusInterface.call("Stop");

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & FreedesktopHal)
				{
					QDBusInterface dbusInterface("org.freedesktop.Hal", "/org/freedesktop/Hal/devices/computer", "org.freedesktop.Hal.Device.SystemPowerManagement", QDBusConnection::systemBus());
					QDBusMessage reply = dbusInterface.call("Shutdown");

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}

				emit executionException(NotAvailable, tr("Shutdown is not available"));
				return;
			}
#endif
#ifdef Q_WS_WIN
			if(!ExitWindowsEx(EWX_POWEROFF | (mode == Force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER))
			{
				emit executionException(NotEnoughRightsException, tr("Not enough rights to shutdown"));
				return;
			}
#endif
			break;
		case Suspend:
#ifdef Q_WS_X11
			{
				if(mCapabilities & FreedesktopUPower)
				{
					QDBusInterface dbusInterface("org.freedesktop.UPower", "/org/freedesktop/UPower", "org.freedesktop.UPower", QDBusConnection::systemBus());
					QDBusMessage reply = dbusInterface.call("Suspend");

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & FreedesktopDeviceKit)
				{
					QDBusInterface dbusInterface("org.freedesktop.DeviceKit.Power", "/org/freedesktop/DeviceKit/Power", "org.freedesktop.DeviceKit.Power", QDBusConnection::systemBus());
					QDBusMessage reply = dbusInterface.call("Suspend");

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & FreedesktopHal)
				{
					QDBusInterface dbusInterface("org.freedesktop.Hal", "/org/freedesktop/Hal/devices/computer", "org.freedesktop.Hal.Device.SystemPowerManagement", QDBusConnection::systemBus());
					QDBusMessage reply = dbusInterface.call("Suspend", 1);

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}

				emit executionException(NotAvailable, tr("Suspend is not available"));
				return;
			}
#endif
#ifdef Q_WS_WIN
			if(!SetSuspendState(false, (mode == Force), true))
			{
				emit executionException(NotEnoughRightsException, tr("Suspend failed"));
				return;
			}
#endif
			break;
		case Hibernate:
#ifdef Q_WS_X11
			{
				if(mCapabilities & FreedesktopUPower)
				{
					QDBusInterface dbusInterface("org.freedesktop.UPower", "/org/freedesktop/UPower", "org.freedesktop.UPower", QDBusConnection::systemBus());
					QDBusMessage reply = dbusInterface.call("Hibernate");

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & FreedesktopDeviceKit)
				{
					QDBusInterface dbusInterface("org.freedesktop.DeviceKit.Power", "/org/freedesktop/DeviceKit/Power", "org.freedesktop.DeviceKit.Power", QDBusConnection::systemBus());
					QDBusMessage reply = dbusInterface.call("Hibernate");

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & FreedesktopHal)
				{
					QDBusInterface dbusInterface("org.freedesktop.Hal", "/org/freedesktop/Hal/devices/computer", "org.freedesktop.Hal.Device.SystemPowerManagement", QDBusConnection::systemBus());
					QDBusMessage reply = dbusInterface.call("Hibernate");

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}

				emit executionException(NotAvailable, tr("Hibernate is not available"));
				return;
			}
#endif
#ifdef Q_WS_WIN
			if(!SetSuspendState(true, (mode == Force), false))
			{
				emit executionException(NotEnoughRightsException, tr("Hibernate failed"));
				return;
			}
#endif
			break;
		case LockScreen:
#ifdef Q_WS_X11
			{
				if(mCapabilities & GnomeScreenSaver)
				{
					QDBusInterface dbusInterface("org.gnome.ScreenSaver", "/ScreenSaver", "org.gnome.ScreenSaver", QDBusConnection::sessionBus());
					dbusInterface.asyncCall("Lock");
					break;
				}
				if(mCapabilities & FreedesktopScreenSaver)
				{
					QDBusInterface dbusInterface("org.freedesktop.ScreenSaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
					dbusInterface.asyncCall("Lock");
					break;
				}
				if(mCapabilities & KdeScreenSaver)
				{
					QDBusInterface dbusInterface("org.kde.screensaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
					dbusInterface.asyncCall("Lock");
					break;
				}

				emit executionException(NotAvailable, tr("Lock is not available"));
				return;
			}
#endif
#ifdef Q_WS_WIN
			if(!LockWorkStation())
			{
				emit executionException(NotEnoughRightsException, tr("Not enough rights to lock"));
				return;
			}
#endif
			break;
		case StartScreenSaver:
#ifdef Q_WS_X11
			{
				if(mCapabilities & GnomeScreenSaver)
				{
					QDBusInterface dbusInterface("org.gnome.ScreenSaver", "/ScreenSaver", "org.gnome.ScreenSaver", QDBusConnection::sessionBus());
					QDBusMessage reply = dbusInterface.call("SetActive", true);

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & FreedesktopScreenSaver)
				{
					QDBusInterface dbusInterface("org.freedesktop.ScreenSaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
					QDBusMessage reply = dbusInterface.call("SetActive", true);

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}
				if(mCapabilities & KdeScreenSaver)
				{
					QDBusInterface dbusInterface("org.kde.screensaver", "/ScreenSaver", "org.freedesktop.ScreenSaver", QDBusConnection::sessionBus());
					QDBusMessage reply = dbusInterface.call("SetActive", true);

					if(reply.type() != QDBusMessage::ErrorMessage)
						break;
				}

				emit executionException(NotAvailable, tr("Start screen saver is not available"));
				return;
			}
#endif
#ifdef Q_WS_WIN
			SendMessage(GetDesktopWindow(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);
#endif
			break;
		}

		emit executionEnded();
	}

#ifdef Q_WS_X11
bool SystemInstance::checkForDBusInterface(const QString &service, const QString &path, const QString &interface, const QString &testMethod, bool systemBus) const
{
	QDBusInterface dbusInterface(service, path, interface, systemBus ? QDBusConnection::systemBus() : QDBusConnection::sessionBus());
	QDBusMessage reply = dbusInterface.call(testMethod);

	return (reply.type() != QDBusMessage::ErrorMessage);
}

void SystemInstance::checkOperatingSystemCapabilities()
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
}

