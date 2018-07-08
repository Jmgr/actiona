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
#include "keyboardkeyconditioninstance.h"
#include "keyboardkeyparameterdefinition.h"
#include "listparameterdefinition.h"
#include "ifactionparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
    class KeyboardKeyConditionDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
        explicit KeyboardKeyConditionDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            translateItems("KeyboardKeyConditionInstance::conditions", KeyboardKeyConditionInstance::conditions);

            auto &keys = addParameter<ActionTools::KeyboardKeyParameterDefinition>({QStringLiteral("keys"), tr("Keys")});
            keys.setTooltip(tr("The keys to check"));

            auto &condition = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("condition"), tr("Condition")});
            condition.setTooltip(tr("The condition to wait for"));
            condition.setItems(KeyboardKeyConditionInstance::conditions);
            condition.setDefaultValue(KeyboardKeyConditionInstance::conditions.second.at(KeyboardKeyConditionInstance::Pressed));

            auto &ifTrue = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifTrue"), tr("If true")});
            ifTrue.setTooltip(tr("What to do if the condition is met"));

            auto &ifFalse = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifFalse"), tr("If false")});
            ifFalse.setTooltip(tr("What to do if the condition is not met"));
            ifFalse.setAllowWait(true);
		}

        QString name() const override													{ return QObject::tr("Key condition"); }
        QString id() const override														{ return QStringLiteral("ActionKeyboardKeyCondition"); }
        ActionTools::Flag flags() const override										{ return ActionDefinition::flags() | ActionTools::Official; }
        QString description() const override                                            { return QObject::tr("Checks for the state of a key combination"); }
        ActionTools::ActionInstance *newActionInstance() const override					{ return new KeyboardKeyConditionInstance(this); }
        ActionTools::ActionCategory category() const override							{ return ActionTools::Device; }
        QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/keycondition.png")); }

	private:
        Q_DISABLE_COPY(KeyboardKeyConditionDefinition)
	};
}

