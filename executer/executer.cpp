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

#include "executer.h"
#include "script.h"
#include "actionfactory.h"
#include "executionwindow.h"
#include "codeinitializer.h"
#include "actiondefinition.h"
#include "scriptagent.h"
#include "actioninstance.h"
#include "code/codetools.h"
#include "codeactionaz.h"

#include <QDesktopWidget>
#include <QAction>
#include <QMainWindow>
#include <QApplication>
#include <QLocale>
#include <QProgressDialog>
#include <QScriptEngine>

namespace LibExecuter
{
	Executer::ExecutionStatus Executer::mExecutionStatus = Executer::Stopped;
	
	Executer::Executer(QObject *parent)
		: QObject(parent),
		mExecutionWindow(new ExecutionWindow()),
		mConsoleWidget(new ActionTools::ConsoleWidget()),
		mScriptEngine(0),
		mScriptAgent(0),
		mHasExecuted(false),
		mPauseInterrupt(false)
	{
		connect(mExecutionWindow, SIGNAL(canceled()), this, SLOT(stopExecution()));
		connect(mExecutionWindow, SIGNAL(paused()), this, SLOT(pauseExecution()));
		connect(mExecutionWindow, SIGNAL(debug()), this, SLOT(debugExecution()));
		connect(&mExecutionTimer, SIGNAL(timeout()), this, SLOT(updateTimerProgress()));
		connect(&mScriptEngineDebugger, SIGNAL(evaluationSuspended()), mExecutionWindow, SLOT(onEvaluationPaused()));
		connect(&mScriptEngineDebugger, SIGNAL(evaluationResumed()), mExecutionWindow, SLOT(onEvaluationResumed()));
		connect(&mScriptEngineDebugger, SIGNAL(evaluationSuspended()), this, SLOT(executionPaused()));
		connect(&mScriptEngineDebugger, SIGNAL(evaluationResumed()), this, SLOT(executionResumed()));

		mScriptEngineDebugger.setAutoShowStandardWindow(false);

		mConsoleWidget->setWindowFlags(Qt::Tool |
					   Qt::WindowStaysOnTopHint |
					   Qt::CustomizeWindowHint |
					   Qt::WindowTitleHint);
	}

	Executer::~Executer()
	{
		delete mExecutionWindow;
		delete mConsoleWidget;
	}
	
	void Executer::setup(ActionTools::Script *script,
			   ActionTools::ActionFactory *actionFactory,
			   bool showExecutionWindow,
			   int executionWindowPosition,
			   int executionWindowScreen,
			   bool showConsoleWindow,
			   int consoleWindowPosition,
			   int consoleWindowScreen,
			   int pauseBefore,
			   int pauseAfter,
			   Tools::Version actionazVersion,
			   Tools::Version scriptVersion,
			   bool isActExec,
			   QStandardItemModel *consoleModel)
	{
		mScript = script;
		mScriptEngine = new QScriptEngine(this);

		foreach(QString extension, mScriptEngine->availableExtensions())
			mScriptEngine->importExtension(extension);
		
		mActionFactory = actionFactory;
		mShowExecutionWindow = showExecutionWindow;
		mExecutionWindowPosition = executionWindowPosition;
		mExecutionWindowScreen = executionWindowScreen;
		mShowConsoleWindow = showConsoleWindow;
		mConsoleWindowPosition = consoleWindowPosition;
		mConsoleWindowScreen = consoleWindowScreen;
		mPauseBefore = pauseBefore;
		mPauseAfter = pauseAfter;
		mCurrentActionIndex = 0;
		mExecutionStarted = false;
		mExecutionEnded = false;
		mExecuteOnlySelection = false;
		mProgressDialog = 0;
		mActiveActionsCount = 0;
		mExecutionPaused = false;
		mActionazVersion = actionazVersion;
		mScriptVersion = scriptVersion;
		mIsActExec = isActExec;
		
		mScriptEngineDebugger.attachTo(mScriptEngine);
		mDebuggerWindow = mScriptEngineDebugger.standardWindow();

		mScriptAgent = new ScriptAgent(mScriptEngine);

		connect(mScriptAgent, SIGNAL(executionStopped()), this, SLOT(stopExecution()));
		connect(mScriptAgent, SIGNAL(evaluationStarted()), mExecutionWindow, SLOT(enableDebug()));
		connect(mScriptAgent, SIGNAL(evaluationStopped()), mExecutionWindow, SLOT(disableDebug()));

		QScriptEngineAgent *debuggerAgent = mScriptEngine->agent();
		mScriptEngine->setAgent(mScriptAgent);
		mScriptAgent->setDebuggerAgent(debuggerAgent);
		
		mConsoleWidget->setup(consoleModel);
		
		mExecutionTimer.setSingleShot(false);
		mExecutionTimer.setInterval(5);
		mConsoleWidget->updateClearButton();
	}
	
