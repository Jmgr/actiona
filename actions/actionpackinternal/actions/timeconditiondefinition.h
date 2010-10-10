/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#ifndef TIMECONDITIONDEFINITION_H
#define TIMECONDITIONDEFINITION_H

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
			ActionTools::DateTimeParameterDefinition *date = new ActionTools::DateTimeParameterDefinition("date", tr("Date"), this);
			date->setTooltip(tr("The date to compare"));
			addElement(date);

			ActionTools::IfActionParameterDefinition *ifBefore = new ActionTools::IfActionParameterDefinition("ifBefore", tr("If before"), this);
			ifBefore->setTooltip(tr("What to do if the date is in the past"));
			addElement(ifBefore);

			ActionTools::IfActionParameterDefinition *ifNow = new ActionTools::IfActionParameterDefinition("ifNow", tr("If now"), this);
			ifNow->setTooltip(tr("What to do if the date is the current date"));
			addElement(ifNow);

			ActionTools::IfActionParameterDefinition *ifAfter = new ActionTools::IfActionParameterDefinition("ifAfter", tr("If after"), this);
			ifAfter->setTooltip(tr("What to do if the date is in the future"));
			ifAfter->setAllowWait(true);
			addElement(ifAfter);
		}

		QString name() const													{ return QObject::tr("Time condition"); }
		QString id() const														{ return "ActionTimeCondition"; }
		Flag flags() const														{ return ActionDefinition::flags() | Official; }
		QString description() const												{ return QObject::tr("Check the current date time and do some action"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new TimeConditionInstance(this); }
		Category category() const												{ return Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/timecondition.png"); }

	private:
		Q_DISABLE_COPY(TimeConditionDefinition)
	};
}

#endif // TIMECONDITIONDEFINITION_H
