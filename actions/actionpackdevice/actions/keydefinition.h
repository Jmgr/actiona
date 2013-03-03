/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#ifndef KEYDEFINITION_H
#define KEYDEFINITION_H

#include "actiondefinition.h"
#include "keyinstance.h"
#include "keyparameterdefinition.h"
#include "listparameterdefinition.h"
#include "numberparameterdefinition.h"
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
	class KeyDefinition : public QObject, public ActionTools::ActionDefinition
	{
		Q_OBJECT
	
	public:
		explicit KeyDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("KeyInstance::actions", KeyInstance::actions);
			translateItems("KeyInstance::types", KeyInstance::types);

			ActionTools::KeyParameterDefinition *key = new ActionTools::KeyParameterDefinition(ActionTools::Name("key", tr("Key")), this);
			key->setTooltip(tr("The key to simulate"));
			addElement(key);
			
			ActionTools::ListParameterDefinition *action = new ActionTools::ListParameterDefinition(ActionTools::Name("action", tr("Action")), this);
			action->setTooltip(tr("The action to simulate"));
			action->setItems(KeyInstance::actions);
			action->setDefaultValue(KeyInstance::actions.second.at(KeyInstance::PressReleaseAction));
			addElement(action);

			ActionTools::GroupDefinition *pressAndReleaseGroup = new ActionTools::GroupDefinition(this);
			pressAndReleaseGroup->setMasterList(action);
			pressAndReleaseGroup->setMasterValues(QStringList() << KeyInstance::actions.first.at(KeyInstance::PressReleaseAction));

			ActionTools::NumberParameterDefinition *amount = new ActionTools::NumberParameterDefinition(ActionTools::Name("amount", tr("Amount")), this);
			amount->setTooltip(tr("The amount of key presses to simulate"));
			amount->setMinimum(1);
			amount->setMaximum(std::numeric_limits<int>::max());
			amount->setDefaultValue(1);
			pressAndReleaseGroup->addMember(amount);

			addElement(pressAndReleaseGroup);

			ActionTools::BooleanParameterDefinition *ctrl = new ActionTools::BooleanParameterDefinition(ActionTools::Name("ctrl", tr("Ctrl")), this);
			ctrl->setTooltip(tr("Should the Ctrl key be pressed"));
			addElement(ctrl);

			ActionTools::BooleanParameterDefinition *alt = new ActionTools::BooleanParameterDefinition(ActionTools::Name("alt", tr("Alt")), this);
			alt->setTooltip(tr("Should the Alt key be pressed"));
			addElement(alt);

			ActionTools::BooleanParameterDefinition *shift = new ActionTools::BooleanParameterDefinition(ActionTools::Name("shift", tr("Shift")), this);
			shift->setTooltip(tr("Should the Shift key be pressed"));
			addElement(shift);

		#ifdef Q_WS_WIN
			QString metaKeyName = tr("Windows");
		#else
			QString metaKeyName = tr("Meta");
		#endif

			ActionTools::BooleanParameterDefinition *meta = new ActionTools::BooleanParameterDefinition(ActionTools::Name("meta", metaKeyName), this);
			meta->setTooltip(tr("Should the %1 key be pressed").arg(metaKeyName));
			addElement(meta);

			ActionTools::ListParameterDefinition *type = new ActionTools::ListParameterDefinition(ActionTools::Name("type", tr("Type")), this);
			type->setTooltip(tr("The key type to use"));
			type->setItems(KeyInstance::types);
			type->setDefaultValue(KeyInstance::types.second.at(KeyInstance::Win32Type));
			type->setOperatingSystems(ActionTools::WorksOnWindows);
			addElement(type, 1);

			ActionTools::NumberParameterDefinition *pause = new ActionTools::NumberParameterDefinition(ActionTools::Name("pause", tr("Press/Release pause")), this);
			pause->setTooltip(tr("The pause duration between press and release"));
			pause->setMinimum(0);
			pause->setMaximum(std::numeric_limits<int>::max());
			pause->setDefaultValue(10);
			pause->setSuffix(tr(" ms", "milliseconds"));
			addElement(pause, 1);

			addException(KeyInstance::FailedToSendInputException, tr("Send input failure"));
			addException(KeyInstance::InvalidActionException, tr("Invalid action"));
		}

		QString name() const													{ return QObject::tr("Key"); }
		QString id() const														{ return "ActionKey"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Emulates a key press"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new KeyInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Device; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/key.png"); }
		bool requirementCheck(QStringList &missingRequirements) const			{ return requirementCheckXTest(missingRequirements); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(KeyDefinition)
	};
}

#endif // KEYDEFINITION_H