	void printCall(QScriptContext *context, ActionTools::ConsoleWidget::Type type)
	{
		QApplication::processEvents();//Call this to prevent UI freeze when calling print often

		QScriptValue calleeData = context->callee().data();
		Executer *executer = qobject_cast<Executer *>(calleeData.toQObject());
		QString message;
		ScriptAgent *agent = executer->scriptAgent();

		if(!agent)
			return;
		
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
												   context->engine()->property("currentParameter").toString(),
												   context->engine()->property("currentSubParameter").toString(),
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
		if(!Executer::isExecuterRunning())
			return QScriptValue();

		if(context->argumentCount() < 1)
			return engine->undefinedValue();

		printCall(context, ActionTools::ConsoleWidget::Information);

		return engine->undefinedValue();
	}

	QScriptValue printWarningFunction(QScriptContext *context, QScriptEngine *engine)
	{
		if(!Executer::isExecuterRunning())
			return QScriptValue();

		if(context->argumentCount() < 1)
			return engine->undefinedValue();

		printCall(context, ActionTools::ConsoleWidget::Warning);

		return engine->undefinedValue();
	}

	QScriptValue printErrorFunction(QScriptContext *context, QScriptEngine *engine)
	{
		if(!Executer::isExecuterRunning())
			return QScriptValue();

		if(context->argumentCount() < 1)
			return engine->undefinedValue();

		printCall(context, ActionTools::ConsoleWidget::Error);

		return engine->undefinedValue();
	}

