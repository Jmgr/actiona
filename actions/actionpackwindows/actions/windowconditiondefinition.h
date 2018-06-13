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
	class WindowConditionDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit WindowConditionDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("WindowConditionInstance::conditions", WindowConditionInstance::conditions);

			ActionTools::WindowParameterDefinition *title = new ActionTools::WindowParameterDefinition(ActionTools::Name(QStringLiteral("title"), tr("Window title")), this);
			title->setTooltip(tr("The title of the window to find, you can use wildcards like * (any number of characters) or ? (one character) here"));
			addElement(title);

			ActionTools::ListParameterDefinition *condition = new ActionTools::ListParameterDefinition(ActionTools::Name(QStringLiteral("condition"), tr("Condition")), this);
			condition->setTooltip(tr("The condition to wait for"));
			condition->setItems(WindowConditionInstance::conditions);
			condition->setDefaultValue(WindowConditionInstance::conditions.second.at(WindowConditionInstance::Exists));
			addElement(condition);

			ActionTools::IfActionParameterDefinition *ifTrue = new ActionTools::IfActionParameterDefinition(ActionTools::Name(QStringLiteral("ifTrue"), tr("If true")), this);
			ifTrue->setTooltip(tr("What to do if the condition is met"));
			addElement(ifTrue);

			ActionTools::IfActionParameterDefinition *ifFalse = new ActionTools::IfActionParameterDefinition(ActionTools::Name(QStringLiteral("ifFalse"), tr("If false")), this);
			ifFalse->setTooltip(tr("What to do if the condition is not met"));
			ifFalse->setAllowWait(true);
			addElement(ifFalse);

			ActionTools::VariableParameterDefinition *position = new ActionTools::VariableParameterDefinition(ActionTools::Name(QStringLiteral("position"), tr("Position")), this);
			position->setTooltip(tr("The position of the found window"));
			addElement(position, 1);

			ActionTools::VariableParameterDefinition *size = new ActionTools::VariableParameterDefinition(ActionTools::Name(QStringLiteral("size"), tr("Size")), this);
			size->setTooltip(tr("The size of the found window"));
			addElement(size, 1);

			ActionTools::VariableParameterDefinition *xCoordinate = new ActionTools::VariableParameterDefinition(ActionTools::Name(QStringLiteral("xCoordinate"), tr("X-coordinate")), this);
			xCoordinate->setTooltip(tr("The x-coordinate of the found window"));
			addElement(xCoordinate, 2);

			ActionTools::VariableParameterDefinition *yCoordinate = new ActionTools::VariableParameterDefinition(ActionTools::Name(QStringLiteral("yCoordinate"), tr("Y-coordinate")), this);
			yCoordinate->setTooltip(tr("The y-coordinate of the found window"));
			addElement(yCoordinate, 2);

			ActionTools::VariableParameterDefinition *width = new ActionTools::VariableParameterDefinition(ActionTools::Name(QStringLiteral("width"), tr("Width")), this);
			width->setTooltip(tr("The width of the found window"));
			addElement(width, 2);

			ActionTools::VariableParameterDefinition *height = new ActionTools::VariableParameterDefinition(ActionTools::Name(QStringLiteral("height"), tr("Height")), this);
			height->setTooltip(tr("The height of the found window"));
			addElement(height, 2);

			ActionTools::VariableParameterDefinition *processId = new ActionTools::VariableParameterDefinition(ActionTools::Name(QStringLiteral("processId"), tr("Process id")), this);
			processId->setTooltip(tr("The process id of the found window"));
			addElement(processId, 1);
		}

		QString name() const override													{ return QObject::tr("Window condition"); }
		QString id() const override														{ return QStringLiteral("ActionWindowCondition"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Checks for the state of a window"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new WindowConditionInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Windows; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/windowcondition.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs + QStringList() << tr("Deprecated"); }

	private:
		Q_DISABLE_COPY(WindowConditionDefinition)
	};
}

