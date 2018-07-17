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
#include "clickinstance.h"
#include "listparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "positionparameterdefinition.h"
#include "booleanparameterdefinition.h"
#include "groupdefinition.h"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ClickDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT
	
	public:
		explicit ClickDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("ClickInstance::buttons", ClickInstance::buttons);
			translateItems("ClickInstance::actions", ClickInstance::actions);

            auto &action = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("action"), tr("Action")});
            action.setTooltip(tr("The action to simulate"));
            action.setItems(ClickInstance::actions);
            action.setDefaultValue(ClickInstance::actions.second.at(ClickInstance::ClickAction));

            auto &button = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("button"), tr("Button")});
            button.setTooltip(tr("The button to simulate"));
            button.setItems(ClickInstance::buttons);
            button.setDefaultValue(ClickInstance::buttons.second.at(MouseDevice::LeftButton));

            auto &position = addParameter<ActionTools::PositionParameterDefinition>({QStringLiteral("position"), tr("Position")});
            position.setTooltip(tr("The screen position where to simulate a mouse click"));

            auto &restoreCursorPosition = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("restoreCursorPosition"), tr("Restore cursor position")});
            restoreCursorPosition.setTooltip(tr("Restore the cursor position after the action is finished"));

            auto &clickGroup = addGroup();
            clickGroup.setMasterList(action);
            clickGroup.setMasterValues({ClickInstance::actions.first.at(ClickInstance::ClickAction)});
	
            auto &amount = clickGroup.addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("amount"), tr("Amount")});
            amount.setTooltip(tr("The amount of clicks to simulate"));
            amount.setMinimum(1);
            amount.setMaximum(std::numeric_limits<int>::max());
            amount.setDefaultValue(QStringLiteral("1"));
			
            auto &positionOffset = addParameter<ActionTools::PositionParameterDefinition>({QStringLiteral("positionOffset"), tr("Offset")}, 1);
            positionOffset.setTooltip(tr("The offset to apply to the click position"));

			addException(ClickInstance::FailedToSendInputException, tr("Send input failure"));
			addException(ClickInstance::InvalidActionException, tr("Invalid action"));
		}
	
		QString name() const override													{ return QObject::tr("Click"); }
		QString id() const override														{ return QStringLiteral("ActionClick"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Emulates a mouse click"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new ClickInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Device; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/click.png")); }
		bool requirementCheck(QStringList &missingRequirements) const override			{ return requirementCheckXTest(missingRequirements); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }
	
	private:
		Q_DISABLE_COPY(ClickDefinition)
	};
}

