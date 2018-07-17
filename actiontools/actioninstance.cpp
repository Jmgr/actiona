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

#include "actioninstance.h"
#include "actiondefinition.h"
#include "actionfactory.h"
#include "elementdefinition.h"
#include "parameterdefinition.h"
#include "groupdefinition.h"
#include "script.h"
#include "code/point.h"
#include "code/color.h"
#include "code/image.h"

#include <QDateTime>
#include <QScriptValueIterator>
#include <QApplication>
#include <QDesktopWidget>

namespace ActionTools
{
	bool ActionInstanceData::operator==(const ActionInstanceData &other) const
	{
		return (parametersData == other.parametersData &&
				definition == other.definition &&
				comment == other.comment &&
				label == other.label &&
				color == other.color &&
				enabled == other.enabled &&
				selected == other.selected &&
				exceptionActionInstances == other.exceptionActionInstances &&
				pauseBefore == other.pauseBefore &&
				pauseAfter == other.pauseAfter &&
                timeout == other.timeout);
	}


	const QRegExp ActionInstance::NumericalIndex(QStringLiteral("(\\d+)"));
	const QRegExp ActionInstance::NameRegExp(QStringLiteral("^[A-Za-z_][A-Za-z0-9_]*$"), Qt::CaseSensitive, QRegExp::RegExp2);
	const QRegExp ActionInstance::VariableRegExp(QStringLiteral("\\$([A-Za-z_][A-Za-z0-9_]*)"), Qt::CaseSensitive, QRegExp::RegExp2);
	qint64 ActionInstance::mCurrentRuntimeId = 0;

	ActionInstance::ActionInstance(const ActionDefinition *definition, QObject *parent)
		: QObject(parent),
		  mRuntimeId(mCurrentRuntimeId),
		  d(new ActionInstanceData())
    {
		d->definition = definition;
		++mCurrentRuntimeId;

		//Set the default values
		if(definition)
		{
            for(ElementDefinition *element: definition->elements())
				element->setDefaultValues(this);

			//Set the default exception action
			for(int i = 0; i < ActionTools::ActionException::ExceptionCount; ++i)
			{
				setExceptionActionInstance(static_cast<ActionTools::ActionException::Exception>(i),
										   ActionTools::ActionException::ExceptionActionInstance(ActionTools::ActionException::ExceptionDefaultAction[i], QString()));
			}
		}
	}

	ActionInstance::ActionInstance(const ActionInstance &other)
		: QObject(),
		  mRuntimeId(mCurrentRuntimeId),
		  d(other.d)
	{
		++mCurrentRuntimeId;
	}

    bool ActionInstance::callProcedure(const QString &procedureName)
    {
        //Search for the corresponding ActionBeginProcedure action
        int beginProcedureLine = script()->findProcedure(procedureName);
        if(beginProcedureLine == -1)
        {
            emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Unable to find any procedure named \"%1\"").arg(procedureName));

