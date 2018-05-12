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
#include "cursorpathinstance.h"
#include "pointlistparameterdefinition.h"
#include "listparameterdefinition.h"
#include "positionparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class CursorPathDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit CursorPathDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            translateItems("CursorPathInstance::buttons", CursorPathInstance::buttons);

			ActionTools::PointListParameterDefinition *path = new ActionTools::PointListParameterDefinition(ActionTools::Name(QStringLiteral("path"), tr("Path")), this);
			path->setTooltip(tr("The path to follow"));
			addElement(path);

			ActionTools::ListParameterDefinition *button = new ActionTools::ListParameterDefinition(ActionTools::Name(QStringLiteral("button"), tr("Button")), this);
            button->setTooltip(tr("The button to simulate"));
            button->setItems(CursorPathInstance::buttons);
            button->setDefaultValue(CursorPathInstance::buttons.second.at(CursorPathInstance::NoButton));
            addElement(button);

			ActionTools::PositionParameterDefinition *positionOffset = new ActionTools::PositionParameterDefinition(ActionTools::Name(QStringLiteral("positionOffset"), tr("Offset")), this);
			positionOffset->setTooltip(tr("The offset to apply to the path"));
			addElement(positionOffset, 1);
		}

		QString name() const													{ return QObject::tr("Cursor path"); }
		QString id() const														{ return QStringLiteral("ActionCursorPath"); }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Move the mouse cursor on a path"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new CursorPathInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Device; }
		QPixmap icon() const													{ return QPixmap(QStringLiteral(":/actions/icons/movecursor.png")); }
		bool requirementCheck(QStringList &missingRequirements) const			{ return requirementCheckXTest(missingRequirements); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(CursorPathDefinition)
	};
}

