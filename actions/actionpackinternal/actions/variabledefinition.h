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

#ifndef VARIABLEDEFINITION_H
#define VARIABLEDEFINITION_H

#include "actiondefinition.h"
#include "actions/variableinstance.h"
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
			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition("variable", tr("Variable"), this);
			variable->setTooltip(tr("The variable name"));
			addElement(variable);

			ActionTools::TextParameterDefinition *value = new ActionTools::TextParameterDefinition("value", tr("Value"), this);
			value->setTooltip(tr("The variable's new value"));
			addElement(value);
		}

		QString name() const													{ return QObject::tr("Variable"); }
		QString id() const														{ return "ActionVariable"; }
		Flag flags() const														{ return ActionDefinition::flags() | Official; }
		QString description() const												{ return QObject::tr("Set the value of a variable"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new VariableInstance(this); }
		Category category() const												{ return Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/variable.png"); }

	private:
		Q_DISABLE_COPY(VariableDefinition)
	};
}

#endif // VARIABLEDEFINITION_H
