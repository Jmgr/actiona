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

#ifndef VARIABLEDEFINITION_H
#define VARIABLEDEFINITION_H

#include "actiondefinition.h"
#include "variableinstance.h"
#include "variableparameterdefinition.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class VariableDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit VariableDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("VariableInstance::types", VariableInstance::types);

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition("variable", tr("Variable"), this);
			variable->setCategory(ActionTools::ElementDefinition::INPUT);
			variable->setTooltip(tr("The variable name"));
			addElement(variable);

			ActionTools::TextParameterDefinition *value = new ActionTools::TextParameterDefinition("value", tr("Value"), this);
			value->setTooltip(tr("The variable's new value"));
			addElement(value);

			ActionTools::ListParameterDefinition *type = new ActionTools::ListParameterDefinition("type", tr("Type"), this);
			type->setTooltip(tr("The variable type"));
			type->setItems(VariableInstance::types);
			type->setDefaultValue(VariableInstance::types.second.at(VariableInstance::String));
			addElement(type);

			addException(VariableInstance::ConversionFailedException, tr("Conversion failed"));
		}

		QString name() const													{ return QObject::tr("Variable"); }
		QString id() const														{ return "ActionVariable"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Set the value of a variable"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new VariableInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/variable.png"); }

	private:
		Q_DISABLE_COPY(VariableDefinition)
	};
}

#endif // VARIABLEDEFINITION_H
