/*
	Actiona
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef BEGINPROCEDUREDEFINITION_H
#define BEGINPROCEDUREDEFINITION_H

#include "actiondefinition.h"
#include "beginprocedureinstance.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class BeginProcedureDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit BeginProcedureDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::TextParameterDefinition *name = new ActionTools::TextParameterDefinition(ActionTools::Name("name", tr("Name")), this);
			name->setTooltip(tr("The name of the procedure"));
			name->setTextCodeMode(ActionTools::TextParameterDefinition::TextOnly);
			addElement(name);
		}

		QString name() const													{ return QObject::tr("Begin procedure"); }
		QString id() const														{ return "ActionBeginProcedure"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Declares a new procedure"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new BeginProcedureInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Procedures; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/beginprocedure.png"); }

	private:
		Q_DISABLE_COPY(BeginProcedureDefinition)
	};
}

#endif // BEGINPROCEDUREDEFINITION_H
