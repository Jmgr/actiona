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

#ifndef SYSTEMDEFINITION_H
#define SYSTEMDEFINITION_H

#include "actiondefinition.h"
#include "systeminstance.h"
#include "listparameterdefinition.h"

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
			translateItems("SystemInstance::modes", SystemInstance::modes);

			ActionTools::ListParameterDefinition *operation = new ActionTools::ListParameterDefinition("operation", tr("Operation"), this);
			operation->setTooltip(tr("The operation to execute"));
			operation->setItems(SystemInstance::operations);
			operation->setDefaultValue(SystemInstance::operations.second.at(SystemInstance::Logout));
			addElement(operation);

			ActionTools::ListParameterDefinition *mode = new ActionTools::ListParameterDefinition("mode", tr("Mode"), this);
			mode->setTooltip(tr("The operation mode"));
			mode->setItems(SystemInstance::modes);
			mode->setDefaultValue(SystemInstance::modes.second.at(SystemInstance::Normal));
			addElement(mode);

			addException(SystemInstance::NotEnoughRightsException, tr("Not enough rights"));
			addException(SystemInstance::NotAvailable, tr("Not available"));
		}

		QString name() const													{ return QObject::tr("System"); }
		QString id() const														{ return "ActionSystem"; }
		Flag flags() const														{ return ActionDefinition::flags() | Official; }
		QString description() const												{ return QObject::tr("Restart, stop the computer or logout the current user"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new SystemInstance(this); }
		Category category() const												{ return System; }
		QPixmap icon() const													{ return QPixmap(":/icons/system.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(SystemDefinition)
	};
}

#endif // SYSTEMDEFINITION_H
