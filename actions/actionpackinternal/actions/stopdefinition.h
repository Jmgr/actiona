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

	Contact : jmgr@jmgr.info
*/

#ifndef STOPDEFINITION_H
#define STOPDEFINITION_H

#include "actiondefinition.h"
#include "stopinstance.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class StopDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit StopDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
		}

		QString name() const													{ return QObject::tr("Stop"); }
		QString id() const														{ return "ActionStop"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Stop the script execution"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new StopInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/stop.png"); }

	private:
		Q_DISABLE_COPY(StopDefinition)
	};
}

#endif // STOPDEFINITION_H
