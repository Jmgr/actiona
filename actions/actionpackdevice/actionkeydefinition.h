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
		ActionTools::KeyParameterDefinition *key = new ActionTools::KeyParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"key",
																								tr("Key"),
																								this);
		key->setTooltip(tr("The key to simulate"));
		addElement(key);

		addException(ActionKeyInstance::FailedToSendInputException, tr("Send input failure"));
	}

	QString name() const													{ return QObject::tr("Key"); }
	QString id() const														{ return "ActionKey"; }
	Flag flags() const														{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const												{ return QObject::tr("Emulates a key press"); }
	Tools::Version version() const											{ return Tools::Version(0, 0, 1); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionKeyInstance(this); }
	Status status() const													{ return Alpha; }
	Category category() const												{ return Device; }
	QPixmap icon() const													{ return QPixmap(":/icons/code.png"); }

private:
	Q_DISABLE_COPY(ActionKeyDefinition)
};

#endif // ACTIONKEYDEFINITION_H
