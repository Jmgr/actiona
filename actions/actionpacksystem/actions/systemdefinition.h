/*
	Actiona
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef SYSTEMDEFINITION_H
#define SYSTEMDEFINITION_H

#include "actiondefinition.h"
#include "systeminstance.h"
#include "listparameterdefinition.h"
#include "groupdefinition.h"
#include "booleanparameterdefinition.h"
#include "../systemsession.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class SystemDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit SystemDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("SystemInstance::operations", SystemInstance::operations);

			ActionTools::ListParameterDefinition *operation = new ActionTools::ListParameterDefinition(ActionTools::Name("operation", tr("Operation")), this);
			operation->setTooltip(tr("The operation to execute"));
			operation->setItems(SystemInstance::operations);
			operation->setDefaultValue(SystemInstance::operations.second.at(SystemInstance::Logout));
			addElement(operation);

			ActionTools::GroupDefinition *operationMode = new ActionTools::GroupDefinition(this);
			operationMode->setMasterList(operation);
			operationMode->setMasterValues(QStringList()
										   << SystemInstance::operations.first.at(SystemInstance::Shutdown)
										   << SystemInstance::operations.first.at(SystemInstance::Restart)
										   << SystemInstance::operations.first.at(SystemInstance::Logout)
										   << SystemInstance::operations.first.at(SystemInstance::Suspend)
										   << SystemInstance::operations.first.at(SystemInstance::Hibernate));

			ActionTools::BooleanParameterDefinition *force = new ActionTools::BooleanParameterDefinition(ActionTools::Name("force", tr("Force")), this);
			force->setTooltip(tr("Should the operation be forced"));
			force->setDefaultValue(false);
			operationMode->addMember(force);

			addElement(operationMode);

			addException(SystemInstance::NotAvailable, tr("Not available"));
		}

		QString name() const													{ return QObject::tr("System"); }
		QString id() const														{ return "ActionSystem"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Restart, stop the computer or logout the current user"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new SystemInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::System; }
		QPixmap icon() const													{ return QPixmap(":/icons/system.png"); }

	private:
		Q_DISABLE_COPY(SystemDefinition)
	};
}

#endif // SYSTEMDEFINITION_H
