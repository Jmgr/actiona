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

#ifndef VARIABLEINSTANCE_H
#define VARIABLEINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"
#include "script.h"

namespace Actions
{
	class VariableInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Type)

	public:
		enum Type
		{
			String,
			Integer,
			Float
		};
		enum Exceptions
		{
			ConversionFailedException = ActionTools::ActionException::UserException
		};

		VariableInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)										{}

		static ActionTools::StringListPair types;

		void startExecution()
		{
			ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
			QString variable;
			QString value;
			Type type;

			if(!actionInstanceExecutionHelper.evaluateString(variable, "variable") ||
			   !actionInstanceExecutionHelper.evaluateString(value, "value") ||
			   !actionInstanceExecutionHelper.evaluateListElement(type, types, "type"))
				return;

			switch(type)
			{
			case String:
				{
					actionInstanceExecutionHelper.setVariable(variable, value);
				}
				break;
			case Integer:
				{
					bool ok;
					actionInstanceExecutionHelper.setVariable(variable, value.toInt(&ok));
					if(!ok)
					{
						emit executionException(ConversionFailedException, tr("Cannot evaluate the value as an integer"));
						return;
					}
				}
				break;
			case Float:
				{
					bool ok;
					actionInstanceExecutionHelper.setVariable(variable, value.toFloat(&ok));
					if(!ok)
					{
						emit executionException(ConversionFailedException, tr("Cannot evaluate the value as a floating number"));
						return;
					}
				}
				break;
			}

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(VariableInstance)
	};
}

#endif // VARIABLEINSTANCE_H