	bool Executer::startExecution(bool onlySelection)
	{
		Q_ASSERT(mScriptAgent);
		Q_ASSERT(mScriptEngine);
		
	#ifdef ACT_PROFILE
		Tools::HighResolutionTimer timer("Executer::startExecution");
	#endif

		Code::CodeTools::addClassToScriptEngine<CodeActionaz>("Actionaz", mScriptEngine);
		CodeActionaz::setActExec(mIsActExec);
		CodeActionaz::setActionazVersion(mActionazVersion);
		CodeActionaz::setScriptVersion(mScriptVersion);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Actionaz", &CodeActionaz::version, "version", mScriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Actionaz", &CodeActionaz::scriptVersion, "scriptVersion", mScriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Actionaz", &CodeActionaz::isActExec, "isActExec", mScriptEngine);
		Code::CodeTools::addClassGlobalFunctionToScriptEngine("Actionaz", &CodeActionaz::isActionaz, "isActionaz", mScriptEngine);
		
		mScriptAgent->setContext(ScriptAgent::ActionInit);
		CodeInitializer::initialize(mScriptEngine, mScriptAgent, mActionFactory);
		mScriptAgent->setContext(ScriptAgent::Parameters);
		
		QScriptValue script = mScriptEngine->newObject();
		mScriptEngine->globalObject().setProperty("Script", script, QScriptValue::ReadOnly);
		script.setProperty("nextLine", mScriptEngine->newVariant(QVariant(1)));

		QScriptValue console = mScriptEngine->newObject();
		mScriptEngine->globalObject().setProperty("Console", console, QScriptValue::ReadOnly);

		QScriptValue printFun = mScriptEngine->newFunction(printFunction);
		printFun.setData(mScriptEngine->newQObject(this));
		console.setProperty("print", printFun);

		printFun = mScriptEngine->newFunction(printWarningFunction);
		printFun.setData(mScriptEngine->newQObject(this));
		console.setProperty("printWarning", printFun);

		printFun = mScriptEngine->newFunction(printErrorFunction);
		printFun.setData(mScriptEngine->newQObject(this));
		console.setProperty("printError", printFun);
		
		mExecuteOnlySelection = onlySelection;
		mCurrentActionIndex = 0;
		mActiveActionsCount = 0;
		mExecutionPaused = false;

		bool initSucceeded = true;
		int lastBeginProcedure = -1;

		mScript->clearProcedures();
		mScript->clearCallStack();

		for(int actionIndex = 0; actionIndex < mScript->actionCount(); ++actionIndex)
		{
			ActionTools::ActionInstance *actionInstance = mScript->actionAt(actionIndex);
			actionInstance->reset();
			actionInstance->clearRuntimeParameters();
			actionInstance->setupExecution(mScriptEngine, mScript, actionIndex);
			mActionEnabled.append(true);

			if(canExecuteAction(actionIndex) == CanExecute)
			{
				++mActiveActionsCount;

				if(actionInstance->definition()->id() == "ActionBeginProcedure")
				{
					if(lastBeginProcedure != -1)
					{
						mConsoleWidget->addActionLine(tr("Invalid Begin procedure action, you have to end the previous procedure before starting another one"), actionIndex, QString(), QString(), -1, -1, ActionTools::ConsoleWidget::Error);

						return false;
					}

					lastBeginProcedure = actionIndex;

					const ActionTools::SubParameter &nameParameter = actionInstance->subParameter("name", "value");
					const QString &procedureName = nameParameter.value().toString();

					if(procedureName.isEmpty())
					{
						mConsoleWidget->addActionLine(tr("A procedure name cannot be empty"), actionIndex, QString(), QString(), -1, -1, ActionTools::ConsoleWidget::Error);

						return false;
					}

					if(mScript->findProcedure(procedureName) != -1)
					{
						mConsoleWidget->addActionLine(tr("A procedure with the name \"%1\" has already been declared").arg(procedureName), actionIndex, QString(), QString(), -1, -1, ActionTools::ConsoleWidget::Error);

						return false;
					}

					mScript->addProcedure(procedureName, actionIndex);
				}
				else if(actionInstance->definition()->id() == "ActionEndProcedure")
				{
					if(lastBeginProcedure == -1)
					{
						mConsoleWidget->addActionLine(tr("Invalid End procedure"), actionIndex, QString(), QString(), -1, -1, ActionTools::ConsoleWidget::Error);

						return false;
					}

					ActionTools::ActionInstance *beginProcedureActionInstance = mScript->actionAt(lastBeginProcedure);

					actionInstance->setRuntimeParameter("procedureBeginLine", lastBeginProcedure);
					beginProcedureActionInstance->setRuntimeParameter("procedureEndLine", actionIndex);

					lastBeginProcedure = -1;
				}
			}
		}

		if(lastBeginProcedure != -1)
		{
			mConsoleWidget->addActionLine(tr("Begin procedure action without end procedure"), lastBeginProcedure, QString(), QString(), -1, -1, ActionTools::ConsoleWidget::Error);

			return false;
		}

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
				QScriptValue result = mScriptEngine->evaluate(scriptParameter.value());
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

			mScriptEngine->globalObject().setProperty(scriptParameter.name(), value, QScriptValue::ReadOnly | QScriptValue::Undeletable);
		}

		if(!initSucceeded || mScript->actionCount() == 0)
			return false;

		if(mShowExecutionWindow)
		{
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

			mExecutionWindow->setPauseStatus(false);
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
		
		mHasExecuted = true;

		executeCurrentAction();

		return true;
	}

