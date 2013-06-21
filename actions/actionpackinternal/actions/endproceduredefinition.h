/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#ifndef ENDPROCEDUREDEFINITION_H
#define ENDPROCEDUREDEFINITION_H

#include "actiondefinition.h"
#include "endprocedureinstance.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class EndProcedureDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit EndProcedureDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
		}

		QString name() const													{ return QObject::tr("End procedure"); }
		QString id() const														{ return "ActionEndProcedure"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Ends a procedure"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new EndProcedureInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Procedures; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/endprocedure.png"); }

	private:
		Q_DISABLE_COPY(EndProcedureDefinition)
	};
}

#endif // ENDPROCEDUREDEFINITION_H
