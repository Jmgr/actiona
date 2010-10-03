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

#ifndef MOVECURSORDEFINITION_H
#define MOVECURSORDEFINITION_H

#include "actiondefinition.h"
#include "movecursorinstance.h"
#include "positionparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class MoveCursorDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT
	
	public:
		explicit MoveCursorDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::PositionParameterDefinition *position = new ActionTools::PositionParameterDefinition("position", tr("Position"), this);
			position->setTooltip(tr("The position where to move the cursor"));
			addElement(position);
		}
	
		QString name() const													{ return QObject::tr("Move cursor"); }
		QString id() const														{ return "ActionMoveCursor"; }
		Flag flags() const														{ return ActionDefinition::flags() | Official; }
		QString description() const												{ return QObject::tr("Move the mouse cursor"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new MoveCursorInstance(this); }
		Category category() const												{ return Device; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/movecursor.png"); }
		bool requirementCheck(QStringList &missingRequirements) const			{ return requirementCheckXTest(missingRequirements); }
	
	private:
		Q_DISABLE_COPY(MoveCursorDefinition)
	};
}

#endif // MOVECURSORDEFINITION_H
