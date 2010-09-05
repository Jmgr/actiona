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

#ifndef ACTIONKEYDEFINITION_H
#define ACTIONKEYDEFINITION_H

#include "actiondefinition.h"
#include "actionkeyinstance.h"
#include "keyparameterdefinition.h"
#include "listparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionKeyDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionKeyDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::KeyParameterDefinition *key = new ActionTools::KeyParameterDefinition("key", tr("Key"), this);
		key->setTooltip(tr("The key to simulate"));
		addElement(key);
		
		ActionTools::ListParameterDefinition *action = new ActionTools::ListParameterDefinition("action", tr("Action"), this);
		action->setTooltip(tr("The action to simulate"));
		action->setItems(ActionKeyInstance::actions);
		action->setDefaultValue(ActionKeyInstance::actions.second.at(ActionKeyInstance::PressReleaseAction));
		addElement(action);

		addException(ActionKeyInstance::FailedToSendInputException, tr("Send input failure"));
		addException(ActionKeyInstance::InvalidActionException, tr("Invalid action"));
	}

	QString name() const													{ return QObject::tr("Key"); }
	QString id() const														{ return "ActionKey"; }
	Flag flags() const														{ return ActionDefinition::flags() | Official; }
	QString description() const												{ return QObject::tr("Emulates a key press"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionKeyInstance(this); }
	Category category() const												{ return Device; }
	QPixmap icon() const													{ return QPixmap(":/actions/icons/key.png"); }
	bool requirementCheck(QStringList &missingRequirements) const			{ return requirementCheckXTest(missingRequirements); }

private:
	Q_DISABLE_COPY(ActionKeyDefinition)
};

#endif // ACTIONKEYDEFINITION_H
