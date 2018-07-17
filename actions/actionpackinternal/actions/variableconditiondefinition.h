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
#include "variableconditioninstance.h"
#include "variableparameterdefinition.h"
#include "listparameterdefinition.h"
#include "textparameterdefinition.h"
#include "ifactionparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class VariableConditionDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit VariableConditionDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("VariableConditionInstance::comparisons", VariableConditionInstance::comparisons);

            auto &variable = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("variable"), tr("Variable")});
            variable.setCategory(ActionTools::ElementDefinition::INPUT);
            variable.setTooltip(tr("The variable to compare"));

            auto &comparison = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("comparison"), tr("Comparison")});
            comparison.setTooltip(tr("The comparison"));
            comparison.setItems(VariableConditionInstance::comparisons);
            comparison.setDefaultValue(VariableConditionInstance::comparisons.second.at(VariableConditionInstance::Equal));

            auto &value = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("value"), tr("Value")});
            value.setTooltip(tr("The value"));

            auto &ifEqual = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifEqual"), tr("If true")});
            ifEqual.setTooltip(tr("What to do if the comparison result is true"));

            auto &ifDifferent = addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifDifferent"), tr("If false")});
            ifDifferent.setTooltip(tr("What to do if the comparison result is false"));
		}

		QString name() const override													{ return QObject::tr("Variable condition"); }
		QString id() const override														{ return QStringLiteral("ActionVariableCondition"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Check the value of a variable and do some action"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new VariableConditionInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Internal; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/variablecondition.png")); }

	private:
		Q_DISABLE_COPY(VariableConditionDefinition)
	};
}

