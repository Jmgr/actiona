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
#include "endprocedureinstance.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class EndProcedureDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit EndProcedureDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
		}

		QString name() const override													{ return QObject::tr("End procedure"); }
		QString id() const override														{ return QStringLiteral("ActionEndProcedure"); }
		QString description() const override												{ return QObject::tr("Ends a procedure"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new EndProcedureInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Procedures; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/endprocedure.png")); }

	private:
		Q_DISABLE_COPY(EndProcedureDefinition)
	};
}

