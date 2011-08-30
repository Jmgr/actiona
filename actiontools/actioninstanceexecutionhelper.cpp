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

#include "actioninstanceexecutionhelper.h"
#include "script.h"
#include "ifactionvalue.h"

#include <QScriptValue>
#include <QScriptValueIterator>
#include <QScriptEngine>
#include <QPoint>
#include <QPolygon>
#include <QDebug>

namespace ActionTools
{
	ActionInstanceExecutionHelper::ActionInstanceExecutionHelper(ActionInstance *actionInstance, Script *script, QScriptEngine *scriptEngine)
			: mActionInstance(actionInstance),
			mScript(script),
			mScriptEngine(scriptEngine),
			mNameRegExp("^[A-Za-z_][A-Za-z0-9_]*$")
	{
		connect(this, SIGNAL(evaluationException(int,QString)),
				actionInstance, SIGNAL(executionException(int,QString)));
	}

	ActionInstanceExecutionHelper::~ActionInstanceExecutionHelper()
	{
	}

	bool ActionInstanceExecutionHelper::evaluateVariant(QVariant &buffer,
										const QString &parameterName,
										const QString &subParameterName)
	{
		mParameterName = parameterName;
		mSubParameterName = subParameterName;

		const SubParameter &toEvaluate = mActionInstance->subParameter(parameterName, subParameterName);
		if(toEvaluate.value().toString().isEmpty())
		{
			buffer = QVariant();
			return true;
		}

		if(!evaluate(toEvaluate))
			return false;

		buffer = mResult;

		return true;
	}

	bool ActionInstanceExecutionHelper::evaluateString(QString &buffer,
										const QString &parameterName,
										const QString &subParameterName)
	{
		mParameterName = parameterName;
		mSubParameterName = subParameterName;

		const SubParameter &toEvaluate = mActionInstance->subParameter(parameterName, subParameterName);
		if(toEvaluate.value().toString().isEmpty())
		{
			buffer = QString();
			return true;
		}

		if(!evaluate(toEvaluate))
			return false;

		if(mResult.type() == QVariant::StringList)
			buffer = mResult.toStringList().join("\n");
		else if(mResult.canConvert(QVariant::String))
			buffer = mResult.toString();
		else
			buffer = tr("[Raw data]");

		return true;
	}

	bool ActionInstanceExecutionHelper::evaluateVariable(QString &buffer,
										const QString &parameterName,
										const QString &subParameterName)
	{
		mParameterName = parameterName;
		mSubParameterName = subParameterName;

		const SubParameter &toEvaluate = mActionInstance->subParameter(parameterName, subParameterName);

		if(!evaluate(toEvaluate))
			return false;

		buffer = mResult.toString();

		if(!buffer.isEmpty() && !mNameRegExp.exactMatch(buffer))
		{
			mErrorMessage = tr("A variable name can only contain alphanumeric characters and cannot start with a digit.");
			emit evaluationException(ActionException::BadParameterException, mErrorMessage);
			return false;
		}

		return true;
	}

	bool ActionInstanceExecutionHelper::evaluateInteger(int &buffer,
										  const QString &parameterName,
										  const QString &subParameterName)
	{
		mParameterName = parameterName;
		mSubParameterName = subParameterName;

		const SubParameter &toEvaluate = mActionInstance->subParameter(parameterName, subParameterName);

		if(!evaluate(toEvaluate))
			return false;

		bool result = false;

		buffer = mResult.toInt(&result);

		if(!result)
		{
			mErrorMessage = tr("Expected an integer value.");
			emit evaluationException(ActionException::BadParameterException, mErrorMessage);
			return false;
		}

		return true;
	}

	bool ActionInstanceExecutionHelper::evaluateBoolean(bool &buffer,
										  const QString &parameterName,
										  const QString &subParameterName)
	{
		mParameterName = parameterName;
		mSubParameterName = subParameterName;

		const SubParameter &toEvaluate = mActionInstance->subParameter(parameterName, subParameterName);

		if(!evaluate(toEvaluate))
			return false;

		buffer = mResult.toBool();

		return true;
	}

