/*
	Actiona
	Copyright (C) 2005-2017 Jonathan Mercier-Ganady

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

#include "systeminstance.h"
#include "actiondefinition.h"

namespace Actions
{
	ActionTools::StringListPair SystemInstance::operations = qMakePair(
			QStringList() << QStringLiteral("logout") << QStringLiteral("reboot") << QStringLiteral("shutdown") << QStringLiteral("suspend") << QStringLiteral("hibernate") << QStringLiteral("lockscreen") << QStringLiteral("startscreensaver"),
			QStringList()
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", QStringLiteral("Logout"))
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", QStringLiteral("Reboot"))
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", QStringLiteral("Shutdown"))
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", QStringLiteral("Suspend"))
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", QStringLiteral("Hibernate"))
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", QStringLiteral("Lock screen"))
			<< QT_TRANSLATE_NOOP("SystemInstance::operations", QStringLiteral("Start screen saver")));

	void SystemInstance::startExecution()
	{
		bool ok = true;

		Operation operation = evaluateListElement<Operation>(ok, operations, QStringLiteral("operation"));
		bool force = evaluateBoolean(ok, QStringLiteral("force"));

		if(!ok)
			return;

		SystemSession systemSession;

		switch(operation)
		{
		case Logout:
			if(!systemSession.logout(force))
			{
				emit executionException(NotAvailable, tr("Logout is not available"));
				return;
			}
			break;
		case Restart:
			if(!systemSession.restart(force))
			{
				emit executionException(NotAvailable, tr("Restart is not available"));
				return;
			}
			break;
		case Shutdown:
			if(!systemSession.shutdown(force))
			{
				emit executionException(NotAvailable, tr("Shutdown is not available"));
				return;
			}
			break;
		case Suspend:
			if(!systemSession.suspend(force))
			{
				emit executionException(NotAvailable, tr("Suspend is not available"));
				return;
			}
			break;
		case Hibernate:
			if(!systemSession.hibernate(force))
			{
				emit executionException(NotAvailable, tr("Hibernate is not available"));
				return;
			}
			break;
		case LockScreen:
			if(!systemSession.lockScreen())
			{
				emit executionException(NotAvailable, tr("Lock is not available"));
				return;
			}
			break;
		case StartScreenSaver:
			if(!systemSession.startScreenSaver())
			{
				emit executionException(NotAvailable, tr("Start screen saver is not available"));
				return;
			}
			break;
		}

		emit executionEnded();
	}
}

