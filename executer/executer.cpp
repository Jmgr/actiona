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

#include "executer.h"
#include "script.h"
#include "actionfactory.h"
#include "executionwindow.h"
#include "consolewidget.h"
#include "scriptparameter.h"
#include "actioninstance.h"
#include "executionenvironment.h"
#include "executionalgorithms.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScriptContextInfo>
#include <QAction>

namespace Executer
{
	Executer::Executer(ActionTools::Script *script,
					   ActionTools::ActionFactory *actionFactory,
					   bool showExecutionWindow,
					   int executionWindowPosition,
					   int executionWindowScreen,
					   bool showConsoleWindow,
					   int consoleWindowPosition,
					   int consoleWindowScreen,
					   const QKeySequence &stopExecutionHotkey,
					   QStandardItemModel *consoleModel,
					   QObject *parent)
		: QObject(parent),
		mScript(script),
		mActionFactory(actionFactory),
		mShowExecutionWindow(showExecutionWindow),
		mExecutionWindowPosition(executionWindowPosition),
		mExecutionWindowScreen(executionWindowScreen),
		mShowConsoleWindow(showConsoleWindow),
		mConsoleWindowPosition(consoleWindowPosition),
		mConsoleWindowScreen(consoleWindowScreen),
		mStopExecutionShortcut(stopExecutionHotkey),
		mExecutionWindow(new ExecutionWindow()),
		mConsoleWidget(new ActionTools::ConsoleWidget(consoleModel)),
		mCurrentActionIndex(0),
		mExecutionStarted(false),
		mExecutionPaused(false),
		mExecuteOnlySelection(false),
		mScriptAgent(new ScriptAgent(&mScriptEngine)),
		mCurrentActionTimeout(0),
		mProgressDialog(0),
		mActiveActionsCount(0)
	{
		connect(mExecutionWindow, SIGNAL(canceled()), this, SLOT(stopExecution()));
		connect(&mStartExecutionTimer, SIGNAL(timeout()), this, SLOT(startActionExecution()));
		connect(&mTimeoutTimer, SIGNAL(timeout()), this, SLOT(updateTimeoutProgress()));

		mStartExecutionTimer.setSingleShot(true);
		mTimeoutTimer.setSingleShot(false);
		mTimeoutTimer.setInterval(10);

		mConsoleWidget->setWindowFlags(Qt::Tool |
					   Qt::WindowStaysOnTopHint |
					   Qt::CustomizeWindowHint |
					   Qt::WindowTitleHint);
		mConsoleWidget->updateClearButton();
	}

	Executer::~Executer()
	{
		delete mExecutionWindow;
		delete mConsoleWidget;
	}

	void printCall(QScriptContext *context, ActionTools::ConsoleWidget:: Type type)
	{
		QScriptValue calleeData = context->callee().data();
		Executer *executer = qobject_cast<Executer *>(calleeData.toQObject());
		QString message;
		ScriptAgent *agent = executer->scriptAgent();

		for(int argumentIndex = 0; argumentIndex < context->argumentCount(); ++argumentIndex)
			message += context->argument(argumentIndex).toString();

		switch(executer->scriptAgent()->context())
		{
		case ScriptAgent::Parameters:
			executer->consoleWidget()->addScriptParameterLine(message,
															  agent->currentParameter(),
															  agent->currentLine(),
															  agent->currentColumn(),
															  type);
			break;
		case ScriptAgent::Actions:
			executer->consoleWidget()->addUserLine(message,
												   executer->currentActionIndex(),
												   context->engine()->evaluate("currentParameter").toString(),
												   context->engine()->evaluate("currentSubParameter").toString(),
												   agent->currentLine(),
												   agent->currentColumn(),
												   context->backtrace(),
												   type);
			break;
		default:
			return;
		}
	}

