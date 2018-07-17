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

#pragma once

#include "actiondefinition.h"
#include "callprocedureinstance.h"
#include "procedureparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class CallProcedureDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit CallProcedureDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			auto &name = addParameter<ActionTools::ProcedureParameterDefinition>({QStringLiteral("name"), tr("Name")});
            name.setTooltip(tr("The name of the procedure to call"));
		}

		QString name() const override													{ return QObject::tr("Call procedure"); }
		QString id() const override														{ return QStringLiteral("ActionCallProcedure"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Calls a procedure"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new CallProcedureInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Procedures; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/callprocedure.png")); }

	private:
		Q_DISABLE_COPY(CallProcedureDefinition)
	};
}

