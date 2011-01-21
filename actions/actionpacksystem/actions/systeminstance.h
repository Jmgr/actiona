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
			Shutdown,
			Suspend,
			Hibernate,
			LockScreen,
			StartScreenSaver
		};
		enum Mode
		{
			Normal,
			Force
		};
		enum Exceptions
		{
			NotEnoughRightsException = ActionTools::ActionException::UserException,
			NotAvailable
		};
#ifdef Q_WS_X11
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
		KdeKSMServer = 1<<8
	};
#endif

		SystemInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)
#ifdef Q_WS_X11
			, mCapabilities(0)
#endif
		{}

		static ActionTools::StringListPair operations;
		static ActionTools::StringListPair modes;

		void startExecution();

	private:
#ifdef Q_WS_X11
	bool checkForDBusInterface(const QString &service, const QString &path, const QString &interface, const QString &testMethod, bool systemBus) const;
	bool checkForDBusInterface(const QString &service, const QString &path, const QString &testMethod, bool systemBus) const;
	void checkOperatingSystemCapabilities();

	int mCapabilities;
#endif

		Q_DISABLE_COPY(SystemInstance)
	};
}

#endif // SYSTEMINSTANCE_H
