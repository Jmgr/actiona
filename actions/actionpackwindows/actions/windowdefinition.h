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
	class WindowDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit WindowDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("WindowInstance::actions", WindowInstance::actions);

            auto &title = addParameter<ActionTools::WindowParameterDefinition>({QStringLiteral("title"), tr("Window title")});
            title.setTooltip(tr("The title of the window to find, you can use wildcards like * (any number of characters) or ? (one character) here"));

            auto &action = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("action"), tr("Action")});
            action.setTooltip(tr("The action to perform"));
            action.setItems(WindowInstance::actions);
            action.setDefaultValue(WindowInstance::actions.second.at(WindowInstance::Close));

            auto &moveGroup = addGroup();
            moveGroup.setMasterList(action);
            moveGroup.setMasterValues({WindowInstance::actions.first.at(WindowInstance::Move)});

            auto &movePosition = moveGroup.addParameter<ActionTools::PositionParameterDefinition>({QStringLiteral("movePosition"), tr("Move position")});
            movePosition.setTooltip(tr("The position where to move the window"));

            auto &resizeGroup = addGroup();
            resizeGroup.setMasterList(action);
            resizeGroup.setMasterValues({WindowInstance::actions.first.at(WindowInstance::Resize)});

            auto &resizeWidth = resizeGroup.addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("resizeWidth"), tr("Resize width")});
            resizeWidth.setTooltip(tr("The new width of the window"));
            resizeWidth.setMinimum(0);
            resizeWidth.setMaximum(std::numeric_limits<int>::max());

            auto &resizeHeight = resizeGroup.addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("resizeHeight"), tr("Resize height")});
            resizeHeight.setTooltip(tr("The new height of the window"));
            resizeHeight.setMinimum(0);
            resizeHeight.setMaximum(std::numeric_limits<int>::max());

            auto &useBorders = resizeGroup.addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("useBorders"), tr("Use borders")});
            useBorders.setTooltip(tr("Should the border size be taken into account when resizing the window"));
            useBorders.setDefaultValue(QStringLiteral("true"));

			addException(WindowInstance::CannotFindWindowException, tr("Cannot find window"));
			addException(WindowInstance::ActionFailedException, tr("Action failed"));
		}

		QString name() const override													{ return QObject::tr("Window"); }
		QString id() const override														{ return QStringLiteral("ActionWindow"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Do some action on a window"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new WindowInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Windows; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/window.png")); }

	private:
		Q_DISABLE_COPY(WindowDefinition)
	};
}

