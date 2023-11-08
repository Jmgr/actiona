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

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools/actiondefinition.hpp"
#include "keyinstance.hpp"
#include "actiontools/keyparameterdefinition.hpp"
#include "actiontools/listparameterdefinition.hpp"
#include "actiontools/numberparameterdefinition.hpp"
#include "actiontools/booleanparameterdefinition.hpp"
#include "actiontools/groupdefinition.hpp"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class KeyDefinition : public ActionTools::ActionDefinition
	{
		Q_OBJECT
	
	public:
		explicit KeyDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("KeyInstance::actions", KeyInstance::actions);
			translateItems("KeyInstance::types", KeyInstance::types);

			auto &key = addParameter<ActionTools::KeyParameterDefinition>({QStringLiteral("key"), tr("Key")});
            key.setTooltip(tr("The key to simulate"));

			auto &action = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("action"), tr("Action")});
            action.setTooltip(tr("The action to simulate"));
            action.setItems(KeyInstance::actions);
            action.setDefaultValue(KeyInstance::actions.second.at(KeyInstance::PressReleaseAction));

            auto &pressAndReleaseGroup = addGroup();
            pressAndReleaseGroup.setMasterList(action);
            pressAndReleaseGroup.setMasterValues({KeyInstance::actions.first.at(KeyInstance::PressReleaseAction)});

            auto &amount = pressAndReleaseGroup.addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("amount"), tr("Amount")});
            amount.setTooltip(tr("The amount of key presses to simulate"));
            amount.setMinimum(1);
            amount.setMaximum(std::numeric_limits<int>::max());
            amount.setDefaultValue(QStringLiteral("1"));

			auto &ctrl = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("ctrl"), tr("Ctrl")});
            ctrl.setTooltip(tr("Should the Ctrl key be pressed"));

			auto &alt = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("alt"), tr("Alt")});
            alt.setTooltip(tr("Should the Alt key be pressed"));

			auto &shift = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("shift"), tr("Shift")});
            shift.setTooltip(tr("Should the Shift key be pressed"));

		#ifdef Q_OS_WIN
			QString metaKeyName = tr("Windows");
		#else
			QString metaKeyName = tr("Meta");
		#endif

            auto &meta = addParameter<ActionTools::BooleanParameterDefinition>({QStringLiteral("meta"), metaKeyName});
            meta.setTooltip(tr("Should the %1 key be pressed").arg(metaKeyName));

            auto &type = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("type"), tr("Type")}, 1);
            type.setTooltip(tr("The key type to use"));
            type.setItems(KeyInstance::types);
            type.setDefaultValue(KeyInstance::types.second.at(KeyInstance::Win32Type));
            type.setOperatingSystems(ActionTools::WorksOnWindows);

            auto &pause = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("pause"), tr("Press/Release pause")}, 1);
            pause.setTooltip(tr("The pause duration between press and release"));
            pause.setMinimum(0);
            pause.setMaximum(std::numeric_limits<int>::max());
            pause.setDefaultValue(QStringLiteral("10"));
            pause.setSuffix(tr(" ms", "milliseconds"));

			addException(KeyInstance::FailedToSendInputException, tr("Send input failure"));
			addException(KeyInstance::InvalidActionException, tr("Invalid action"));
		}

		QString name() const override													{ return QObject::tr("Key"); }
		QString id() const override														{ return QStringLiteral("ActionKey"); }
		QString description() const override												{ return QObject::tr("Emulates a key press"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new KeyInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Device; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/key.png")); }
		bool requirementCheck(QStringList &missingRequirements) const override			{ return requirementCheckXTest(missingRequirements); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(KeyDefinition)
	};
}