	QScriptValue printFunction(QScriptContext *context, QScriptEngine *engine)
	{
		if(context->argumentCount() < 1)
			return engine->undefinedValue();

		printCall(context, ActionTools::ConsoleWidget::Information);

		return engine->undefinedValue();
	}

	QScriptValue printWarningFunction(QScriptContext *context, QScriptEngine *engine)
	{
		if(context->argumentCount() < 1)
			return engine->undefinedValue();

		printCall(context, ActionTools::ConsoleWidget::Warning);

		return engine->undefinedValue();
	}

	QScriptValue printErrorFunction(QScriptContext *context, QScriptEngine *engine)
	{
		if(context->argumentCount() < 1)
			return engine->undefinedValue();

		printCall(context, ActionTools::ConsoleWidget::Error);

		return engine->undefinedValue();
	}

	QScriptValue stopExecutionFunction(QScriptContext *context, QScriptEngine *engine)
	{
		QScriptValue calleeData = context->callee().data();
		Executer *executer = qobject_cast<Executer *>(calleeData.toQObject());

		executer->stopExecution();

		return engine->undefinedValue();
	}

	bool Executer::startExecution(bool onlySelection)
	{
	#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Executer::startExecution");
	#endif
		mScriptAgent->setContext(ScriptAgent::Unknown);

		mExecuteOnlySelection = onlySelection;
		mCurrentActionIndex = 0;
		mActiveActionsCount = 0;

		mScriptEngine.setAgent(mScriptAgent);

		QScriptValue script = mScriptEngine.newObject();
		mScriptEngine.globalObject().setProperty("Script", script, QScriptValue::ReadOnly);
		script.setProperty("nextLine", mScriptEngine.newVariant(QVariant(1)));

		QScriptValue stopExecutionFun = mScriptEngine.newFunction(stopExecutionFunction);
		stopExecutionFun.setData(mScriptEngine.newQObject(this));
		script.setProperty("stopExecution", stopExecutionFun);

		QScriptValue printFun = mScriptEngine.newFunction(printFunction);
		printFun.setData(mScriptEngine.newQObject(this));
		script.setProperty("print", printFun);

		printFun = mScriptEngine.newFunction(printWarningFunction);
		printFun.setData(mScriptEngine.newQObject(this));
		script.setProperty("printWarning", printFun);

		printFun = mScriptEngine.newFunction(printErrorFunction);
		printFun.setData(mScriptEngine.newQObject(this));
		script.setProperty("printError", printFun);

		addClassToScript(new ExecutionEnvironment(), "Environment");
		addClassToScript(new ExecutionAlgorithms(), "Algorithms");

		bool initSucceeded = true;

		mScriptAgent->setContext(ScriptAgent::ActionInit);

		int actionCount = mActionFactory->actionDefinitionCount();
		for(int actionIndex = 0; actionIndex < actionCount; ++actionIndex)
		{
			ActionTools::ActionDefinition *actionDefinition = mActionFactory->actionDefinition(actionIndex);

			ActionTools::ActionInstance *actionInstance = actionDefinition->scriptInit(&mScriptEngine);
			actionInstance->setupExecution(&mScriptEngine, mScript);
		}

		for(int actionIndex = 0; actionIndex < mScript->actionCount(); ++actionIndex)
		{
			ActionTools::ActionInstance *actionInstance = mScript->actionAt(actionIndex);
			actionInstance->reset();
			actionInstance->setupExecution(&mScriptEngine, mScript);
			mActionEnabled.append(true);

			if(canExecuteAction(actionIndex) == CanExecute)
				++mActiveActionsCount;
		}

		mScriptAgent->setContext(ScriptAgent::Parameters);

		for(int parameterIndex = 0; parameterIndex < mScript->parameterCount(); ++parameterIndex)
		{
			mScriptAgent->setCurrentParameter(parameterIndex);

			const ActionTools::ScriptParameter &scriptParameter = mScript->parameter(parameterIndex);
			QRegExp nameRegExp("[a-z_][a-z0-9_]*", Qt::CaseInsensitive);

			if(!nameRegExp.exactMatch(scriptParameter.name()))
			{
				mConsoleWidget->addScriptParameterLine(tr("Incorrect parameter name: \"%1\"").arg(scriptParameter.name()),
													   parameterIndex,
													   -1,
													   -1,
													   ActionTools::ConsoleWidget::Error);
				initSucceeded = false;
				continue;
			}

			QString value;
			if(scriptParameter.isCode())
			{
				QScriptValue result = mScriptEngine.evaluate(scriptParameter.value());
				if(result.isError())
				{
					mConsoleWidget->addScriptParameterLine(tr("Error while evaluating parameter \"%1\", error message: \"%2\"")
														   .arg(scriptParameter.name())
														   .arg(result.toString()),
														   parameterIndex,
														   -1,
														   -1,
														   ActionTools::ConsoleWidget::Error);
					initSucceeded = false;
					continue;
				}
				else
					value = result.toString();
			}
			else
				value = scriptParameter.value();

			mScriptEngine.globalObject().setProperty(scriptParameter.name(), value, QScriptValue::ReadOnly | QScriptValue::Undeletable);
		}

		if(!initSucceeded || mScript->actionCount() == 0)
			return false;

		if(mShowExecutionWindow)
		{
			mExecutionWindow->setStopExecutionShortcut(mStopExecutionShortcut);

			QRect screenRect = QApplication::desktop()->availableGeometry(mExecutionWindowScreen);
			QPoint position;

			if(mExecutionWindowPosition >= 0 && mExecutionWindowPosition <= 2)//Left
				position.setX(screenRect.left());
			else if(mExecutionWindowPosition >= 3 && mExecutionWindowPosition <= 5)//HCenter
				position.setX(screenRect.left() + screenRect.width() / 2 - mExecutionWindow->width() / 2);
			else if(mExecutionWindowPosition >= 6 && mExecutionWindowPosition <= 8)//Right
				position.setX(screenRect.left() + screenRect.width() - mExecutionWindow->width());

			if(mExecutionWindowPosition == 0 || mExecutionWindowPosition == 3 || mExecutionWindowPosition == 6)//Up
				position.setY(screenRect.top());
			else if(mExecutionWindowPosition == 1 || mExecutionWindowPosition == 4 || mExecutionWindowPosition == 7)//VCenter
				position.setY(screenRect.top() + screenRect.height() / 2 - mExecutionWindow->height() / 2);
			else if(mExecutionWindowPosition == 2 || mExecutionWindowPosition == 5 || mExecutionWindowPosition == 8)//Down
				position.setY(screenRect.top() + screenRect.height() - mExecutionWindow->height());

			mExecutionWindow->move(position);
			mExecutionWindow->show();
		}

		if(mShowConsoleWindow)
		{
			QRect screenRect = QApplication::desktop()->availableGeometry(mConsoleWindowScreen);
			QPoint position;

			if(mConsoleWindowPosition >= 0 && mConsoleWindowPosition <= 2)//Left
				position.setX(screenRect.left());
			else if(mConsoleWindowPosition >= 3 && mConsoleWindowPosition <= 5)//HCenter
				position.setX(screenRect.left() + screenRect.width() / 2 - mConsoleWidget->width() / 2);
			else if(mConsoleWindowPosition >= 6 && mConsoleWindowPosition <= 8)//Right
				position.setX(screenRect.left() + screenRect.width() - mConsoleWidget->width());

			if(mConsoleWindowPosition == 0 || mConsoleWindowPosition == 3 || mConsoleWindowPosition == 6)//Up
				position.setY(screenRect.top());
			else if(mConsoleWindowPosition == 1 || mConsoleWindowPosition == 4 || mConsoleWindowPosition == 7)//VCenter
				position.setY(screenRect.top() + screenRect.height() / 2 - mConsoleWidget->height() / 2);
			else if(mConsoleWindowPosition == 2 || mConsoleWindowPosition == 5 || mConsoleWindowPosition == 8)//Down
				position.setY(screenRect.top() + screenRect.height() - mConsoleWidget->height());

			mConsoleWidget->move(position);
			mConsoleWidget->show();
		}

		mExecutionStarted = true;

		mScriptAgent->setContext(ScriptAgent::Actions);

		executeCurrentAction();

		return true;
	}

