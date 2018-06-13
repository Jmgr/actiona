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
#include "timeconditioninstance.h"
#include "datetimeparameterdefinition.h"
#include "ifactionparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class TimeConditionDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit TimeConditionDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::DateTimeParameterDefinition *date = new ActionTools::DateTimeParameterDefinition(ActionTools::Name(QStringLiteral("date"), tr("Date")), this);
			date->setTooltip(tr("The date to compare"));
			addElement(date);

			ActionTools::IfActionParameterDefinition *ifBefore = new ActionTools::IfActionParameterDefinition(ActionTools::Name(QStringLiteral("ifBefore"), tr("If in the past")), this);
			ifBefore->setTooltip(tr("What to do if the date is in the past"));
			addElement(ifBefore);

			ActionTools::IfActionParameterDefinition *ifNow = new ActionTools::IfActionParameterDefinition(ActionTools::Name(QStringLiteral("ifNow"), tr("If now")), this);
			ifNow->setTooltip(tr("What to do if the date is the current date"));
			addElement(ifNow);

			ActionTools::IfActionParameterDefinition *ifAfter = new ActionTools::IfActionParameterDefinition(ActionTools::Name(QStringLiteral("ifAfter"), tr("If in the future")), this);
			ifAfter->setTooltip(tr("What to do if the date is in the future"));
			ifAfter->setAllowWait(true);
			addElement(ifAfter);
		}

		QString name() const override													{ return QObject::tr("Time condition"); }
		QString id() const override														{ return QStringLiteral("ActionTimeCondition"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Check the current date time and do some action"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new TimeConditionInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Internal; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/timecondition.png")); }

	private:
		Q_DISABLE_COPY(TimeConditionDefinition)
	};
}

