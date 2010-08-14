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

#ifndef ACTIONEXITDEFINITION_H
#define ACTIONEXITDEFINITION_H

#include "actiondefinition.h"
#include "actionexitinstance.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionExitDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionExitDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
	}

	QString name() const													{ return QObject::tr("Exit"); }
	QString id() const														{ return "ActionExit"; }
	Flag flags() const														{ return ActionDefinition::flags() | Official; }
	QString description() const												{ return QObject::tr("Exit Actionaz"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionExitInstance(this); }
	Category category() const												{ return Internal; }
	QPixmap icon() const													{ return QPixmap(":/actions/icons/exit.png"); }

private:
	Q_DISABLE_COPY(ActionExitDefinition)
};

#endif // ACTIONEXITDEFINITION_H