	void Executer::stopExecution()
	{
		if(!mExecutionStarted)
			return;

		mTimeoutTimer.stop();

		if(mCurrentActionIndex >= 0 && mCurrentActionIndex < mScript->actionCount())
		{
			mScript->actionAt(mCurrentActionIndex)->disconnect();
			if(!mExecutionPaused)
				mScript->actionAt(mCurrentActionIndex)->stopExecution();
		}

		for(int actionIndex = 0; actionIndex < mScript->actionCount(); ++actionIndex)
		{
			ActionTools::ActionInstance *actionInstance = mScript->actionAt(actionIndex);
			actionInstance->stopLongTermExecution();
		}

		delete mProgressDialog;
		mProgressDialog = 0;
		mExecutionWindow->hide();
		mConsoleWidget->hide();

		emit executionStopped();

		mExecutionStarted = false;

		mScriptEngine.abortEvaluation();
	}

	void Executer::executionException(int exception,
									  const QString &message)
	{
		ActionTools::ActionInstance *currentActionInstance = mScript->actionAt(mCurrentActionIndex);
		bool standardException = (exception >= 0 && exception < ActionTools::ActionException::ExceptionCount);
		bool customException = false;

		foreach(ActionTools::ActionException *actionException, currentActionInstance->definition()->exceptions())
		{
			if(actionException->id() == exception)
			{
				customException = true;
				break;
			}
		}

		if(!standardException && !customException)
		{
			mConsoleWidget->addDesignErrorLine(tr("Action design error: Invalid exception emitted (%1, line %2)")
											   .arg(currentActionInstance->definition()->name())
											   .arg(mCurrentActionIndex+1), ActionTools::ConsoleWidget::Error);
			stopExecution();
			return;
		}

		ActionTools::ActionException::ExceptionActionInstance exceptionActionInstance = currentActionInstance->exceptionActionInstance(static_cast<ActionTools::ActionException::Exception>(exception));
		ActionTools::ConsoleWidget::Type exceptionType;
		bool shouldStopExecution;
		switch(exceptionActionInstance.action())
		{
		case ActionTools::ActionException::SkipExceptionAction:
			exceptionType = ActionTools::ConsoleWidget::Information;
			actionExecutionEnded();

			shouldStopExecution = false;
			break;
		case ActionTools::ActionException::GotoLineExceptionAction:
			{
				exceptionType = ActionTools::ConsoleWidget::Information;

				if(canExecuteAction(exceptionActionInstance.line()) != CanExecute)
				{
					mConsoleWidget->addExceptionLine(tr("Invalid exception line: %1").arg(exceptionActionInstance.line()), mCurrentActionIndex, exception, ActionTools::ConsoleWidget::Error);
					shouldStopExecution = true;
				}
				else
				{
					QScriptValue script = mScriptEngine.globalObject().property("Script");
					script.setProperty("nextLine", mScriptEngine.newVariant(QVariant(exceptionActionInstance.line())));
					actionExecutionEnded();
					shouldStopExecution = false;
				}
			}
			break;
		default:
			exceptionType = ActionTools::ConsoleWidget::Error;

			shouldStopExecution = true;
		}

		if(exception != ActionTools::ActionException::TimeoutException)
		{
			mConsoleWidget->addActionLine(tr("Script line %1: ").arg(mCurrentActionIndex+1) + message,
										mCurrentActionIndex,
										mScriptEngine.evaluate("currentParameter").toString(),
										mScriptEngine.evaluate("currentSubParameter").toString(),
										mScriptAgent->currentLine(),
										mScriptAgent->currentColumn(),
										exceptionType);
		}

		if(shouldStopExecution)
			stopExecution();
	}

