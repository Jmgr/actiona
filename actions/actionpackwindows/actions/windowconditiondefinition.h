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
#include "windowconditioninstance.h"
#include "windowparameterdefinition.h"
#include "listparameterdefinition.h"
#include "ifactionparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WindowConditionDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit WindowConditionDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("WindowConditionInstance::conditions", WindowConditionInstance::conditions);

            auto &title = addParameter<ActionTools::WindowParameterDefinition>({QStringLiteral("title"), tr("Window title")});
            title.setTooltip(tr("The title of the window to find, you can use wildcards like * (any number of characters) or ? (one character) here"));

            auto &condition = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("condition"), tr("Condition")});
            condition.setTooltip(tr("The condition to wait for"));
            condition.setItems(WindowConditionInstance::conditions);
            condition.setDefaultValue(WindowConditionInstance::conditions.second.at(WindowConditionInstance::Exists));

            auto &ifTrue = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifTrue"), tr("If true")});
            ifTrue.setTooltip(tr("What to do if the condition is met"));

            auto &ifFalse = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifFalse"), tr("If false")});
            ifFalse.setTooltip(tr("What to do if the condition is not met"));
            ifFalse.setAllowWait(true);

            auto &position = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("position"), tr("Position")}, 1);
            position.setTooltip(tr("The position of the found window"));

            auto &size = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("size"), tr("Size")}, 1);
            size.setTooltip(tr("The size of the found window"));

            auto &xCoordinate = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("xCoordinate"), tr("X-coordinate")}, 2);
            xCoordinate.setTooltip(tr("The x-coordinate of the found window"));

            auto &yCoordinate = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("yCoordinate"), tr("Y-coordinate")}, 2);
            yCoordinate.setTooltip(tr("The y-coordinate of the found window"));

            auto &width = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("width"), tr("Width")}, 2);
            width.setTooltip(tr("The width of the found window"));

            auto &height = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("height"), tr("Height")}, 2);
            height.setTooltip(tr("The height of the found window"));

            auto &processId = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("processId"), tr("Process id")}, 1);
            processId.setTooltip(tr("The process id of the found window"));
		}

		QString name() const override													{ return QObject::tr("Window condition"); }
		QString id() const override														{ return QStringLiteral("ActionWindowCondition"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Checks for the state of a window"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new WindowConditionInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Windows; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/windowcondition.png")); }
        QStringList tabs() const override												{ return ActionDefinition::StandardTabs + QStringList{} << tr("Deprecated"); }

	private:
		Q_DISABLE_COPY(WindowConditionDefinition)
	};
}

