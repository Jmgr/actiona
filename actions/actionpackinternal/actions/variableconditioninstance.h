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

#ifndef VARIABLECONDITIONINSTANCE_H
#define VARIABLECONDITIONINSTANCE_H

#include "actioninstance.h"
#include "ifactionvalue.h"
#include "code/codeclass.h"
#include "code/rect.h"
#include "code/point.h"

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
            QScriptValue value = evaluateValue(ok, "value");
			ActionTools::IfActionValue ifEqual = evaluateIfAction(ok, "ifEqual");
			ActionTools::IfActionValue ifDifferent = evaluateIfAction(ok, "ifDifferent");

			if(!ok)
				return;

            QScriptValue variableValue = variable(variableName);

            if(!variableValue.isValid())
            {
                setCurrentParameter("variable");
                emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid variable"));

                return;
            }

            bool result;

            if(variableValue.isQObject())
            {
                QObject *variableObject = variableValue.toQObject();

                Code::Rect *rectObject = qobject_cast<Code::Rect*>(variableObject);
                Code::Point *pointObject = qobject_cast<Code::Point*>(value.toQObject());
                if(comparison == Contains && rectObject && pointObject)
                    result = rectObject->rect().contains(pointObject->point());
                else if(Code::CodeClass *object = qobject_cast<Code::CodeClass*>(variableObject))
                    result = object->equals(value);
            }
            else if(variableValue.isBool())
                result = compare(comparison, variableValue.toBool(), value.toBool());
            else if(variableValue.isNumber())
                result = compare(comparison, variableValue.toNumber(), value.toNumber());
            else if(variableValue.isString())
            {
                if(comparison == Contains)
                    result = variableValue.toString().contains(value.toString());
                else
                    result = compare(comparison, variableValue.toString(), value.toString());
            }

            QString action = (result ? ifEqual.action() : ifDifferent.action());
            const ActionTools::SubParameter &actionParameter = (result ? ifEqual.actionParameter() : ifDifferent.actionParameter());
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

        template<class T>
        static bool compare(Comparison comparison, const T &valueA, const T &valueB)
        {
            switch(comparison)
            {
            case Equal:
                return (valueA == valueB);
            case Different:
                return (valueA != valueB);
            case Inferior:
                return (valueA < valueB);
                break;
            case Superior:
                return (valueA > valueB);
                break;
            case InferiorEqual:
                return (valueA <= valueB);
                break;
            case SuperiorEqual:
                return (valueA >= valueB);
                break;
            default:
                return false;
            }
        }
	};
}

#endif // VARIABLECONDITIONINSTANCE_H
