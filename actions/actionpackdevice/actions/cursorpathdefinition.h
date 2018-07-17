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
	class CursorPathDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit CursorPathDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            translateItems("CursorPathInstance::buttons", CursorPathInstance::buttons);

			auto &path = addParameter<ActionTools::PointListParameterDefinition>({QStringLiteral("path"), tr("Path")});
            path.setTooltip(tr("The path to follow"));

			auto &button = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("button"), tr("Button")});
            button.setTooltip(tr("The button to simulate"));
            button.setItems(CursorPathInstance::buttons);
            button.setDefaultValue(CursorPathInstance::buttons.second.at(CursorPathInstance::NoButton));

            auto &positionOffset = addParameter<ActionTools::PositionParameterDefinition>({QStringLiteral("positionOffset"), tr("Offset")}, 1);
            positionOffset.setTooltip(tr("The offset to apply to the path"));
		}

		QString name() const override													{ return QObject::tr("Cursor path"); }
		QString id() const override														{ return QStringLiteral("ActionCursorPath"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Move the mouse cursor on a path"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new CursorPathInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Device; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/movecursor.png")); }
		bool requirementCheck(QStringList &missingRequirements) const override			{ return requirementCheckXTest(missingRequirements); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(CursorPathDefinition)
	};
}