	void Executer::stopExecution()
	{
		if(!mExecutionStarted)
			return;
		
		mScriptAgent->pause(false);
		mScriptAgent->stopExecution(false);
		mScriptEngineDebugger.action(QScriptEngineDebugger::ContinueAction)->trigger();
		
		mExecutionStarted = false;
		mExecutionStatus = Stopped;
		mScriptEngine->abortEvaluation();

		mExecutionTimer.stop();

		if(mCurrentActionIndex >= 0 && mCurrentActionIndex < mScript->actionCount())
		{
			currentActionInstance()->disconnect();
			if(!mExecutionEnded)
				currentActionInstance()->stopExecution();
		}

		for(int actionIndex = 0; actionIndex < mScript->actionCount(); ++actionIndex)
			mScript->actionAt(actionIndex)->stopLongTermExecution();

		mScriptEngineDebugger.detach();
		
		mScriptAgent->deleteLater();
		mScriptAgent = 0;
		mScriptEngine->deleteLater();
		mScriptEngine = 0;

		delete mProgressDialog;
		mProgressDialog = 0;
		mDebuggerWindow->hide();
		mExecutionWindow->hide();
		mConsoleWidget->hide();

		emit executionStopped();
	}
	
	void Executer::pauseExecution()
	{
		pauseOrDebug(false);
	}

	void Executer::debugExecution()
	{
		pauseOrDebug(true);
	}

