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

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools/actioninstance.hpp"
#include "actiontools/ifactionvalue.hpp"
#include "actiontools/code/codeclass.hpp"
#include "actiontools/code/rect.hpp"
#include "actiontools/code/point.hpp"

namespace Actions
{
	class VariableConditionInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

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
        Q_ENUM(Comparison)

		VariableConditionInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent)										{}

        bool jsValueLessThan(const QJSValue &lhs, const QJSValue &rhs) {
            QJSEngine engine;
            QJSValue compareFunction = engine.evaluate(QStringLiteral("(function(a, b) { return a < b; })"));
            QJSValue result = compareFunction.call(QJSValueList() << lhs << rhs);

            return result.toBool();
        }

		void startExecution() override
		{
			bool ok = true;

			QString variableName = evaluateVariable(ok, QStringLiteral("variable"));
			auto comparison = evaluateListElement<Comparison>(ok, comparisons, QStringLiteral("comparison"));
			QJSValue value = evaluateValue(ok, QStringLiteral("value"));
			ActionTools::IfActionValue ifEqual = evaluateIfAction(ok, QStringLiteral("ifEqual"));
			ActionTools::IfActionValue ifDifferent = evaluateIfAction(ok, QStringLiteral("ifDifferent"));

			if(!ok)
				return;

            QJSValue variableValue = variable(variableName);

            if(variableValue.isUndefined())
            {
				setCurrentParameter(QStringLiteral("variable"));
                emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid variable"));

                return;
            }

            bool hasResult = false;
            bool result = false;

            if(comparison == Contains)
            {
                if(variableValue.isQObject())
                {
                    QObject *variableObject = variableValue.toQObject();

                    auto rectObject = qobject_cast<Code::Rect*>(variableObject);
                    auto pointObject = qobject_cast<Code::Point*>(value.toQObject());
                    if(rectObject && pointObject)
                    {
                        result = rectObject->rect().contains(pointObject->point());

                        hasResult = true;
                    }
                }
                else if(variableValue.isString())
                {
                    result = variableValue.toString().contains(value.toString());

                    hasResult = true;
                }
                else if(variableValue.isArray())
                {
                    int arrayLength = variableValue.property(QStringLiteral("length")).toInt();

                    result = false;
                    hasResult = true;

                    for(int arrayIndex = 0; arrayIndex < arrayLength; ++arrayIndex)
                    {
                        if(variableValue.property(arrayIndex).toString() == value.toString())
                        {
                            result = true;

                            break;
                        }
                    }
                }
            }

            if(!hasResult)
            {
                switch(comparison)
                {
                case Equal:
                    result = (variableValue.equals(value));
                    break;
                case Different:
                    result = (!variableValue.equals(value));
                    break;
                case Inferior:
                    result = (jsValueLessThan(variableValue, value));
                    break;
                case Superior:
                    result = (!jsValueLessThan(variableValue, value) && !variableValue.equals(value));
                    break;
                case InferiorEqual:
                    result = (jsValueLessThan(variableValue, value) || variableValue.equals(value));
                    break;
                case SuperiorEqual:
                    result = (!jsValueLessThan(variableValue, value));
                    break;
                default:
                    result = false;
                    break;
                }
            }

            QString action = (result ? ifEqual.action() : ifDifferent.action());
            const ActionTools::SubParameter &actionParameter = (result ? ifEqual.actionParameter() : ifDifferent.actionParameter());
			QString line = evaluateSubParameter(ok, actionParameter);

			if(!ok)
				return;

            bool stopScript = false;
			if(action == ActionTools::IfActionValue::GOTO)
				setNextLine(line);
			else if(action == ActionTools::IfActionValue::CALLPROCEDURE)
			{
				if(!callProcedure(line))
					return;
			}
            else if(action == ActionTools::IfActionValue::STOPEXECUTION)
            {
                stopScript = true;
            }

            executionEnded(stopScript);
		}

        static Tools::StringListPair comparisons;

	private:
		Q_DISABLE_COPY(VariableConditionInstance)
	};
}

