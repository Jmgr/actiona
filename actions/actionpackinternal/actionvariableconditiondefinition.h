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

#ifndef ACTIONVARIABLECONDITIONDEFINITION_H
#define ACTIONVARIABLECONDITIONDEFINITION_H

#include "actiondefinition.h"
#include "actionvariableconditioninstance.h"
#include "variableparameterdefinition.h"
#include "listparameterdefinition.h"
#include "textparameterdefinition.h"
#include "ifactionparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionVariableConditionDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionVariableConditionDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"variable",
																								tr("Variable"),
																								this);
		variable->setTooltip(tr("The variable to compare"));
		addElement(variable);

		ActionTools::ListParameterDefinition *comparison = new ActionTools::ListParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"comparison",
																								tr("Comparison"),
																								this);
		comparison->setTooltip(tr("The comparison"));
		comparison->setItems(ActionVariableConditionInstance::comparisons);
		comparison->setDefaultValue(ActionVariableConditionInstance::comparisons.second.at(ActionVariableConditionInstance::Equal));
		addElement(comparison);

		ActionTools::TextParameterDefinition *value = new ActionTools::TextParameterDefinition(ActionTools::ElementDefinition::INPUT,
																							   "value",
																							   tr("Value"),
																							   this);
		value->setTooltip(tr("The value"));
		addElement(value);

		ActionTools::IfActionParameterDefinition *ifEqual = new ActionTools::IfActionParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"ifEqual",
																								tr("If equal"),
																								this);
		ifEqual->setTooltip(tr("What to do if the values are equal"));
		addElement(ifEqual);

		ActionTools::IfActionParameterDefinition *ifDifferent = new ActionTools::IfActionParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"ifDifferent",
																								tr("If different"),
																								this);
		ifDifferent->setTooltip(tr("What to do if the values are different"));
		addElement(ifDifferent);
	}

	QString name() const													{ return QObject::tr("Variable condition"); }
	QString id() const														{ return "ActionVariableCondition"; }
	Flag flags() const														{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const												{ return QObject::tr("Check the value of a variable and do some action"); }
	Tools::Version version() const											{ return Tools::Version(0, 0, 1); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionVariableConditionInstance(this); }
	Status status() const													{ return Stable; }
	Category category() const												{ return Internal; }
	QPixmap icon() const													{ return QPixmap(":/icons/goto.png"); }

private:
	Q_DISABLE_COPY(ActionVariableConditionDefinition)
};

#endif // ACTIONVARIABLECONDITIONDEFINITION_H
