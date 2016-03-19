/*
	Actiona
	Copyright (C) 2005-2016 Jonathan Mercier-Ganady

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

#ifndef NOOPDEFINITION_H
#define NOOPDEFINITION_H

#include "actiondefinition.h"
#include "noopinstance.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class NoopDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit NoopDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
		}

		QString name() const													{ return QObject::tr("No-op"); }
		QString id() const														{ return "ActionNoop"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Does nothing"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new NoopInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/noop.png"); }

	private:
		Q_DISABLE_COPY(NoopDefinition)
	};
}

#endif // NOOPDEFINITION_H