	void Executer::actionExecutionEnded()
	{
		mTimeoutTimer.stop();
		mScript->actionAt(mCurrentActionIndex)->disconnect();

		emit actionEnded(mCurrentActionIndex, mActiveActionsCount);

		QTimer::singleShot(mScript->actionAt(mCurrentActionIndex)->pauseAfter(), this, SLOT(startNextAction()));
		mExecutionPaused = true;
	}

	void Executer::disableAction(bool disable)
	{
		mActionEnabled[mCurrentActionIndex] = !disable;
	}

	void Executer::startNextAction()
	{
		mExecutionPaused = false;

		QScriptValue script = mScriptEngine.globalObject().property("Script");
		QString nextLineString = script.property("nextLine").toString();

		bool ok;
		int nextLine = nextLineString.toInt(&ok);

		if(!ok)
		{
			nextLine = mScript->labelLine(nextLineString);

			if(nextLine == -1)
			{
				executionException(ActionTools::ActionException::CodeErrorException, tr("Unable to find the label named \"%1\"").arg(nextLineString));
				return;
			}
		}
		else
			--nextLine;

		if(nextLine < 0)//End of the script
			mCurrentActionIndex = nextLine;
		else
		{
			switch(canExecuteAction(nextLine))
			{
			case CanExecute:
				mCurrentActionIndex = nextLine;
				break;
			case IncorrectLine:
				executionException(ActionTools::ActionException::CodeErrorException, tr("Incorrect Script.nextLine value: %1").arg(nextLineString));
				return;
			case InvalidAction:
				executionException(ActionTools::ActionException::CodeErrorException, tr("The action at line %1 is invalid").arg(nextLineString));
				return;
			case DisabledAction:
				mCurrentActionIndex = nextLine;
				break;
			case UnselectedAction:
				mCurrentActionIndex = nextLine;
				break;
			}
		}

		executeCurrentAction();
	}

