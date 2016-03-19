/*
	Actiona
	Copyright (C) 2005-2016 Jonathan Mercier-Ganady

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

#ifndef CLICKDEFINITION_H
#define CLICKDEFINITION_H

#include "actiondefinition.h"
#include "clickinstance.h"
#include "listparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "positionparameterdefinition.h"
#include "groupdefinition.h"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ClickDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT
	
	public:
		explicit ClickDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("ClickInstance::buttons", ClickInstance::buttons);
			translateItems("ClickInstance::actions", ClickInstance::actions);

			ActionTools::ListParameterDefinition *action = new ActionTools::ListParameterDefinition(ActionTools::Name("action", tr("Action")), this);
			action->setTooltip(tr("The action to simulate"));
			action->setItems(ClickInstance::actions);
			action->setDefaultValue(ClickInstance::actions.second.at(ClickInstance::ClickAction));
			addElement(action);
	
			ActionTools::ListParameterDefinition *button = new ActionTools::ListParameterDefinition(ActionTools::Name("button", tr("Button")), this);
			button->setTooltip(tr("The button to simulate"));
			button->setItems(ClickInstance::buttons);
			button->setDefaultValue(ClickInstance::buttons.second.at(MouseDevice::LeftButton));
			addElement(button);
	
			ActionTools::PositionParameterDefinition *position = new ActionTools::PositionParameterDefinition(ActionTools::Name("position", tr("Position")), this);
			position->setTooltip(tr("The screen position where to simulate a mouse click"));
			addElement(position);
	
			ActionTools::GroupDefinition *clickGroup = new ActionTools::GroupDefinition(this);
			clickGroup->setMasterList(action);
			clickGroup->setMasterValues(QStringList() << ClickInstance::actions.first.at(ClickInstance::ClickAction));
	
			ActionTools::NumberParameterDefinition *amount = new ActionTools::NumberParameterDefinition(ActionTools::Name("amount", tr("Amount")), this);
			amount->setTooltip(tr("The amount of clicks to simulate"));
			amount->setMinimum(1);
			amount->setMaximum(std::numeric_limits<int>::max());
			amount->setDefaultValue(1);
			clickGroup->addMember(amount);
	
			addElement(clickGroup);
			
			ActionTools::PositionParameterDefinition *positionOffset = new ActionTools::PositionParameterDefinition(ActionTools::Name("positionOffset", tr("Offset")), this);
			positionOffset->setTooltip(tr("The offset to apply to the click position"));
			addElement(positionOffset, 1);

			addException(ClickInstance::FailedToSendInputException, tr("Send input failure"));
			addException(ClickInstance::InvalidActionException, tr("Invalid action"));
		}
	
		QString name() const													{ return QObject::tr("Click"); }
		QString id() const														{ return "ActionClick"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Emulates a mouse click"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new ClickInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Device; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/click.png"); }
		bool requirementCheck(QStringList &missingRequirements) const			{ return requirementCheckXTest(missingRequirements); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }
	
	private:
		Q_DISABLE_COPY(ClickDefinition)
	};
}

#endif // CLICKDEFINITION_H
