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

#ifndef SYSTEMINSTANCE_H
#define SYSTEMINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"
#include "script.h"

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <QMessageBox>
#endif

#ifdef Q_WS_X11
#include <QProcess>
#include <QDBusInterface>
#include <QDBusReply>
#endif

namespace Actions
{
	class SystemInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Operation)
		Q_ENUMS(Mode)

	public:
		enum Operation
		{
			Logout,
			Restart,
			Shutdown
		};
		enum Mode
		{
			Ask,
			Normal,
			Force
		};
		enum Exceptions
		{
			NotEnoughRightsException = ActionTools::ActionException::UserException,
			NotAvailable
		};

		SystemInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)											{}

		static ActionTools::StringListPair operations;
		static ActionTools::StringListPair modes;

		void startExecution()
		{
			ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
			Operation operation;
			Mode mode;

			if(!actionInstanceExecutionHelper.evaluateListElement(operation, operations, "operation") ||
			   !actionInstanceExecutionHelper.evaluateListElement(mode, modes, "mode"))
				return;

#ifdef Q_WS_X11
			QDBusInterface gnomeInterface("org.gnome.SessionManager",
										  "/org/gnome/SessionManager",
										  "org.gnome.SessionManager");
			QDBusInterface kdeInterface("org.kde.ksmserver",
										"/KSMServer",
										"org.kde.KSMServerInterface");
			QDBusInterface consolekitInterface("org.freedesktop.ConsoleKit",
											   "/org/freedesktop/ConsoleKit/Manager",
											   "org.freedesktop.ConsoleKit.Manager",
											   QDBusConnection::systemBus());
			QDBusInterface halInterface("org.freedesktop.Hal",
										"/org/freedesktop/Hal/devices/computer",
										"org.freedesktop.Hal.Device.SystemPowerManagement",
										QDBusConnection::systemBus());
#endif

			switch(operation)
			{
			case Logout:
#ifdef Q_WS_X11
				{
					unsigned int gnomeLogoutCode;
					switch(mode)
					{
					case Ask:
						gnomeLogoutCode = 0;
						break;
					case Normal:
						gnomeLogoutCode = 1;
						break;
					case Force:
						gnomeLogoutCode = 2;
						break;
					}

					QDBusReply<void> voidReply;

					voidReply = gnomeInterface.call("Logout", gnomeLogoutCode);
					if(voidReply.isValid())
						break;
					voidReply = kdeInterface.call("logout", mode == Ask ? 1 : 0, 3, mode == Force ? 2 : 1);
					if(voidReply.isValid())
						break;

					emit executionException(NotAvailable, tr("Logout is not available"));
					return;
				}
#endif
#ifdef Q_WS_WIN
				if(mode == Ask && QMessageBox::question(0, tr("Logout"), tr("Do you really want to logout ?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
					break;

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
					QDBusReply<void> voidReply;
					QDBusReply<int> intReply;

					voidReply = gnomeInterface.call("RequestReboot");
					if(voidReply.isValid())
						break;
					voidReply = kdeInterface.call("logout", mode == Ask ? 1 : 0, 1, mode == Force ? 2 : 1);
					if(voidReply.isValid())
						break;
					voidReply = consolekitInterface.call("Restart");
					if(voidReply.isValid())
						break;
					intReply = halInterface.call("Reboot");
					if(intReply.isValid())
						break;

					emit executionException(NotAvailable, tr("Restart is not available"));
					return;
				}
#endif
#ifdef Q_WS_WIN
				if(mode == Ask && QMessageBox::question(0, tr("Restart"), tr("Do you really want to restart ?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
					break;

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
					QDBusReply<void> voidReply;
					QDBusReply<int> intReply;

					voidReply = gnomeInterface.call("RequestShutdown");
					if(voidReply.isValid())
						break;
					voidReply = kdeInterface.call("logout", mode == Ask ? 1 : 0, 2, mode == Force ? 2 : 1);
					if(voidReply.isValid())
						break;
					voidReply = consolekitInterface.call("Stop");
					if(voidReply.isValid())
						break;
					intReply = halInterface.call("Shutdown");
					if(intReply.isValid())
						break;

					emit executionException(NotAvailable, tr("Shutdown is not available"));
					return;
				}
#endif
#ifdef Q_WS_WIN
				if(mode == Ask && QMessageBox::question(0, tr("Shutdown"), tr("Do you really want to shutdown ?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
					break;

				if(!ExitWindowsEx(EWX_POWEROFF | (mode == Force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER))
				{
					emit executionException(NotEnoughRightsException, tr("Not enough rights to shutdown"));
					return;
				}
#endif
				break;
			}

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(SystemInstance)
	};
}

#endif // SYSTEMINSTANCE_H