	void Executer::startActionExecution()
	{
		mExecutionPaused = false;

		mCurrentActionTimeout = mScript->actionAt(mCurrentActionIndex)->timeout();
		if(mCurrentActionTimeout > 0)
		{
			mTimeoutTimer.start();
			mTimeoutTime.start();
			mExecutionWindow->setProgressVisible(true);
			mExecutionWindow->setProgressMinimum(0);
			mExecutionWindow->setProgressMaximum(mCurrentActionTimeout);
			mExecutionWindow->setProgressValue(0);
		}
		else
			mExecutionWindow->setProgressVisible(false);

		emit actionStarted(mCurrentActionIndex, mActiveActionsCount);

		mScript->actionAt(mCurrentActionIndex)->startExecution();
	}

	void Executer::updateTimeoutProgress()
	{
		if(mTimeoutTime.elapsed() >= mCurrentActionTimeout)
		{
			mTimeoutTimer.stop();
			mScript->actionAt(mCurrentActionIndex)->disconnect();
			mScript->actionAt(mCurrentActionIndex)->stopExecution();

			executionException(ActionTools::ActionException::TimeoutException, QString());
		}

		mExecutionWindow->setProgressValue(mTimeoutTime.elapsed());
	}

	void Executer::showProgressDialog(const QString &title, int maximum)
	{
		if(!mProgressDialog)
			mProgressDialog = new QProgressDialog(0, Qt::WindowStaysOnTopHint);

		connect(mProgressDialog, SIGNAL(canceled()), this, SLOT(stopExecution()));

		mProgressDialog->setWindowTitle(title);
		mProgressDialog->setMaximum(maximum);
		mProgressDialog->setValue(0);

		mProgressDialog->show();
	}

	void Executer::updateProgressDialog(const QString &caption)
	{
		mProgressDialog->setLabelText(caption);
	}

