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
#include <QTimer>
#include <QScriptContextInfo>
#include <QAction>
#include <QMainWindow>

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
	mExecuteOnlySelection(false),
	mScriptAgent(new ScriptAgent(&mScriptEngine))
{
	connect(mExecutionWindow, SIGNAL(canceled()), this, SLOT(stopExecution()));

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
	mScriptAgent->setContext(ScriptAgent::Unknown);

	mExecuteOnlySelection = onlySelection;
	mCurrentActionIndex = 0;

	mScriptEngine.setAgent(mScriptAgent);
	
	mScript->clearVariables();

	//TODO : Install actionaz specific stuff here
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
	}
	
	mScriptAgent->setContext(ScriptAgent::Parameters);

	for(int parameterIndex = 0; parameterIndex < mScript->parameterCount(); ++parameterIndex)
	{
		mScriptAgent->setCurrentParameter(parameterIndex);

		const ActionTools::ScriptParameter &scriptParameter = mScript->parameter(parameterIndex);
		QRegExp nameRegExp("[a-z_][a-z0-9_]*", Qt::CaseInsensitive);

		if(!nameRegExp.exactMatch(scriptParameter.name()))
		{
			mConsoleWidget->addScriptParameterLine(tr("Incorrect parameter name : \"%1\"").arg(scriptParameter.name()),
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
				mConsoleWidget->addScriptParameterLine(tr("Error while evaluating parameter \"%1\", error message : \"%2\"")
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

	QTimer::singleShot(0, this, SLOT(startFirstAction()));

	return true;
}

void Executer::stopExecution()
{
	if(!mExecutionStarted)
		return;

	if(mCurrentActionIndex >= 0 && mCurrentActionIndex < mScript->actionCount())
	{
		mScript->actionAt(mCurrentActionIndex)->disconnect();
		mScript->actionAt(mCurrentActionIndex)->stopExecution();
	}

	mExecutionWindow->hide();
	mConsoleWidget->hide();

	emit executionStopped();

	mExecutionStarted = false;

	mScriptEngine.abortEvaluation();
}

void Executer::startFirstAction()
{
	executeCurrentAction();
}

void Executer::executionException(ActionTools::ActionInstance::ExecutionException exceptionType,
								  const QString &message)
{
	mConsoleWidget->addActionLine(tr("Line %1 : ").arg(mCurrentActionIndex+1) + message,
								  mCurrentActionIndex,
								  mScriptEngine.evaluate("currentParameter").toString(),
								  mScriptEngine.evaluate("currentSubParameter").toString(),
								  mScriptAgent->currentLine(),
								  mScriptAgent->currentColumn(),
								  (exceptionType == ActionTools::ActionInstance::Error) ? ActionTools::ConsoleWidget::Error : ActionTools::ConsoleWidget::Warning);

	if(exceptionType == ActionTools::ActionInstance::Error)
		stopExecution();
}

void Executer::actionExecutionEnded()
{
	mScript->actionAt(mCurrentActionIndex)->disconnect();

	QTimer::singleShot(1, this, SLOT(startNextAction()));
}

void Executer::disableAction(bool disable)
{
	mActionEnabled[mCurrentActionIndex] = !disable;
}

void Executer::startNextAction()
{
	QString nextLineString = mScriptEngine.evaluate("Script.nextLine").toString();
	bool ok;
	int nextLine = nextLineString.toInt(&ok);

	if(!ok)
		nextLine = mScript->labelLine(nextLineString) + 1;
	--nextLine;

	if(nextLine == mCurrentActionIndex)
	{
		executionException(ActionTools::ActionInstance::Error, tr("Incorrect Script.nextLine value : %1").arg(nextLineString));
		return;
	}

	switch(canExecuteAction(nextLine))
	{
	case CanExecute:
		mCurrentActionIndex = nextLine;
		break;
	case IncorrectLine:
		if(nextLine < 0)
		{
			mCurrentActionIndex = nextLine;
			break;
		}

		executionException(ActionTools::ActionInstance::Error, tr("Incorrect Script.nextLine value : %1").arg(nextLineString));
		return;
	case InvalidAction:
		executionException(ActionTools::ActionInstance::Error, tr("The action at line %1 is invalid").arg(nextLineString));
		return;
	case DisabledAction:
		mCurrentActionIndex = nextLine;
		break;
	case UnselectedAction:
		mCurrentActionIndex = nextLine;
		break;
	}

	executeCurrentAction();
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
	connect(actionInstance, SIGNAL(executionException(ActionTools::ActionInstance::ExecutionException,QString)), this, SLOT(executionException(ActionTools::ActionInstance::ExecutionException,QString)));
	connect(actionInstance, SIGNAL(disableAction(bool)), this, SLOT(disableAction(bool)));

	actionInstance->startExecution();
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