	bool ActionInstanceExecutionHelper::evaluateDouble(double &buffer,
										const QString &parameterName,
										const QString &subParameterName)
	{
		mParameterName = parameterName;
		mSubParameterName = subParameterName;

		const SubParameter &toEvaluate = mActionInstance->subParameter(parameterName, subParameterName);
		if(!evaluate(toEvaluate))
			return false;

		bool result = false;

		buffer = mResult.toDouble(&result);

		if(!result)
		{
			mErrorMessage = tr("Expected a decimal value.");
			emit evaluationException(ActionException::BadParameterException, mErrorMessage);
			return false;
		}

		return true;
	}

	bool ActionInstanceExecutionHelper::evaluateIfAction(IfActionValue &buffer,
										const QString &parameterName)
	{
		buffer.setActionParameter(mActionInstance->subParameter(parameterName, "line"));

		return evaluateString(buffer.action(), parameterName, "action");
	}

	bool ActionInstanceExecutionHelper::evaluateSubParameter(QString &buffer,
							  const SubParameter &subParameter)
	{
		//This needs some refactoring -> copy & paste mostly
		if(subParameter.value().toString().isEmpty())
		{
			buffer = QString();
			return true;
		}

		if(!evaluate(subParameter))
			return false;

		if(mResult.type() == QVariant::StringList)
			buffer = mResult.toStringList().join("\n");
		else if(mResult.canConvert(QVariant::String))
			buffer = mResult.toString();
		else
			buffer = tr("[Raw data]");

		return true;
	}

	bool ActionInstanceExecutionHelper::evaluatePoint(QPoint &buffer,
					   const QString &parameterName,
					   const QString &subParameterName)
	{
		QString positionString;

		if(!evaluateString(positionString, parameterName, subParameterName))
			return false;

		QStringList positionStringList = positionString.split(":");
		if(positionStringList.count() != 2)
		{
			mErrorMessage = tr("\"%1\" is not a valid position.").arg(positionString);
			emit evaluationException(ActionException::BadParameterException, mErrorMessage);
			return false;
		}

		bool ok = true;
		buffer = QPoint(positionStringList.at(0).toInt(&ok), positionStringList.at(1).toInt(&ok));
		if(!ok)
		{
			mErrorMessage = tr("\"%1\" is not a valid position.").arg(positionString);
			emit evaluationException(ActionException::BadParameterException, mErrorMessage);
			return false;
		}

		return true;
	}

	bool ActionInstanceExecutionHelper::evaluatePolygon(QPolygon &buffer,
					   const QString &parameterName,
					   const QString &subParameterName)
	{
		QString polygonString;

		if(!evaluateString(polygonString, parameterName, subParameterName))
			return false;

		QStringList pointStrings = polygonString.split(';', QString::SkipEmptyParts);

		foreach(const QString &pointString, pointStrings)
		{
			QStringList pointComponents = pointString.split(':', QString::SkipEmptyParts);
			if(pointComponents.size() != 2)
				continue;

			buffer << QPoint(pointComponents.at(0).toInt(), pointComponents.at(1).toInt());
		}

		return true;
	}

	bool ActionInstanceExecutionHelper::evaluateColor(QColor &buffer,
					   const QString &parameterName,
					   const QString &subParameterName)
	{
		QString colorString;

		if(!evaluateString(colorString, parameterName, subParameterName))
			return false;

		QStringList colorStringList = colorString.split(":");
		if(colorStringList.count() != 3)
		{
			mErrorMessage = tr("\"%1\" is not a valid color.").arg(colorString);
			emit evaluationException(ActionException::BadParameterException, mErrorMessage);
			return false;
		}

		bool ok = true;
		buffer = QColor(colorStringList.at(0).toInt(&ok), colorStringList.at(1).toInt(&ok), colorStringList.at(2).toInt(&ok));
		if(!ok)
		{
			mErrorMessage = tr("\"%1\" is not a valid color.").arg(colorString);
			emit evaluationException(ActionException::BadParameterException, mErrorMessage);
			return false;
		}

		return true;
	}

