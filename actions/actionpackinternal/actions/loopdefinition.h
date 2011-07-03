/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef LOOPDEFINITION_H
#define LOOPDEFINITION_H

#include "actiondefinition.h"
#include "loopinstance.h"
#include "lineparameterdefinition.h"
#include "numberparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class LoopDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit LoopDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::LineParameterDefinition *line = new ActionTools::LineParameterDefinition("line", tr("Line"), this);
			line->setTooltip(tr("The line (or label) to go to"));
			addElement(line);

			ActionTools::NumberParameterDefinition *count = new ActionTools::NumberParameterDefinition("count", tr("Count"), this);
			count->setTooltip(tr("The amount of times (evaluated the first time)"));
			count->setMinimum(0);
			count->setMaximum(INT_MAX);
			addElement(count);
		}

		QString name() const													{ return QObject::tr("Loop"); }
		QString id() const														{ return "ActionLoop"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Go to a script line a specific number of times"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new LoopInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/loop.png"); }

	private:
		Q_DISABLE_COPY(LoopDefinition)
	};
}

#endif // LOOPDEFINITION_H
