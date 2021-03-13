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

#include "systeminstance.hpp"
#include "actiontools/actiondefinition.hpp"
#include "backend/system.hpp"

namespace Actions
{
    Tools::StringListPair SystemInstance::operations =
    {
        {
            QStringLiteral("logout"),
            QStringLiteral("reboot"),
            QStringLiteral("shutdown"),
            QStringLiteral("suspend"),
            QStringLiteral("hibernate"),
            QStringLiteral("lockscreen"),
            QStringLiteral("startscreensaver")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("SystemInstance::operations", "Logout")),
            QStringLiteral(QT_TRANSLATE_NOOP("SystemInstance::operations", "Reboot")),
            QStringLiteral(QT_TRANSLATE_NOOP("SystemInstance::operations", "Shutdown")),
            QStringLiteral(QT_TRANSLATE_NOOP("SystemInstance::operations", "Suspend")),
            QStringLiteral(QT_TRANSLATE_NOOP("SystemInstance::operations", "Hibernate")),
            QStringLiteral(QT_TRANSLATE_NOOP("SystemInstance::operations", "Lock screen")),
            QStringLiteral(QT_TRANSLATE_NOOP("SystemInstance::operations", "Start screen saver"))
        }
    };

	void SystemInstance::startExecution()
	{
		bool ok = true;

		auto operation = evaluateListElement<Operation>(ok, operations, QStringLiteral("operation"));
		bool force = evaluateBoolean(ok, QStringLiteral("force"));

		if(!ok)
			return;

        const auto &system = Backend::Instance::system();

        try
        {
            switch(operation)
            {
            case Logout:
                system.logout(force);
                break;
            case Restart:
                system.restart(force);
                break;
            case Shutdown:
                system.shutdown(force);
                break;
            case Suspend:
                system.suspend(force);
                break;
            case Hibernate:
                system.hibernate(force);
                break;
            case LockScreen:
                system.lockScreen();
                break;
            case StartScreenSaver:
                system.startScreenSaver();
                break;
            }
        }
        catch(const Backend::BackendError &e)
        {
            emit executionException(NotAvailable, tr("Operation not available: %1").arg(e.what()));
            return;
        }

		executionEnded();
	}
}