	void Executer::updateProgressDialog(int value)
	{
		mProgressDialog->setValue(value);
	}

	void Executer::hideProgressDialog()
	{
		delete mProgressDialog;
		mProgressDialog = 0;
	}

	Executer::ExecuteActionResult Executer::canExecuteAction(const QString &line) const
	{
		bool ok;
		int nextLine = line.toInt(&ok);

		if(!ok)
			nextLine = mScript->labelLine(line);
		else
			--nextLine;

		return canExecuteAction(nextLine);
	}

	Executer::ExecuteActionResult Executer::canExecuteAction(int index) const
	{
		if(index < 0 || index >= mScript->actionCount())
			return IncorrectLine;

		ActionTools::ActionInstance *actionInstance = mScript->actionAt(index);
		if(!actionInstance)
			return InvalidAction;

		if(!mActionEnabled[index] || !actionInstance->isEnabled())
			return DisabledAction;

		if(mExecuteOnlySelection && !actionInstance->isSelected())
			return UnselectedAction;

		return CanExecute;
	}

	void Executer::executeCurrentAction()
	{
		//Skip disabled actions
		if(mCurrentActionIndex >= 0)
		{
			while(mCurrentActionIndex < mScript->actionCount() && canExecuteAction(mCurrentActionIndex) != CanExecute)
				++mCurrentActionIndex;
		}

		if(mCurrentActionIndex < 0 || mCurrentActionIndex >= mScript->actionCount())
		{
			stopExecution();
			return;
		}

		int nextLine = mCurrentActionIndex+2;
		if(nextLine > mScript->actionCount())
			nextLine = -1;

		QScriptValue script = mScriptEngine.globalObject().property("Script");
		script.setProperty("nextLine", mScriptEngine.newVariant(QVariant(nextLine)));

		ActionTools::ActionInstance *actionInstance = mScript->actionAt(mCurrentActionIndex);

		mExecutionWindow->setCurrentActionName(actionInstance->definition()->name());
		mExecutionWindow->setCurrentActionColor(actionInstance->color());

		connect(actionInstance, SIGNAL(executionEnded()), this, SLOT(actionExecutionEnded()));
		connect(actionInstance, SIGNAL(executionException(int,QString)), this, SLOT(executionException(int,QString)));
		connect(actionInstance, SIGNAL(disableAction(bool)), this, SLOT(disableAction(bool)));
		connect(actionInstance, SIGNAL(showProgressDialog(QString,int)), this, SLOT(showProgressDialog(QString,int)));
		connect(actionInstance, SIGNAL(updateProgressDialog(int)), this, SLOT(updateProgressDialog(int)));
		connect(actionInstance, SIGNAL(updateProgressDialog(QString)), this, SLOT(updateProgressDialog(QString)));
		connect(actionInstance, SIGNAL(hideProgressDialog()), this, SLOT(hideProgressDialog()));

		mStartExecutionTimer.setInterval(actionInstance->pauseBefore());
		mStartExecutionTimer.start();

		mExecutionPaused = true;
	}

	void Executer::addClassToScript(QObject *classPointer, const QString &name)
	{
		QScriptValue scriptObject = mScriptEngine.newQObject(classPointer, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
		mScriptEngine.globalObject().setProperty(name, scriptObject);
		for(int enumeratorIndex = 0; enumeratorIndex < classPointer->metaObject()->enumeratorCount(); ++enumeratorIndex)
		{
			const QMetaEnum &metaEnum = classPointer->metaObject()->enumerator(enumeratorIndex);
			QScriptValue enumObject = mScriptEngine.newObject();
			scriptObject.setProperty(metaEnum.name(), enumObject);
			for(int keyIndex = 0; keyIndex < metaEnum.keyCount(); ++keyIndex)
			{
				enumObject.setProperty(metaEnum.key(keyIndex), metaEnum.value(keyIndex));
			}
		}
	}
}
