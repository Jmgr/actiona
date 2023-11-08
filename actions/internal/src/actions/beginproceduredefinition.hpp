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

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools/actiondefinition.hpp"
#include "beginprocedureinstance.hpp"
#include "actiontools/textparameterdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class BeginProcedureDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit BeginProcedureDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			auto &name = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("name"), tr("Name")});
            name.setTooltip(tr("The name of the procedure"));
            name.setTextCodeMode(ActionTools::TextParameterDefinition::TextOnly);
		}

		QString name() const override													{ return QObject::tr("Begin procedure"); }
		QString id() const override														{ return QStringLiteral("ActionBeginProcedure"); }
		QString description() const override												{ return QObject::tr("Declares a new procedure"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new BeginProcedureInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Procedures; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/beginprocedure.png")); }

	private:
		Q_DISABLE_COPY(BeginProcedureDefinition)
	};
}

