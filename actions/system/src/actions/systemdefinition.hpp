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

#pragma once

#include "actiontools/actiondefinition.hpp"
#include "systeminstance.hpp"
#include "actiontools/listparameterdefinition.hpp"
#include "actiontools/groupdefinition.hpp"
#include "actiontools/booleanparameterdefinition.hpp"
#include "../systemsession.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class SystemDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit SystemDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("SystemInstance::operations", SystemInstance::operations);

            auto &operation = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("operation"), tr("Operation")});
            operation.setTooltip(tr("The operation to execute"));
            operation.setItems(SystemInstance::operations);
            operation.setDefaultValue(SystemInstance::operations.second.at(SystemInstance::Logout));

            auto &operationMode = addGroup();
            operationMode.setMasterList(operation);
            operationMode.setMasterValues({SystemInstance::operations.first.at(SystemInstance::Shutdown), SystemInstance::operations.first.at(SystemInstance::Restart),
                                           SystemInstance::operations.first.at(SystemInstance::Logout), SystemInstance::operations.first.at(SystemInstance::Suspend),
                                           SystemInstance::operations.first.at(SystemInstance::Hibernate)});

            auto &force = operationMode.addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("force"), tr("Force")});
            force.setTooltip(tr("Should the operation be forced"));
            force.setDefaultValue(QStringLiteral("false"));

			addException(SystemInstance::NotAvailable, tr("Not available"));
		}

		QString name() const override													{ return QObject::tr("System"); }
		QString id() const override														{ return QStringLiteral("ActionSystem"); }
		QString description() const override												{ return QObject::tr("Restart, stop the computer or logout the current user"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new SystemInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::System; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/system.png")); }

	private:
		Q_DISABLE_COPY(SystemDefinition)
	};
}