	void Executer::executionException(int exception,
									  const QString &message)
	{
		ActionTools::ActionInstance *actionInstance = currentActionInstance();
		bool standardException = (exception >= 0 && exception < ActionTools::ActionException::ExceptionCount);
		bool customException = false;

		foreach(ActionTools::ActionException *actionException, actionInstance->definition()->exceptions())
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
											   .arg(actionInstance->definition()->name())
											   .arg(mCurrentActionIndex+1), ActionTools::ConsoleWidget::Error);
			stopExecution();
			return;
		}

		ActionTools::ActionException::ExceptionActionInstance exceptionActionInstance = actionInstance->exceptionActionInstance(static_cast<ActionTools::ActionException::Exception>(exception));
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
					QScriptValue script = mScriptEngine->globalObject().property("Script");
					script.setProperty("nextLine", mScriptEngine->newVariant(QVariant(exceptionActionInstance.line())));
					actionExecutionEnded();
					shouldStopExecution = false;
				}
			}
			break;
		default:
			exceptionType = ActionTools::ConsoleWidget::Error;

			shouldStopExecution = true;
		}

		if(shouldStopExecution)
		{
			QString currentFile = mScriptAgent->currentFile();
			QString finalMessage;
			if(currentFile.isEmpty())
				finalMessage = tr("Script line %1: ").arg(mCurrentActionIndex+1);
			else
				finalMessage = tr("Script %1, line %2: ").arg(currentFile).arg(mCurrentActionIndex+1);

			mConsoleWidget->addActionLine(finalMessage + message,
										mCurrentActionIndex,
										mScriptEngine->property("currentParameter").toString(),
										mScriptEngine->property("currentSubParameter").toString(),
										mScriptAgent->currentLine(),
										mScriptAgent->currentColumn(),
										exceptionType);

			stopExecution();
		}
	}

	void Executer::actionExecutionEnded()
	{
		mExecutionTimer.stop();
		currentActionInstance()->disconnect();

		emit actionEnded(mCurrentActionIndex, mActiveActionsCount);
		
		mExecutionStatus = PostPause;

		mExecutionTimer.start();
		mExecutionTime = 0;
		if(currentActionInstance()->pauseAfter() + mPauseAfter > 0)
		{
			mExecutionWindow->setProgressEnabled(true);
			mExecutionWindow->setProgressMinimum(0);
			mExecutionWindow->setProgressMaximum(currentActionInstance()->pauseAfter() + mPauseAfter);
			mExecutionWindow->setProgressValue(0);
		}
		else
			mExecutionWindow->setProgressEnabled(false);
		
		mExecutionEnded = true;
	}

	void Executer::disableAction(bool disable)
	{
		mActionEnabled[mCurrentActionIndex] = !disable;
	}

	void Executer::startNextAction()
	{
		mExecutionEnded = false;

		QScriptValue script = mScriptEngine->globalObject().property("Script");
		QString nextLineString = script.property("nextLine").toString();
		int previousLine = mCurrentActionIndex;

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
			--nextLine;//Make the nextLine value 0-based instead of 1-based

		if(nextLine < 0 || nextLine == mScript->actionCount())//End of the script
			mCurrentActionIndex = nextLine;
		else
		{
			switch(canExecuteAction(nextLine))
			{
			case IncorrectLine:
				executionException(ActionTools::ActionException::CodeErrorException, tr("Incorrect Script.nextLine value: %1").arg(nextLineString));
				return;
			case InvalidAction:
				executionException(ActionTools::ActionException::CodeErrorException, tr("The action at line %1 is invalid").arg(nextLineString));
				return;
			case DisabledAction:
			case UnselectedAction:
			case CanExecute:
				mCurrentActionIndex = nextLine;
				break;
			}
		}

		if(mCurrentActionIndex >= 0)
		{
			for(int actionIndex = mCurrentActionIndex; actionIndex < previousLine; ++actionIndex)
				mScript->actionAt(actionIndex)->reset();
		}

		executeCurrentAction();
	}

	void Executer::startActionExecution()
	{
		mExecutionStatus = Executing;
		
		mExecutionEnded = false;

		int actionTimeout = currentActionInstance()->timeout();
		if(actionTimeout > 0)
		{
			mExecutionTimer.start();
			mExecutionTime = 0;
			mExecutionWindow->setProgressEnabled(true);
			mExecutionWindow->setProgressMinimum(0);
			mExecutionWindow->setProgressMaximum(actionTimeout);
			mExecutionWindow->setProgressValue(0);
		}
		else
			mExecutionWindow->setProgressEnabled(false);

		emit actionStarted(mCurrentActionIndex, mActiveActionsCount);

		currentActionInstance()->startExecution();
	}

	void Executer::updateTimerProgress()
	{
		if(mExecutionPaused)
			return;
		
		mExecutionTime += mExecutionTimer.interval();
		
		ActionTools::ActionInstance *actionInstance = currentActionInstance();
		switch(mExecutionStatus)
		{
		case PrePause:
			if(mExecutionTime >= actionInstance->pauseBefore() + mPauseBefore)
			{
				mExecutionTimer.stop();
				startActionExecution();
			}
			mExecutionWindow->setProgressValue(mExecutionTime);
			break;
		case Executing://Timeout
			if(mExecutionTime >= actionInstance->timeout())
			{
				mExecutionTimer.stop();
				actionInstance->disconnect();
				actionInstance->stopExecution();
	
				executionException(ActionTools::ActionException::TimeoutException, QString());
			}
			mExecutionWindow->setProgressValue(mExecutionTime);
			break;
		case PostPause:
			if(mExecutionTime >= actionInstance->pauseAfter() + mPauseAfter)
			{
				mExecutionTimer.stop();
				startNextAction();
			}
			mExecutionWindow->setProgressValue(mExecutionTime);
			break;
		default:
			Q_ASSERT(false && "updateTimerProgress() called, but execution is stopped");
			break;
		}
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

	void Executer::executionPaused()
	{
		mExecutionPaused = true;

		if(!mPauseInterrupt)
			mDebuggerWindow->show();
		else
			mPauseInterrupt = false;
	}

	void Executer::executionResumed()
	{
		mExecutionPaused = false;
	}

	void Executer::consolePrint(const QString &text)
	{
		consolePrint(text, ActionTools::ConsoleWidget::Information);
	}

	void Executer::consolePrintWarning(const QString &text)
	{
		consolePrint(text, ActionTools::ConsoleWidget::Warning);
	}

	void Executer::consolePrintError(const QString &text)
	{
		consolePrint(text, ActionTools::ConsoleWidget::Error);
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

	void Executer::consolePrint(const QString &text, ActionTools::ConsoleWidget::Type type)
	{
		consoleWidget()->addUserLine(text,
									   currentActionIndex(),
									   mScriptEngine->currentContext()->engine()->property("currentParameter").toString(),
									   mScriptEngine->currentContext()->engine()->property("currentSubParameter").toString(),
									   mScriptAgent->currentLine(),
									   mScriptAgent->currentColumn(),
									   mScriptEngine->currentContext()->backtrace(),
									   type);
	}

	void Executer::pauseOrDebug(bool debug)
	{
		if(mExecutionStatus == Stopped)
			return;

		mExecutionPaused = !mExecutionPaused;

		mPauseInterrupt = !debug;

		if(mScriptEngine->isEvaluating())
		{
			if(mExecutionPaused)
			{
				mScriptEngineDebugger.action(QScriptEngineDebugger::InterruptAction)->trigger();

				if(debug)
					mDebuggerWindow->show();
			}
			else
			{
				mScriptEngineDebugger.action(QScriptEngineDebugger::ContinueAction)->trigger();

				if(debug)
					mDebuggerWindow->hide();
			}

			mScriptAgent->pause(mExecutionPaused);
		}
		else
		{
			ActionTools::ActionInstance *currentAction = currentActionInstance();
			if(currentAction)
			{
				if(mExecutionPaused)
					currentAction->pauseExecution();
				else
					currentAction->resumeExecution();
			}
		}

		mExecutionWindow->setPauseStatus(mExecutionPaused);
	}
	
	ActionTools::ActionInstance *Executer::currentActionInstance() const
	{
		if(mCurrentActionIndex < 0 || mCurrentActionIndex >= mScript->actionCount())
			return 0;
		
		return mScript->actionAt(mCurrentActionIndex);
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

		QScriptValue script = mScriptEngine->globalObject().property("Script");
		script.setProperty("nextLine", mScriptEngine->newVariant(QVariant(nextLine)));

		ActionTools::ActionInstance *actionInstance = currentActionInstance();

		mExecutionWindow->setCurrentActionName(actionInstance->definition()->name());
		mExecutionWindow->setCurrentActionColor(actionInstance->color());

		connect(actionInstance, SIGNAL(executionEnded()), this, SLOT(actionExecutionEnded()));
		connect(actionInstance, SIGNAL(executionException(int,QString)), this, SLOT(executionException(int,QString)));
		connect(actionInstance, SIGNAL(disableAction(bool)), this, SLOT(disableAction(bool)));
		connect(actionInstance, SIGNAL(showProgressDialog(QString,int)), this, SLOT(showProgressDialog(QString,int)));
		connect(actionInstance, SIGNAL(updateProgressDialog(int)), this, SLOT(updateProgressDialog(int)));
		connect(actionInstance, SIGNAL(updateProgressDialog(QString)), this, SLOT(updateProgressDialog(QString)));
		connect(actionInstance, SIGNAL(hideProgressDialog()), this, SLOT(hideProgressDialog()));
		connect(actionInstance, SIGNAL(consolePrint(QString)), this, SLOT(consolePrint(QString)));
		connect(actionInstance, SIGNAL(consolePrintWarning(QString)), this, SLOT(consolePrintWarning(QString)));
		connect(actionInstance, SIGNAL(consolePrintError(QString)), this, SLOT(consolePrintError(QString)));
		
		mExecutionStatus = PrePause;

		mExecutionTimer.start();
		mExecutionTime = 0;
		if(currentActionInstance()->pauseBefore() + mPauseBefore > 0)
		{
			mExecutionWindow->setProgressEnabled(true);
			mExecutionWindow->setProgressMinimum(0);
			mExecutionWindow->setProgressMaximum(currentActionInstance()->pauseBefore() + mPauseBefore);
			mExecutionWindow->setProgressValue(0);
		}
		else
			mExecutionWindow->setProgressEnabled(false);

		mExecutionEnded = true;
	}
}
