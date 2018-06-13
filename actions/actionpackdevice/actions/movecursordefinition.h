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
			ActionTools::PositionParameterDefinition *position = new ActionTools::PositionParameterDefinition(ActionTools::Name(QStringLiteral("position"), tr("Position")), this);
			position->setTooltip(tr("The position where to move the cursor"));
			addElement(position);

			ActionTools::PositionParameterDefinition *positionOffset = new ActionTools::PositionParameterDefinition(ActionTools::Name(QStringLiteral("positionOffset"), tr("Offset")), this);
			positionOffset->setTooltip(tr("The offset to apply to the cursor movement"));
			addElement(positionOffset, 1);
		}
	
		QString name() const override													{ return QObject::tr("Move cursor"); }
		QString id() const override														{ return QStringLiteral("ActionMoveCursor"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Move the mouse cursor"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new MoveCursorInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Device; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/movecursor.png")); }
		bool requirementCheck(QStringList &missingRequirements) const override			{ return requirementCheckXTest(missingRequirements); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(MoveCursorDefinition)
	};
}

