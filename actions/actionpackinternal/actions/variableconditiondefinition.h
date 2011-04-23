/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef VARIABLECONDITIONDEFINITION_H
#define VARIABLECONDITIONDEFINITION_H

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
	class VariableConditionDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit VariableConditionDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("VariableConditionInstance::comparisons", VariableConditionInstance::comparisons);

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition("variable", tr("Variable"), this);
			variable->setCategory(ActionTools::ElementDefinition::INPUT);
			variable->setTooltip(tr("The variable to compare"));
			addElement(variable);

			ActionTools::ListParameterDefinition *comparison = new ActionTools::ListParameterDefinition("comparison", tr("Comparison"), this);
			comparison->setTooltip(tr("The comparison"));
			comparison->setItems(VariableConditionInstance::comparisons);
			comparison->setDefaultValue(VariableConditionInstance::comparisons.second.at(VariableConditionInstance::Equal));
			addElement(comparison);

			ActionTools::TextParameterDefinition *value = new ActionTools::TextParameterDefinition("value", tr("Value"), this);
			value->setTooltip(tr("The value"));
			addElement(value);

			ActionTools::IfActionParameterDefinition *ifEqual = new ActionTools::IfActionParameterDefinition("ifEqual", tr("If equal"), this);
			ifEqual->setTooltip(tr("What to do if the values are equal"));
			addElement(ifEqual);

			ActionTools::IfActionParameterDefinition *ifDifferent = new ActionTools::IfActionParameterDefinition("ifDifferent", tr("If different"), this);
			ifDifferent->setTooltip(tr("What to do if the values are different"));
			addElement(ifDifferent);
		}

		QString name() const													{ return QObject::tr("Variable condition"); }
		QString id() const														{ return "ActionVariableCondition"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Check the value of a variable and do some action"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new VariableConditionInstance(this); }
		ActionTools::ActionCategory category() const									{ return ActionTools::Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/variablecondition.png"); }

	private:
		Q_DISABLE_COPY(VariableConditionDefinition)
	};
}

#endif // VARIABLECONDITIONDEFINITION_H
