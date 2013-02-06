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

#include "actioninstance.h"
#include "actiondefinition.h"
#include "actionfactory.h"
#include "elementdefinition.h"
#include "parameterdefinition.h"
#include "groupdefinition.h"
#include "script.h"
#include "code/point.h"
#include "code/color.h"

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

	const QRegExp ActionInstance::mNameRegExp("^[A-Za-z_][A-Za-z0-9_]*$", Qt::CaseSensitive, QRegExp::RegExp2);
	const QRegExp ActionInstance::mVariableRegExp("([^\\\\]|^)\\$([A-Za-z_][A-Za-z0-9_]*)", Qt::CaseSensitive, QRegExp::RegExp2);
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
			foreach(ElementDefinition *element, definition->elements())
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

	QVariant ActionInstance::evaluateVariant(bool &ok,
										const QString &parameterName,
										const QString &subParameterName)
	{
		if(!ok)
			return QVariant();

		const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
		QVariant result;

		if(subParameter.isCode())
			result = evaluateCode(ok, subParameter).toVariant();
		else
			result = evaluateText(ok, subParameter);

		if(!ok)
			return QVariant();

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

	QString ActionInstance::evaluateVariable(bool &ok,
										const QString &parameterName,
										const QString &subParameterName)
	{
		QString result = evaluateString(ok, parameterName, subParameterName);

		if(!ok)
			return 0;

		if(!result.isEmpty() && !mNameRegExp.exactMatch(result))
		{
			ok = false;

			emit executionException(ActionException::BadParameterException, tr("A variable name can only contain alphanumeric characters and cannot start with a digit."));

			return QString();
		}

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

			emit executionException(ActionException::BadParameterException, tr("Integer value expected."));

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

			emit executionException(ActionException::BadParameterException, tr("Decimal value expected."));

			return 0.0;
		}

		return doubleResult;
	}

	IfActionValue ActionInstance::evaluateIfAction(bool &ok,
										const QString &parameterName)
	{
		QString action = evaluateString(ok, parameterName, "action");

		if(!ok)
			return IfActionValue();

		return IfActionValue(action, subParameter(parameterName, "line"));
	}

	QString ActionInstance::evaluateSubParameter(bool &ok,
							  const SubParameter &subParameter)
	{
		if(!ok || subParameter.value().toString().isEmpty())
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

	QPoint ActionInstance::evaluatePoint(bool &ok,
					   const QString &parameterName,
					   const QString &subParameterName)
	{
		if(!ok)
			return QPoint();

		const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
		QString result;

		if(subParameter.isCode())
		{
			QScriptValue evaluationResult = evaluateCode(ok, subParameter);
			if(Code::Point *codePoint = qobject_cast<Code::Point*>(evaluationResult.toQObject()))
				return codePoint->point();

			result = evaluationResult.toString();
		}
		else
			result = evaluateText(ok, subParameter);

		if(!ok)
			return QPoint();

		if(result.isEmpty() || result == ":")
			return QPoint();

		QStringList positionStringList = result.split(":");
		if(positionStringList.count() != 2)
		{
			ok = false;

			emit executionException(ActionException::BadParameterException, tr("\"%1\" is not a valid position.").arg(result));

			return QPoint();
		}

		QPoint point = QPoint(positionStringList.at(0).toInt(&ok), positionStringList.at(1).toInt(&ok));
		if(!ok)
		{
			emit executionException(ActionException::BadParameterException, tr("\"%1\" is not a valid position.").arg(result));

			return QPoint();
		}

		return point;
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

		return result.split('\n', QString::SkipEmptyParts);
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

		if(result.isEmpty() || result == ";")
			return QPolygon();

		QStringList pointStrings = result.split(';', QString::SkipEmptyParts);
		QPolygon polygon;

		foreach(const QString &pointString, pointStrings)
		{
			QStringList pointComponents = pointString.split(':', QString::SkipEmptyParts);
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
			return QColor();

		const SubParameter &subParameter = retreiveSubParameter(parameterName, subParameterName);
		QString result;

		if(subParameter.isCode())
		{
			QScriptValue evaluationResult = evaluateCode(ok, subParameter);
			if(Code::Color *codeColor = qobject_cast<Code::Color*>(evaluationResult.toQObject()))
				return codeColor->color();

			result = evaluationResult.toString();
		}
		else
			result = evaluateText(ok, subParameter);

		if(!ok)
			return QColor();

		if(result.isEmpty() || result == "::")
			return QColor();

		QStringList colorStringList = result.split(":");
		if(colorStringList.count() != 3)
		{
			ok = false;

			emit executionException(ActionException::BadParameterException, tr("\"%1\" is not a valid color.").arg(result));

			return QColor();
		}

		QColor color = QColor(colorStringList.at(0).toInt(&ok), colorStringList.at(1).toInt(&ok), colorStringList.at(2).toInt(&ok));
		if(!ok)
		{
			emit executionException(ActionException::BadParameterException, tr("\"%1\" is not a valid color.").arg(result));

			return QColor();
		}

		return color;
	}

	QString ActionInstance::nextLine() const
	{
		return d->scriptEngine->property("Script.nextLine").toString();
	}

	void ActionInstance::setNextLine(const QString &nextLine)
	{
		QScriptValue scriptValue = d->scriptEngine->globalObject().property("Script");
		scriptValue.setProperty("nextLine", d->scriptEngine->newVariant(QVariant(nextLine)));
	}

	void ActionInstance::setNextLine(int nextLine)
	{
		setNextLine(QString::number(nextLine));
	}

	void ActionInstance::setArray(const QString &name, const QStringList &stringList)
	{
		if(stringList.count() == 0)
			return;

		QScriptValue back = d->scriptEngine->newArray(stringList.count());
		QStringList key_value;

		for(int index = 0; index < stringList.count(); ++index)
		{
			key_value = stringList.at(index).split("=");
			back.setProperty(key_value.at(0), key_value.at(1));
		}

		if(!name.isEmpty() && mNameRegExp.exactMatch(name))
			d->scriptEngine->globalObject().setProperty(name, back);
	}

	QScriptValue ActionInstance::arrayElement(const QString &name, int index)
	{
		if(name.isEmpty() || !mNameRegExp.exactMatch(name))
			return QScriptValue();

		return d->scriptEngine->globalObject().property(name).property(index);
	}

	void ActionInstance::setVariable(const QString &name, const QScriptValue &value)
	{
		if(!name.isEmpty() && mNameRegExp.exactMatch(name))
			d->scriptEngine->globalObject().setProperty(name, value);
	}

    QScriptValue ActionInstance::variable(const QString &name)
	{
		if(name.isEmpty() || !mNameRegExp.exactMatch(name))
            return QScriptValue();

        return d->scriptEngine->globalObject().property(name);
	}

	void ActionInstance::setCurrentParameter(const QString &parameterName, const QString &subParameterName)
	{
		d->scriptEngine->globalObject().setProperty("currentParameter", parameterName, QScriptValue::ReadOnly);
		d->scriptEngine->globalObject().setProperty("currentSubParameter", subParameterName, QScriptValue::ReadOnly);
	}

	bool ActionInstance::callProcedure(const QString &procedureName)
	{
		//Search for the corresponding ActionBeginProcedure action
		int beginProcedureLine = script()->findProcedure(procedureName);
		if(beginProcedureLine == -1)
		{
			emit executionException(ActionTools::ActionException::BadParameterException, tr("Unable to find any procedure named \"%1\"").arg(procedureName));

			return false;
		}

		setNextLine(beginProcedureLine + 2);//Lines start at 1

		script()->addProcedureCall(scriptLine());

		return true;
	}

	SubParameter ActionInstance::retreiveSubParameter(const QString &parameterName, const QString &subParameterName)
	{
		setCurrentParameter(parameterName, subParameterName);

		return subParameter(parameterName, subParameterName);
	}

	QScriptValue ActionInstance::evaluateCode(bool &ok, const SubParameter &toEvaluate)
	{
		ok = true;

		QScriptValue result = d->scriptEngine->evaluate(toEvaluate.value().toString());
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

	QString ActionInstance::evaluateText(bool &ok, const SubParameter &toEvaluate)
	{
		ok = true;

		QString value = toEvaluate.value().toString();

		int position = 0;

		while((position = mVariableRegExp.indexIn(value, position)) != -1)
		{
			QString foundVariableName = mVariableRegExp.cap(2);
			QScriptValue foundVariable = d->scriptEngine->globalObject().property(foundVariableName);

			position += mVariableRegExp.cap(1).length();

			if(!foundVariable.isValid())
			{
				ok = false;

				emit executionException(ActionException::BadParameterException, tr("Undefined variable \"%1\"").arg(foundVariableName));
				return QString();
			}

			QString stringEvaluationResult;

			if(foundVariable.isNull())
				stringEvaluationResult = tr("[Null]");
			else if(foundVariable.isUndefined())
				stringEvaluationResult = tr("[Undefined]");
			else if(foundVariable.isVariant())
			{
				QVariant variantEvaluationResult = foundVariable.toVariant();
				switch(variantEvaluationResult.type())
				{
				case QVariant::StringList:
					stringEvaluationResult = variantEvaluationResult.toStringList().join("\n");
					break;
				case QVariant::ByteArray:
					stringEvaluationResult = tr("[Raw data]");
					break;
				default:
					stringEvaluationResult = foundVariable.toString();
					break;
				}
			}
			else
				stringEvaluationResult = foundVariable.toString();

			value.replace(position,
						  foundVariableName.length() + 1,
						  stringEvaluationResult);
		}

		return value;
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
		foreach(const QString &parameterName, parametersData.keys())
		{
			dbg.space() << parameterName << "=" << parametersData.value(parameterName);
		}

		return dbg.maybeSpace();
	}
	
	QDebug &operator << (QDebug &dbg, const ExceptionActionInstancesHash &exceptionActionInstancesHash)
	{
		foreach(ActionException::Exception exception, exceptionActionInstancesHash.keys())
		{
			dbg.space() << exception << "=" << exceptionActionInstancesHash.value(exception);
		}

        return dbg.maybeSpace();
    }
}
