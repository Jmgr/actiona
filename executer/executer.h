#ifndef EXECUTER_H
#define EXECUTER_H

#include "executer_global.h"
#include "action.h"
#include "scriptagent.h"

#include <QObject>
#include <QScriptEngine>
#include <QPoint>
#include <QKeySequence>

namespace ActionTools
{
	class Script;
	class ActionFactory;
	class ConsoleWidget;
}

class ExecutionWindow;
class QStandardItemModel;

class EXECUTERSHARED_EXPORT Executer : public QObject
{
	Q_OBJECT

public:
	Executer(ActionTools::Script *script,
			 ActionTools::ActionFactory *actionFactory,
			 bool showExecutionWindow,
			 int executionWindowPosition,
			 int executionWindowScreen,
			 bool showConsoleWindow,
			 int consoleWindowPosition,
			 int consoleWindowScreen,
			 const QKeySequence &stopExecutionHotkey,
			 QStandardItemModel *consoleModel,
			 QObject *parent = 0);
	~Executer();

	ExecutionWindow *executionWindow() const			{ return mExecutionWindow; }
	ActionTools::ConsoleWidget *consoleWidget() const		{ return mConsoleWidget; }
	ScriptAgent *scriptAgent() const				{ return mScriptAgent; }

	int currentActionIndex() const					{ return mCurrentActionIndex; }
	ActionTools::Script *script() const				{ return mScript; }

public slots:
	bool startExecution(bool onlySelection);
	void stopExecution();

signals:
	void executionStopped();
	void scriptError(int actionIndex, const QString &parameter, const QString &error);
	void actionStarted(int actionIndex);
	void actionEnded(int actionIndex);

private slots:
	void startFirstAction();
	void executionException(ActionTools::Action::ExecutionException exceptionType,
							const QString &message);
	void actionExecutionEnded();
	void startNextAction();

private:
	enum ExecuteActionResult
	{
		CanExecute,
		IncorrectLine,
		InvalidAction,
		DisabledAction,
		UnselectedAction
	};

	ExecuteActionResult canExecuteAction(int index) const;
	void executeCurrentAction();

	ActionTools::Script *mScript;
	ActionTools::ActionFactory *mActionFactory;
	bool mShowExecutionWindow;
	int mExecutionWindowPosition;
	int mExecutionWindowScreen;
	bool mShowConsoleWindow;
	int mConsoleWindowPosition;
	int mConsoleWindowScreen;
	QKeySequence mStopExecutionShortcut;
	ExecutionWindow *mExecutionWindow;
	ActionTools::ConsoleWidget *mConsoleWidget;
	int mCurrentActionIndex;
	bool mExecutionStarted;
	QScriptEngine mScriptEngine;
	bool mExecuteOnlySelection;
	ScriptAgent *mScriptAgent;

	Q_DISABLE_COPY(Executer)
};

#endif // EXECUTER_H