            return false;
        }

        setNextLine(beginProcedureLine + 2);//Lines start at 1

        script()->addProcedureCall(scriptLine());

        return true;
    }

    void ActionInstance::doStartExecution()
    {
        ++d->executionCounter;

        d->executionTimer.start();

        startExecution();
    }

    void ActionInstance::doStopExecution()
    {
        stopExecution();
    }

    void ActionInstance::doPauseExecution()
    {
        pauseExecution();

        d->executionDuration += d->executionTimer.elapsed();
    }

    void ActionInstance::doResumeExecution()
    {
        d->executionTimer.start();

        resumeExecution();
    }

    void ActionInstance::copyActionDataFrom(const ActionInstance &other)
    {
        setComment(other.comment());
        setLabel(other.label());
        setParametersData(other.parametersData());
		setColor(other.color());
		setEnabled(other.isEnabled());
		setSelected(other.isSelected());
		setExceptionActionInstances(other.exceptionActionInstances());
		setPauseBefore(other.pauseBefore());
		setPauseAfter(other.pauseAfter());
        setTimeout(other.timeout());
    }

    QString ActionInstance::convertToVariableName(const QString &input)
    {
        QString back = input;

        for(int i = 0; i < back.size(); ++i)
        {
			if(back[i] >= QLatin1Char('a') && back[i] <= QLatin1Char('z'))
                continue;
			if(back[i] >= QLatin1Char('A') && back[i] <= QLatin1Char('Z'))
                continue;
			if(i > 0 && back[i] >= QLatin1Char('0') && back[i] <= QLatin1Char('9'))
                continue;

			back[i] = QLatin1Char('_');
        }

        return back;
    }

    QSet<QString> ActionInstance::findVariables(const QString &input, bool code)
    {
        QSet<QString> back;

        if(code)
        {
			for(const QString &codeLine: input.split(QRegExp(QStringLiteral("[\n\r;]")), QString::SkipEmptyParts))
            {
                int position = 0;

                while((position = Script::CodeVariableDeclarationRegExp.indexIn(codeLine, position)) != -1)
                {
                    QString foundVariableName = Script::CodeVariableDeclarationRegExp.cap(1);

                    position += Script::CodeVariableDeclarationRegExp.cap(1).length();

                    if(!foundVariableName.isEmpty())
                        back << foundVariableName;
                }
            }
        }
        else
        {
            int position = 0;

            while((position = ActionInstance::VariableRegExp.indexIn(input, position)) != -1)
            {
                QString foundVariableName = ActionInstance::VariableRegExp.cap(1);

                position += ActionInstance::VariableRegExp.cap(0).length();

                if(!foundVariableName.isEmpty())
                    back << foundVariableName;
            }
        }

        return back;
    }

    QScriptValue ActionInstance::evaluateValue(bool &ok,
                                               const QString &parameterName,
                                               const QString &subParameterName)
    {
        if(!ok)
            return QScriptValue();

        const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
        QScriptValue result;

        if(subParameter.isCode())
            result = evaluateCode(ok, subParameter);
        else
            result = evaluateText(ok, subParameter);

        if(!ok)
            return QScriptValue();

        return result;
    }

	QString ActionInstance::evaluateString(bool &ok,
										const QString &parameterName,
										const QString &subParameterName)
	{
		if(!ok)
			return QString();

		const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
		QString result;

		if(subParameter.isCode())
            result = evaluateCode(ok, subParameter).toString();
		else
			result = evaluateText(ok, subParameter);

		if(!ok)
			return QString();

        return result;
    }

    QImage ActionInstance::evaluateImage(bool &ok, const QString &parameterName, const QString &subParameterName)
    {
        if(!ok)
            return QImage();

        const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
        QString filename;

        if(subParameter.isCode())
        {
            QScriptValue evaluationResult = evaluateCode(ok, subParameter);
            if(auto codeImage = qobject_cast<Code::Image*>(evaluationResult.toQObject()))
                return codeImage->image();

            if(!evaluationResult.isString())
            {
                ok = false;

                emit executionException(ActionException::InvalidParameterException, tr("Invalid image."));

                return QImage();
            }

            filename = evaluationResult.toString();
        }
        else
            filename = evaluateText(ok, subParameter);

        if(!ok || filename.isEmpty())
            return QImage();

        QImage image(filename);

        if(!image.isNull())
            return image;

        ok = false;

        emit executionException(ActionException::InvalidParameterException, tr("Unable to load image: %1").arg(filename));

        return QImage();
    }

	QString ActionInstance::evaluateVariable(bool &ok,
										const QString &parameterName,
										const QString &subParameterName)
	{
		QString result = evaluateString(ok, parameterName, subParameterName);

		if(!ok)
			return QString();

        if(!result.isEmpty() && !NameRegExp.exactMatch(result))
		{
			ok = false;

            emit executionException(ActionException::InvalidParameterException, tr("A variable name can only contain alphanumeric characters and cannot start with a digit."));

			return QString();
		}

		return result;
	}

	QString ActionInstance::evaluateVariableArray(bool &ok, const QScriptValue &scriptValue)
	{
		QString result;

		QScriptValueIterator it(scriptValue);

		if(scriptValue.isArray())
		{
			int lastIndex = -1;
			result = QStringLiteral("[");

			while (it.hasNext()) {
				it.next();

				if (it.flags() & QScriptValue::SkipInEnumeration)
					continue;

				QScriptValue nextScriptValue = it.value();
				//is it an array ?
				if(nextScriptValue.isArray())
					result += evaluateVariableArray(ok, nextScriptValue);
				else
                    if(NumericalIndex.exactMatch(it.name())) //it.name : numerical only ?
					{
						int newIndex = it.name().toInt();
						if( newIndex > lastIndex+1)
						{
							//insert some commas
							for(lastIndex++ ; lastIndex < newIndex; lastIndex++ )
								result += QStringLiteral(",");
						}
						lastIndex = newIndex;
						result += it.value().toString();
					}
					else
						result += it.name().append(QStringLiteral("=")).append(it.value().toString());

				result += QStringLiteral(",");
			}

			if(result == QLatin1String("["))
				result += QStringLiteral("]");
			else
				result[result.lastIndexOf(QStringLiteral(","))] = QLatin1Char(']');
		}
		else
			result = it.value().toString();

		return result;
	}

	int ActionInstance::evaluateInteger(bool &ok,
										  const QString &parameterName,
										  const QString &subParameterName)
	{
		QString result = evaluateString(ok, parameterName, subParameterName);

		if(!ok || result.isEmpty())
			return 0;

		int intResult = result.toInt(&ok);

		if(!ok)
		{
			ok = false;

            emit executionException(ActionException::InvalidParameterException, tr("Integer value expected."));

			return 0;
		}

		return intResult;
	}

	bool ActionInstance::evaluateBoolean(bool &ok,
										  const QString &parameterName,
										  const QString &subParameterName)
	{
		QVariant result = evaluateString(ok, parameterName, subParameterName);

		if(!ok)
			return false;

		return result.toBool();
	}

	double ActionInstance::evaluateDouble(bool &ok,
										const QString &parameterName,
										const QString &subParameterName)
	{
		QString result = evaluateString(ok, parameterName, subParameterName);

		if(!ok || result.isEmpty())
			return 0.0;

		double doubleResult = result.toDouble(&ok);

		if(!ok)
		{
			ok = false;

            emit executionException(ActionException::InvalidParameterException, tr("Decimal value expected."));

			return 0.0;
		}

		return doubleResult;
	}

	IfActionValue ActionInstance::evaluateIfAction(bool &ok,
										const QString &parameterName)
	{
		QString action = evaluateString(ok, parameterName, QStringLiteral("action"));

		if(!ok)
			return IfActionValue();

		return IfActionValue(action, subParameter(parameterName, QStringLiteral("line")));
	}

	QString ActionInstance::evaluateSubParameter(bool &ok,
							  const SubParameter &subParameter)
	{
        if(!ok || subParameter.value().isEmpty())
			return QString();

		QString result;

		if(subParameter.isCode())
			result = evaluateCode(ok, subParameter).toString();
		else
			result = evaluateText(ok, subParameter);

		if(!ok)
			return QString();

		return result;
    }

    QString ActionInstance::evaluateEditableListElement(bool &ok, const Tools::StringListPair &listElements, const QString &parameterName, const QString &subParameterName)
    {
        if(!ok)
            return QString();

        const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
        QString result;

        if(subParameter.isCode())
            result = evaluateCode(ok, subParameter).toString();
        else
            result = evaluateText(ok, subParameter);

        if(!ok)
            return QString();

        //Search in the translated items
        for(int i=0;i<listElements.second.size();++i)
        {
            if(listElements.second.at(i) == result)
                return listElements.first.at(i);
        }

        if(result.isEmpty())
        {
            ok = false;

            setCurrentParameter(parameterName, subParameterName);

            emit executionException(ActionException::InvalidParameterException, tr("Please choose a value for this field."));

            return QString();
        }

        return result;
    }

    void computePercentPosition(QPointF &point, const SubParameter &unitSubParameter)
    {
        if(unitSubParameter.value().toInt() == 1)//Percents
        {
            QRect screenGeometry = QApplication::desktop()->screenGeometry();

            point.setX((point.x() * screenGeometry.width()) / 100.0f);
            point.setY((point.y() * screenGeometry.height()) / 100.0f);
        }
    }

    QPoint ActionInstance::evaluatePoint(bool &ok,
                                         const QString &parameterName,
                                         const QString &subParameterName,
                                         bool *empty)
    {
        if(empty)
            *empty = false;

        if(!ok)
            return QPoint();

        const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
		const SubParameter &unitSubParameter = retreiveSubParameter(parameterName, QStringLiteral("unit"));
        QString result;

        if(subParameter.isCode())
        {
            QScriptValue evaluationResult = evaluateCode(ok, subParameter);
            if(auto codePoint = qobject_cast<Code::Point*>(evaluationResult.toQObject()))
            {
                QPointF point = QPointF(codePoint->point().x(), codePoint->point().y());

                computePercentPosition(point, unitSubParameter);

                return QPoint(point.x(), point.y());
            }

            result = evaluationResult.toString();
        }
        else
            result = evaluateText(ok, subParameter);

        if(!ok)
            return QPoint();

		if(result.isEmpty() || result == QLatin1String(":"))
        {
            if(empty)
                *empty = true;

            return QPoint();
        }

		QStringList positionStringList = result.split(QStringLiteral(":"));
        if(positionStringList.count() != 2)
        {
            ok = false;

            emit executionException(ActionException::InvalidParameterException, tr("\"%1\" is not a valid position.").arg(result));

            return QPoint();
        }

        QPointF point(positionStringList.at(0).toFloat(&ok), positionStringList.at(1).toFloat(&ok));
        if(!ok)
        {
            emit executionException(ActionException::InvalidParameterException, tr("\"%1\" is not a valid position.").arg(result));

            return QPoint();
		}

        computePercentPosition(point, unitSubParameter);

        return {static_cast<int>(point.x()), static_cast<int>(point.y())};
	}

	QStringList ActionInstance::evaluateItemList(bool &ok, const QString &parameterName, const QString &subParameterName)
	{
		if(!ok)
			return QStringList();

		const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
		QString result;

		if(subParameter.isCode())
			result = evaluateCode(ok, subParameter).toString();
		else
			result = evaluateText(ok, subParameter);

		if(!ok)
			return QStringList();

		return result.split(QLatin1Char('\n'), QString::SkipEmptyParts);
	}

	QPolygon ActionInstance::evaluatePolygon(bool &ok,
					   const QString &parameterName,
					   const QString &subParameterName)
	{
		if(!ok)
			return QPolygon();

		const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
		QString result;

		if(subParameter.isCode())
			result = evaluateCode(ok, subParameter).toString();
		else
			result = evaluateText(ok, subParameter);

		if(!ok)
			return QPolygon();

		if(result.isEmpty() || result == QLatin1String(";"))
			return QPolygon();

		QStringList pointStrings = result.split(QLatin1Char(';'), QString::SkipEmptyParts);
		QPolygon polygon;

        for(const QString &pointString: pointStrings)
		{
			QStringList pointComponents = pointString.split(QLatin1Char(':'), QString::SkipEmptyParts);
			if(pointComponents.size() != 2)
				continue;

			polygon << QPoint(pointComponents.at(0).toInt(), pointComponents.at(1).toInt());
		}

		return polygon;
	}

	QColor ActionInstance::evaluateColor(bool &ok,
					   const QString &parameterName,
					   const QString &subParameterName)
	{
		if(!ok)
			return {};

		const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
		QString result;

		if(subParameter.isCode())
		{
			QScriptValue evaluationResult = evaluateCode(ok, subParameter);
			if(auto codeColor = qobject_cast<Code::Color*>(evaluationResult.toQObject()))
				return codeColor->color();

			result = evaluationResult.toString();
		}
		else
			result = evaluateText(ok, subParameter);

		if(!ok)
			return QColor();

		if(result.isEmpty() || result == QLatin1String("::"))
			return QColor();

		QStringList colorStringList = result.split(QStringLiteral(":"));
		if(colorStringList.count() != 3)
		{
			ok = false;

            emit executionException(ActionException::InvalidParameterException, tr("\"%1\" is not a valid color.").arg(result));

			return QColor();
		}

		QColor color = QColor(colorStringList.at(0).toInt(&ok), colorStringList.at(1).toInt(&ok), colorStringList.at(2).toInt(&ok));
		if(!ok)
		{
            emit executionException(ActionException::InvalidParameterException, tr("\"%1\" is not a valid color.").arg(result));

			return QColor();
		}

        return color;
    }

    QDateTime ActionInstance::evaluateDateTime(bool &ok, const QString &parameterName, const QString &subParameterName)
    {
        if(!ok)
            return QDateTime();

        const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
        QString result;

        if(subParameter.isCode())
        {
            QScriptValue evaluationResult = evaluateCode(ok, subParameter);
            if(evaluationResult.isDate())
                return evaluationResult.toDateTime();

            result = evaluationResult.toString();
        }
        else
            result = evaluateText(ok, subParameter);

        if(!ok)
            return QDateTime();

		QDateTime dateTime = QDateTime::fromString(result, QStringLiteral("dd/MM/yyyy hh:mm:ss"));

        if(!dateTime.isValid())
        {
            ok = false;

            return QDateTime();
        }

        return dateTime;
    }

    void ActionInstance::validateParameterRange(bool &ok, int parameter, const QString &parameterName, const QString &parameterTranslatedName, int minimum, int maximum)
    {
        if(ok && (parameter < minimum || parameter > maximum))
        {
            ok = false;

            setCurrentParameter(parameterName);
            emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid %1 value : %2").arg(parameterTranslatedName).arg(parameter));
            return;
        }
    }

	QString ActionInstance::nextLine() const
	{
		return d->scriptEngine->property("Script.nextLine").toString();
	}

    void ActionInstance::setNextLine(const QString &nextLine, bool doNotResetPreviousActions)
	{
		QScriptValue scriptValue = d->scriptEngine->globalObject().property(QStringLiteral("Script"));
		scriptValue.setProperty(QStringLiteral("nextLine"), d->scriptEngine->newVariant(QVariant(nextLine)));
		scriptValue.setProperty(QStringLiteral("doNotResetPreviousActions"), doNotResetPreviousActions);
	}

    void ActionInstance::setNextLine(int nextLine, bool doNotResetPreviousActions)
	{
        setNextLine(QString::number(nextLine), doNotResetPreviousActions);
	}

	void ActionInstance::setArray(const QString &name, const QStringList &stringList)
	{
		if(stringList.isEmpty())
			return;

		QScriptValue back = d->scriptEngine->newArray(stringList.count());

		for(int index = 0; index < stringList.count(); ++index)
			back.setProperty(index, stringList.at(index));

        if(!name.isEmpty() && NameRegExp.exactMatch(name))
			d->scriptEngine->globalObject().setProperty(name, back);
	}

	void ActionInstance::setArrayKeyValue(const QString &name, const QHash<QString, QString> &hashKeyValue)
	{
		if(hashKeyValue.isEmpty())
			return;

		QScriptValue back = d->scriptEngine->newArray(hashKeyValue.count());

		QHashIterator<QString, QString> it(hashKeyValue);
		while (it.hasNext())
		{
			it.next();
			back.setProperty(it.key(), it.value());
		}

		setVariable(name, back);
	}

	void ActionInstance::setVariable(const QString &name, const QScriptValue &value)
	{
        if(!name.isEmpty() && NameRegExp.exactMatch(name))
			d->scriptEngine->globalObject().setProperty(name, value);
	}

    QScriptValue ActionInstance::variable(const QString &name)
	{
        if(name.isEmpty() || !NameRegExp.exactMatch(name))
            return QScriptValue();

        return d->scriptEngine->globalObject().property(name);
	}

	void ActionInstance::setCurrentParameter(const QString &parameterName, const QString &subParameterName)
	{
		d->scriptEngine->globalObject().setProperty(QStringLiteral("currentParameter"), parameterName, QScriptValue::ReadOnly);
        d->scriptEngine->globalObject().setProperty(QStringLiteral("currentSubParameter"), subParameterName, QScriptValue::ReadOnly);
    }

    void ActionInstance::executionEnded()
    {
        emit executionEndedSignal();

        d->executionDuration += d->executionTimer.elapsed();
    }

	SubParameter ActionInstance::retreiveSubParameter(const QString &parameterName, const QString &subParameterName)
	{
		setCurrentParameter(parameterName, subParameterName);

        SubParameter back = subParameter(parameterName, subParameterName);

        // Re-evaluate the field as code if it contains a single variable
		if(!back.isCode() && back.value().startsWith(QLatin1Char('$')))
        {
            QString stringValue = back.value();
            QString variableName = stringValue.right(stringValue.size() - 1);
            const QScriptValue &value = d->scriptEngine->globalObject().property(variableName);

            if(value.isValid())
            {
                back.setCode(true);
                back.setValue(variableName);
            }
        }

        return back;
	}

    QScriptValue ActionInstance::evaluateCode(bool &ok, const QString &toEvaluate)
	{
		ok = true;

        QScriptValue result = d->scriptEngine->evaluate(toEvaluate);
		if(result.isError())
		{
            ok = false;

            emit executionException(ActionException::CodeErrorException, result.toString());
			return QScriptValue();
		}

		if(!result.isValid())
		{
			ok = false;

			return QScriptValue();
		}

        return result;
    }

    QScriptValue ActionInstance::evaluateCode(bool &ok, const SubParameter &toEvaluate)
    {
        return evaluateCode(ok, toEvaluate.value());
    }

    QString ActionInstance::evaluateText(bool &ok, const QString &toEvaluate)
	{
		ok = true;

        int position = 0;

        return evaluateTextString(ok, toEvaluate, position);
    }

    QString ActionInstance::evaluateText(bool &ok, const SubParameter &toEvaluate)
    {
        return evaluateText(ok, toEvaluate.value());
    }

    QString ActionInstance::evaluateTextString(bool &ok, const QString &toEvaluate, int &position)
	{
		ok = true;

		int startIndex = position;

		QString result;

		while(position < toEvaluate.length())
		{
			if(toEvaluate[position] == QLatin1Char('$'))
			{
				//find a variable name
				if(VariableRegExp.indexIn(toEvaluate, position) != -1)
				{
					QString foundVariableName = VariableRegExp.cap(1);
					QScriptValue foundVariable = d->scriptEngine->globalObject().property(foundVariableName);

					position += foundVariableName.length();

					if(!foundVariable.isValid())
					{
						ok = false;

						emit executionException(ActionException::InvalidParameterException, tr("Undefined variable \"%1\"").arg(foundVariableName));
						return QString();
					}

					QString stringEvaluationResult;

					if(foundVariable.isNull())
						stringEvaluationResult = QStringLiteral("[Null]");
					else if(foundVariable.isUndefined())
						stringEvaluationResult = QStringLiteral("[Undefined]");
					else if(foundVariable.isArray())
					{
						while((position + 1 < toEvaluate.length()) && toEvaluate[position + 1] == QLatin1Char('['))
						{
							position += 2;
							QString indexArray = evaluateTextString(ok, toEvaluate, position);

							if((position < toEvaluate.length()) && toEvaluate[position] == QLatin1Char(']'))
							{
								QScriptString internalIndexArray = d->scriptEngine->toStringHandle(indexArray);
								bool flag = true;
								int numIndex = internalIndexArray.toArrayIndex(&flag);

								if(flag) //numIndex is valid
									foundVariable = foundVariable.property(numIndex);
								else //use internalIndexArray
									foundVariable = foundVariable.property(internalIndexArray);
							}
							else
							{
								//syntax error
								ok = false;

								emit executionException(ActionException::InvalidParameterException, tr("Invalid parameter. Unable to evaluate string"));
								return QString();
							}

							//COMPATIBILITY: we break the while loop if foundVariable is no more of Array type
							if(!foundVariable.isArray())
								break;
						}
						//end of while, no more '['
						if(foundVariable.isArray())
							stringEvaluationResult = evaluateVariableArray(ok, foundVariable);
						else
							stringEvaluationResult = foundVariable.toString();
					}
					else if(foundVariable.isVariant())
					{
						QVariant variantEvaluationResult = foundVariable.toVariant();
						switch(variantEvaluationResult.type())
						{
						case QVariant::StringList:
							stringEvaluationResult = variantEvaluationResult.toStringList().join(QStringLiteral("\n"));
							break;
						case QVariant::ByteArray:
							stringEvaluationResult = QStringLiteral("[Raw data]");
							break;
						default:
							stringEvaluationResult = foundVariable.toString();
							break;
						}
					}
					else
						stringEvaluationResult = foundVariable.toString();

					result.append(stringEvaluationResult);
				}

			}
			else if (toEvaluate[position] == QLatin1Char(']'))
			{
				if(startIndex == 0)
					//in top level evaluation isolated character ']' is accepted (for compatibility reason), now prefer "\]"
					//i.e without matching '['
					result.append(toEvaluate[position]);
				else
					//on other levels, the parsing is stopped at this point
					return result;
			}
			else if(toEvaluate[position] == QLatin1Char('\\'))
			{
				if(startIndex == 0)
				{
					//for ascendant compatibility reason
					//in top level evaluation '\' is not only an escape character,
					//but can also be a standard character in some cases
					if((position + 1) < toEvaluate.length())
					{
						position++;
						if(toEvaluate[position] == QLatin1Char('$') || toEvaluate[position] == QLatin1Char('[') || toEvaluate[position] == QLatin1Char(']') || toEvaluate[position] == QLatin1Char('\\'))
							result.append(toEvaluate[position]);
						else
						{
							position--;
							result.append(toEvaluate[position]);
						}
					}
					else
						result.append(toEvaluate[position]);
				}
				else
				{
					position++;
					if( position < toEvaluate.length() )
						result.append(toEvaluate[position]);
				}
			}
			else
				result.append(toEvaluate[position]);

			position++;
		}

		return result;
	}

	QDataStream &operator << (QDataStream &s, const ActionInstance &actionInstance)
	{
		s << actionInstance.label();
		s << actionInstance.comment();
		s << actionInstance.parametersData();
		s << actionInstance.color();
		s << actionInstance.isEnabled();
		s << actionInstance.isSelected();
		s << actionInstance.exceptionActionInstances();
		s << actionInstance.pauseBefore();
		s << actionInstance.pauseAfter();
		s << actionInstance.timeout();

		return s;
	}

	QDataStream &operator >> (QDataStream &s, ActionInstance &actionInstance)
	{
		QString label;
		QString comment;
		ParametersData parametersData;
		QColor color;
		bool enabled;
		bool selected;
		ExceptionActionInstancesHash exceptionActionInstances;
		int pauseBefore;
		int pauseAfter;
		int timeout;

		s >> label;
		s >> comment;
		s >> parametersData;
		s >> color;
		s >> enabled;
		s >> selected;
		s >> exceptionActionInstances;
		s >> pauseBefore;
		s >> pauseAfter;
		s >> timeout;

		actionInstance.setLabel(label);
		actionInstance.setComment(comment);
		actionInstance.setParametersData(parametersData);
		actionInstance.setColor(color);
		actionInstance.setEnabled(enabled);
		actionInstance.setSelected(selected);
		actionInstance.setExceptionActionInstances(exceptionActionInstances);
		actionInstance.setPauseBefore(pauseBefore);
		actionInstance.setPauseAfter(pauseAfter);
		actionInstance.setTimeout(timeout);

		return s;
	}

	QDebug &operator << (QDebug &dbg, const ActionInstance &actionInstance)
	{
		dbg.space() << "Id:" << actionInstance.definition()->id();
		dbg.space() << "Label:" << actionInstance.label();
		dbg.space() << "Comment:" << actionInstance.comment();
		dbg.space() << "Color:" << actionInstance.color();
		dbg.space() << "Enabled:" << actionInstance.isEnabled();
		dbg.space() << "Selected:" << actionInstance.isSelected();
		dbg.space() << "Exception action instances:" << actionInstance.exceptionActionInstances();
		dbg.space() << "Data:" << actionInstance.parametersData();
		dbg.space() << "Pause before:" << actionInstance.pauseBefore();
		dbg.space() << "Pause after:" << actionInstance.pauseAfter();
		dbg.space() << "Timeout:" << actionInstance.timeout();

		return dbg.maybeSpace();
	}

	QDebug &operator << (QDebug &dbg, const ParametersData &parametersData)
	{
        for(const QString &parameterName: parametersData.keys())
		{
			dbg.space() << parameterName << "=" << parametersData.value(parameterName);
		}

		return dbg.maybeSpace();
	}
	
	QDebug &operator << (QDebug &dbg, const ExceptionActionInstancesHash &exceptionActionInstancesHash)
	{
        for(ActionException::Exception exception: exceptionActionInstancesHash.keys())
		{
			dbg.space() << exception << "=" << exceptionActionInstancesHash.value(exception);
		}

        return dbg.maybeSpace();
    }
}
