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
#include "timeconditioninstance.hpp"
#include "actiontools/datetimeparameterdefinition.hpp"
#include "actiontools/ifactionparameterdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
    class TimeConditionDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit TimeConditionDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            auto &date = addParameter<ActionTools::DateTimeParameterDefinition>({QStringLiteral("date"), tr("Date")});
            date.setTooltip(tr("The date to compare"));

            auto &ifBefore = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifBefore"), tr("If in the past")});
            ifBefore.setTooltip(tr("What to do if the date is in the past"));

            auto &ifNow = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifNow"), tr("If now")});
            ifNow.setTooltip(tr("What to do if the date is the current date"));

            auto &ifAfter = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifAfter"), tr("If in the future")});
            ifAfter.setTooltip(tr("What to do if the date is in the future"));
            ifAfter.setAllowWait(true);
		}

		QString name() const override													{ return QObject::tr("Time condition"); }
		QString id() const override														{ return QStringLiteral("ActionTimeCondition"); }
		QString description() const override												{ return QObject::tr("Check the current date time and do some action"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new TimeConditionInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Internal; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/timecondition.png")); }

	private:
		Q_DISABLE_COPY(TimeConditionDefinition)
	};
}

