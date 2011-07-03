/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef EXITDEFINITION_H
#define EXITDEFINITION_H

#include "actiondefinition.h"
#include "exitinstance.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ExitDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit ExitDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
		}

		QString name() const													{ return QObject::tr("Exit Actionaz", "action name: to exit"); }
		QString id() const														{ return "ActionExit"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Exit Actionaz", "action description: this exits"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new ExitInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/exit.png"); }

	private:
		Q_DISABLE_COPY(ExitDefinition)
	};
}

#endif // EXITDEFINITION_H
