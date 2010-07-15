#include "actioninstanceexecutionhelper.h"
#include "script.h"

#include <QScriptValue>
#include <QScriptEngine>
#include <QPoint>
#include <QDebug>

namespace ActionTools
{
	ActionInstanceExecutionHelper::ActionInstanceExecutionHelper(ActionInstance *actionInstance, Script *script, QScriptEngine *scriptEngine)
			: mActionInstance(actionInstance),
			mScript(script),
			mScriptEngine(scriptEngine)
	{
		connect(this, SIGNAL(evaluationError(ActionTools::ActionInstance::ExecutionException,QString)),
				actionInstance, SIGNAL(executionException(ActionTools::ActionInstance::ExecutionException,QString)));
	}

	ActionInstanceExecutionHelper::~ActionInstanceExecutionHelper()
	{
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

		buffer = mResult.toString();

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
		
		if(!buffer.isEmpty() && !buffer.contains(QRegExp("^[a-zA-Z0-9]+$")))
		{
			mErrorMessage = tr("A variable name can only contain alphanumeric characters.");
			emit evaluationError(ActionInstance::Error, mErrorMessage);
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

		return result;
	}

	bool ActionInstanceExecutionHelper::evaluateFloat(float &buffer,
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

		return result;
	}

	bool ActionInstanceExecutionHelper::evaluateListElement(int &buffer,
											  const QString &parameterName,
											  const QString &subParameterName,
											  const StringListPair &listElements)
	{
		mParameterName = parameterName;
		mSubParameterName = subParameterName;

		const SubParameter &toEvaluate = mActionInstance->subParameter(parameterName, subParameterName);
		if(!evaluate(toEvaluate))
			return false;

		QString selectedItem = mResult.toString();

		for(int i=0;i<listElements.first.size();++i)//Search in the non-translated items
		{
			if(listElements.first.at(i) == selectedItem)
			{
				buffer = i;
				return true;
			}
		}

		for(int i=0;i<listElements.second.size();++i)//Then search in the translated items
		{
			if(listElements.second.at(i) == selectedItem)
			{
				buffer = i;
				return true;
			}
		}

		bool success;

		buffer = mResult.toInt(&success);

		if(!success || buffer < 0 || buffer >= listElements.first.count())
		{
			mErrorMessage = tr("\"%1\" is incorrect.").arg(toEvaluate.value().toString());
			emit evaluationError(ActionInstance::Error, mErrorMessage);
			return false;
		}

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
			emit evaluationError(ActionInstance::Error, mErrorMessage);
			return false;
		}
		
		bool ok = true;
		buffer = QPoint(positionStringList.at(0).toInt(&ok), positionStringList.at(1).toInt(&ok));
		if(!ok)
		{
			mErrorMessage = tr("\"%1\" is not a valid position.").arg(positionString);
			emit evaluationError(ActionInstance::Error, mErrorMessage);
			return false;
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
			emit evaluationError(ActionInstance::Error, mErrorMessage);
			return false;
		}
		
		bool ok = true;
		buffer = QColor(colorStringList.at(0).toInt(&ok), colorStringList.at(1).toInt(&ok), colorStringList.at(2).toInt(&ok));
		if(!ok)
		{
			mErrorMessage = tr("\"%1\" is not a valid color.").arg(colorString);
			emit evaluationError(ActionInstance::Error, mErrorMessage);
			return false;
		}
		
		return true;
	}
	
	QString ActionInstanceExecutionHelper::nextLine() const
	{
		return mScriptEngine->evaluate("Script.nextLine").toString();
	}
	
	void ActionInstanceExecutionHelper::setNextLine(const QString &nextLine)
	{
		QScriptValue scriptValue = mScriptEngine->globalObject().property("Script");
		scriptValue.setProperty("nextLine", mScriptEngine->newVariant(QVariant(nextLine)));
	}

	bool ActionInstanceExecutionHelper::evaluate(const SubParameter &toEvaluate)
	{
		mScriptEngine->globalObject().setProperty("currentParameter", mScriptEngine->newVariant(mParameterName), QScriptValue::ReadOnly);
		mScriptEngine->globalObject().setProperty("currentSubParameter", mScriptEngine->newVariant(mSubParameterName), QScriptValue::ReadOnly);

		if(!toEvaluate.isCode())
		{
			QString value(toEvaluate.value().toString());

			QRegExp regExp("\\$([a-zA-Z0-9]+)\\$");
			if(regExp.indexIn(value) != -1)
			{
				const QStringList &foundVariables = regExp.capturedTexts();
				
				for(int i = 1; i < foundVariables.count(); ++i)
				{
					if(!mScript->hasVariable(foundVariables.at(i)))
					{
						mErrorMessage = tr("Unknown variable : %1").arg(foundVariables.at(i));
						emit evaluationError(ActionInstance::Error, mErrorMessage);
						return false;
					}
					
					value.replace(QString("$%1$").arg(foundVariables.at(i)), mScript->variable(foundVariables.at(i)).toString());
				}
			}
			
			mResult = value;
			
			return true;
		}

		QScriptValue result = mScriptEngine->evaluate(toEvaluate.value().toString());
		if(result.isError())
		{
			mErrorMessage = result.toString();
			emit evaluationError(ActionInstance::Error, mErrorMessage);
			return false;
		}

		mResult = result.toVariant();

		return true;
	}
}
