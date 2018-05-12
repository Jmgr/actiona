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
#include "windowinstance.h"
#include "windowparameterdefinition.h"
#include "listparameterdefinition.h"
#include "groupdefinition.h"
#include "positionparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "booleanparameterdefinition.h"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WindowDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit WindowDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("WindowInstance::actions", WindowInstance::actions);

			ActionTools::WindowParameterDefinition *title = new ActionTools::WindowParameterDefinition(ActionTools::Name(QStringLiteral("title"), tr("Window title")), this);
			title->setTooltip(tr("The title of the window to find, you can use wildcards like * (any number of characters) or ? (one character) here"));
			addElement(title);

			ActionTools::ListParameterDefinition *action = new ActionTools::ListParameterDefinition(ActionTools::Name(QStringLiteral("action"), tr("Action")), this);
			action->setTooltip(tr("The action to perform"));
			action->setItems(WindowInstance::actions);
			action->setDefaultValue(WindowInstance::actions.second.at(WindowInstance::Close));
			addElement(action);

			ActionTools::GroupDefinition *moveGroup = new ActionTools::GroupDefinition(this);
			moveGroup->setMasterList(action);
			moveGroup->setMasterValues(QStringList() << WindowInstance::actions.first.at(WindowInstance::Move));

			ActionTools::PositionParameterDefinition *movePosition = new ActionTools::PositionParameterDefinition(ActionTools::Name(QStringLiteral("movePosition"), tr("Move position")), this);
            movePosition->setTooltip(tr("The position where to move the window"));
			moveGroup->addMember(movePosition);

			addElement(moveGroup);

			ActionTools::GroupDefinition *resizeGroup = new ActionTools::GroupDefinition(this);
			resizeGroup->setMasterList(action);
			resizeGroup->setMasterValues(QStringList() << WindowInstance::actions.first.at(WindowInstance::Resize));

			ActionTools::NumberParameterDefinition *resizeWidth = new ActionTools::NumberParameterDefinition(ActionTools::Name(QStringLiteral("resizeWidth"), tr("Resize width")), this);
			resizeWidth->setTooltip(tr("The new width of the window"));
			resizeWidth->setMinimum(0);
			resizeWidth->setMaximum(std::numeric_limits<int>::max());
			resizeGroup->addMember(resizeWidth);

			ActionTools::NumberParameterDefinition *resizeHeight = new ActionTools::NumberParameterDefinition(ActionTools::Name(QStringLiteral("resizeHeight"), tr("Resize height")), this);
			resizeHeight->setTooltip(tr("The new height of the window"));
			resizeHeight->setMinimum(0);
			resizeHeight->setMaximum(std::numeric_limits<int>::max());
			resizeGroup->addMember(resizeHeight);

			ActionTools::BooleanParameterDefinition *useBorders = new ActionTools::BooleanParameterDefinition(ActionTools::Name(QStringLiteral("useBorders"), tr("Use borders")), this);
			useBorders->setTooltip(tr("Should the border size be taken into account when resizing the window"));
			useBorders->setDefaultValue(QStringLiteral("true"));
			resizeGroup->addMember(useBorders);

			addElement(resizeGroup);

			addException(WindowInstance::CannotFindWindowException, tr("Cannot find window"));
			addException(WindowInstance::ActionFailedException, tr("Action failed"));
		}

		QString name() const													{ return QObject::tr("Window"); }
		QString id() const														{ return QStringLiteral("ActionWindow"); }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Do some action on a window"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new WindowInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Windows; }
		QPixmap icon() const													{ return QPixmap(QStringLiteral(":/icons/window.png")); }

	private:
		Q_DISABLE_COPY(WindowDefinition)
	};
}

