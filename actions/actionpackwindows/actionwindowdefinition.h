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

#ifndef ACTIONWINDOWDEFINITION_H
#define ACTIONWINDOWDEFINITION_H

#include "actiondefinition.h"
#include "actionwindowinstance.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"
#include "groupdefinition.h"
#include "positionparameterdefinition.h"
#include "numberparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionWindowDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionWindowDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::TextParameterDefinition *title = new ActionTools::TextParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"title",
																								 tr("Window title"),
																								 this);
		title->setTooltip(tr("The title of the window to find, you can use wildcards like * (any number of characters) or ? (one character) here"));
		addElement(title);
		
		ActionTools::ListParameterDefinition *action = new ActionTools::ListParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"action",
																								 tr("Action"),
																								 this);
		action->setTooltip(tr("The condition to wait for"));
		action->setItems(ActionWindowInstance::actions);
		action->setDefaultValue(ActionWindowInstance::actions.second.at(ActionWindowInstance::Close));
		addElement(action);
		
		ActionTools::GroupDefinition *moveGroup = new ActionTools::GroupDefinition(				ActionTools::ElementDefinition::INPUT,
																								"move",
																								tr("Move"),
																								this);
		moveGroup->setMasterList(action);
		moveGroup->setMasterValues(QStringList() << ActionWindowInstance::actions.first.at(ActionWindowInstance::Move));
		
		ActionTools::PositionParameterDefinition *movePosition = new ActionTools::PositionParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"movePosition",
																								 tr("Move position"),
																								 this);
		movePosition->setTooltip(tr("The position to move the window"));
		moveGroup->addMember(movePosition);
		
		addElement(moveGroup);
		
		ActionTools::GroupDefinition *resizeGroup = new ActionTools::GroupDefinition(			ActionTools::ElementDefinition::INPUT,
																								"resize",
																								tr("Resize"),
																								this);
		resizeGroup->setMasterList(action);
		resizeGroup->setMasterValues(QStringList() << ActionWindowInstance::actions.first.at(ActionWindowInstance::Resize));
		
		ActionTools::NumberParameterDefinition *resizeWidth = new ActionTools::NumberParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"resizeWidth",
																								 tr("Resize width"),
																								 this);
		resizeWidth->setTooltip(tr("The new width of the window"));
		resizeWidth->setMinimum(0);
		resizeWidth->setMaximum(INT_MAX);
		resizeGroup->addMember(resizeWidth);
		
		ActionTools::NumberParameterDefinition *resizeHeight = new ActionTools::NumberParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"resizeHeight",
																								 tr("Resize height"),
																								 this);
		resizeHeight->setTooltip(tr("The new height of the window"));
		resizeHeight->setMinimum(0);
		resizeHeight->setMaximum(INT_MAX);
		resizeGroup->addMember(resizeHeight);
		
		addElement(resizeGroup);
		
		addException(ActionWindowInstance::CannotFindWindowException, tr("Cannot find window"));
		addException(ActionWindowInstance::ActionFailedException, tr("Action failed"));
	}

	QString name() const													{ return QObject::tr("Window"); }
	QString id() const														{ return "ActionWindow"; }
	Flag flags() const														{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const												{ return QObject::tr("Do some action on a window"); }
	Tools::Version version() const											{ return Tools::Version(0, 0, 1); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionWindowInstance(this); }
	Status status() const													{ return Alpha; }
	Category category() const												{ return Windows; }
	QPixmap icon() const													{ return QPixmap(":/icons/msg.png"); }

private:
	Q_DISABLE_COPY(ActionWindowDefinition)
};

#endif // ACTIONWINDOWDEFINITION_H
