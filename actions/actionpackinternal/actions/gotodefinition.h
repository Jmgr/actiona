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

#ifndef GOTODEFINITION_H
#define GOTODEFINITION_H

#include "actiondefinition.h"
#include "gotoinstance.h"
#include "lineparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class GotoDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit GotoDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::LineParameterDefinition *line = new ActionTools::LineParameterDefinition("line", tr("Line"), this);
			line->setTooltip(tr("The line (or label) to go to"));
			addElement(line);
		}

		QString name() const													{ return QObject::tr("Goto"); }
		QString id() const														{ return "ActionGoto"; }
		Flag flags() const														{ return ActionDefinition::flags() | Official; }
		QString description() const												{ return QObject::tr("Go to a script line"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new GotoInstance(this); }
		Category category() const												{ return Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/goto.png"); }

	private:
		Q_DISABLE_COPY(GotoDefinition)
	};
}

#endif // GOTODEFINITION_H
