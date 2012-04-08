/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#ifndef VARIABLECONDITIONINSTANCE_H
#define VARIABLECONDITIONINSTANCE_H

#include "actioninstance.h"
#include "ifactionvalue.h"

namespace Actions
{
	class VariableConditionInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Comparison)

	public:
		enum Comparison
		{
			Equal,
			Different,
			Inferior,
			Superior,
			InferiorEqual,
			SuperiorEqual,
			Contains
		};

		VariableConditionInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)										{}

		void startExecution()
		{
			bool ok = true;

			QString variableName = evaluateVariable(ok, "variable");
			Comparison comparison = evaluateListElement<Comparison>(ok, comparisons, "comparison");
			QString value = evaluateString(ok, "value");
			ActionTools::IfActionValue ifEqual = evaluateIfAction(ok, "ifEqual");
			ActionTools::IfActionValue ifDifferent = evaluateIfAction(ok, "ifDifferent");

			if(!ok)
				return;

			bool conversionOk = true;
			QVariant variableValue = variable(variableName);
			float numberVariableValue = variableValue.toFloat(&conversionOk);
			float numberValue = value.toFloat(&conversionOk);
			bool equal;

			if(conversionOk)//Number comparison
			{
				switch(comparison)
				{
				case Equal:
					equal = (numberVariableValue == numberValue);
					break;
				case Different:
					equal = (numberVariableValue != numberValue);
					break;
				case Inferior:
					equal = (numberVariableValue < numberValue);
					break;
				case Superior:
					equal = (numberVariableValue > numberValue);
					break;
				case InferiorEqual:
					equal = (numberVariableValue <= numberValue);
					break;
				case SuperiorEqual:
					equal = (numberVariableValue >= numberValue);
					break;
				case Contains:
					equal = (variableValue.toString().contains(value));
					break;
				default:
					equal = false;
					break;
				}
			}
			else//String comparison
			{
				switch(comparison)
				{
				case Equal:
					equal = (variableValue.toString() == value);
					break;
				case Different:
					equal = (variableValue.toString() != value);
					break;
				case Inferior:
					equal = (variableValue.toString() < value);
					break;
				case Superior:
					equal = (variableValue.toString() > value);
					break;
				case InferiorEqual:
					equal = (variableValue.toString() <= value);
					break;
				case SuperiorEqual:
					equal = (variableValue.toString() >= value);
					break;
				case Contains:
					equal = (variableValue.toString().contains(value));
					break;
				default:
					equal = false;
					break;
				}
			}

			QString action = (equal ? ifEqual.action() : ifDifferent.action());
			const ActionTools::SubParameter &actionParameter = (equal ? ifEqual.actionParameter() : ifDifferent.actionParameter());
			QString line = evaluateSubParameter(ok, actionParameter);

			if(!ok)
				return;

			if(action == ActionTools::IfActionValue::GOTO)
				setNextLine(line);
			else if(action == ActionTools::IfActionValue::CALLPROCEDURE)
			{
				if(!callProcedure(line))
					return;
			}

			emit executionEnded();
		}

		static ActionTools::StringListPair comparisons;

	private:
		Q_DISABLE_COPY(VariableConditionInstance)
	};
}

#endif // VARIABLECONDITIONINSTANCE_H