	QString ActionInstanceExecutionHelper::nextLine() const
	{
		return mScriptEngine->property("Script.nextLine").toString();
	}

	void ActionInstanceExecutionHelper::setNextLine(const QString &nextLine)
	{
		QScriptValue scriptValue = mScriptEngine->globalObject().property("Script");
		scriptValue.setProperty("nextLine", mScriptEngine->newVariant(QVariant(nextLine)));
	}

	void ActionInstanceExecutionHelper::setVariable(const QString &name, const QVariant &value)
	{
		if(!name.isEmpty() && mNameRegExp.exactMatch(name))
			mScriptEngine->globalObject().setProperty(name, mScriptEngine->newVariant(value));
	}

	QVariant ActionInstanceExecutionHelper::variable(const QString &name)
	{
		if(name.isEmpty() || !mNameRegExp.exactMatch(name))
			return QVariant();

		return mScriptEngine->globalObject().property(name).toVariant();
	}

	void ActionInstanceExecutionHelper::setCurrentParameter(const QString &parameterName, const QString &subParameterName)
	{
		mScriptEngine->globalObject().setProperty("currentParameter", mScriptEngine->newVariant(parameterName), QScriptValue::ReadOnly);
		mScriptEngine->globalObject().setProperty("currentSubParameter", mScriptEngine->newVariant(subParameterName), QScriptValue::ReadOnly);
	}

	bool ActionInstanceExecutionHelper::evaluate(const SubParameter &toEvaluate)
	{
		mScriptEngine->globalObject().setProperty("currentParameter", mScriptEngine->newVariant(mParameterName), QScriptValue::ReadOnly);
		mScriptEngine->globalObject().setProperty("currentSubParameter", mScriptEngine->newVariant(mSubParameterName), QScriptValue::ReadOnly);

		if(!toEvaluate.isCode())
		{
			QString value(toEvaluate.value().toString());

			QRegExp regexpVariable("([^\\\\]|^)\\$([A-Za-z_][A-Za-z0-9_]*)", Qt::CaseSensitive, QRegExp::RegExp2);
			int position = 0;

			while((position = regexpVariable.indexIn(value, position)) != -1)
			{
				QString foundVariable = regexpVariable.cap(2);
				QScriptValue evaluationResult = mScriptEngine->globalObject().property(foundVariable);

				position += regexpVariable.cap(1).length();

				if(!evaluationResult.isValid())
				{
					mErrorMessage = tr("Variable %1 not found").arg(foundVariable);
					emit evaluationException(ActionException::BadParameterException, mErrorMessage);
					return false;
				}

				QVariant variantEvalutationResult = evaluationResult.toVariant();
				QString stringEvalutationResult;

				switch(variantEvalutationResult.type())
				{
				case QVariant::StringList:
					stringEvalutationResult = variantEvalutationResult.toStringList().join("\n");
					break;
				case QVariant::ByteArray:
					stringEvalutationResult = tr("[Raw data]");
					break;
				default:
					if(variantEvalutationResult.canConvert(QVariant::String))
						stringEvalutationResult = variantEvalutationResult.toString();
					else
						stringEvalutationResult = tr("[Raw data]");
					break;
				}

				value.replace(position,
							  foundVariable.length() + 1,
							  stringEvalutationResult);
			}

			mResult = value;

			return true;
		}

		QScriptValue result = mScriptEngine->evaluate(toEvaluate.value().toString());
		if(result.isError())
		{
			mErrorMessage = result.toString();
			emit evaluationException(ActionException::CodeErrorException, mErrorMessage);
			return false;
		}
		else if(!result.isValid()) // An invalid value means that an exception was thrown but was catched by the debugger ; abort() was called
			return false;

		if(result.isArray())
		{
			QStringList stringList;

			QScriptValueIterator it(result);
			while(it.hasNext())
			{
				it.next();

				stringList << it.value().toString();
			}

			mResult = stringList;
		}
		else
			mResult = result.toVariant();

		if(!mResult.isValid())
			mResult = tr("[Raw data]");

		return true;
	}
}
