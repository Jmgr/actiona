/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#ifndef CALLPROCEDUREDEFINITION_H
#define CALLPROCEDUREDEFINITION_H

#include "actiondefinition.h"
#include "callprocedureinstance.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class CallProcedureDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit CallProcedureDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			//TODO: Make a new parameter definition with a combobox of procedure names
			ActionTools::TextParameterDefinition *name = new ActionTools::TextParameterDefinition(ActionTools::Name("name", tr("Name")), this);
			name->setTooltip(tr("The name of the procedure to call"));
			addElement(name);
		}

		QString name() const													{ return QObject::tr("Call procedure"); }
		QString id() const														{ return "ActionCallProcedure"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Calls a procedure"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new CallProcedureInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Procedures; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/noop.png"); }

	private:
		Q_DISABLE_COPY(CallProcedureDefinition)
	};
}

#endif // CALLPROCEDUREDEFINITION_H
