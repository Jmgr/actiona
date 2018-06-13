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

#pragma once

#include "executer_global.h"
#include "consolewidget.h"
#include "version.h"

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QScriptEngineDebugger>

namespace ActionTools
{
	class Script;
	class ActionFactory;
	class ActionInstance;
}

class QStandardItemModel;
class QMainWindow;
class QScriptEngine;
class QProgressDialog;

namespace LibExecuter
{
	class ExecutionWindow;
	class ScriptAgent;

	class EXECUTERSHARED_EXPORT Executer : public QObject
	{
		Q_OBJECT

	public:
		Executer(QObject *parent = nullptr);
		~Executer() override ;
		
		void setup(ActionTools::Script *script,
				   ActionTools::ActionFactory *actionFactory,
				   bool showExecutionWindow,
				   int executionWindowPosition,
				   int executionWindowScreen,
				   bool showConsoleWindow,
				   int consoleWindowPosition,
				   int consoleWindowScreen,
				   int pauseBefore,
				   int pauseAfter,
                   Tools::Version actionaVersion,
				   Tools::Version scriptVersion,
				   bool isActExec,
				   QStandardItemModel *consoleModel);

		ExecutionWindow *executionWindow() const			{ return mExecutionWindow; }
		ActionTools::ConsoleWidget *consoleWidget() const	{ return mConsoleWidget; }
		ScriptAgent *scriptAgent() const					{ return mScriptAgent; }

		int currentActionIndex() const						{ return mCurrentActionIndex; }
		ActionTools::Script *script() const					{ return mScript; }
		
		static bool isExecuterRunning()						{ return (mExecutionStatus != Stopped); }

        ActionTools::ActionInstance *currentActionInstance() const;

	public slots:
        bool startExecution(bool onlySelection, const QString &filename);
		void stopExecution();
		void pauseExecution();
		void debugExecution();

	signals:
		void executionStopped();
		void scriptError(int actionIndex, const QString &parameter, const QString &error);
		void actionStarted(int actionIndex, int maxActions);
		void actionEnded(int actionIndex, int maxActions);

	private slots:
		void executionException(int exception,
								const QString &message);
		void actionExecutionEnded();
		void disableAction(bool disable);
		void startNextAction();
		void startActionExecution();
		void updateTimerProgress();
		void showProgressDialog(const QString &title, int maximum);
		void updateProgressDialog(const QString &caption);
		void updateProgressDialog(int value);
		void hideProgressDialog();
		void executionPaused();
		void executionResumed();
		void consolePrint(const QString &text);
		void consolePrintWarning(const QString &text);
		void consolePrintError(const QString &text);

	private:
		enum ExecuteActionResult
		{
			CanExecute,
			IncorrectLine,
			InvalidAction,
			DisabledAction,
			UnselectedAction
		};
		enum ExecutionStatus
		{
			Stopped,
			PrePause,
			Executing,
			PostPause
		};

		void consolePrint(const QString &text, ActionTools::ConsoleWidget::Type type);
		void pauseOrDebug(bool debug);
		ExecuteActionResult canExecuteAction(const QString &line) const;
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
		ExecutionWindow *mExecutionWindow;
		ActionTools::ConsoleWidget *mConsoleWidget;
		int mCurrentActionIndex;
		bool mExecutionStarted;
		bool mExecutionEnded;
		QScriptEngine *mScriptEngine{nullptr};
		QScriptEngineDebugger mScriptEngineDebugger;
		QMainWindow *mDebuggerWindow;
		bool mExecuteOnlySelection;
		ScriptAgent *mScriptAgent{nullptr};
		QList<bool> mActionEnabled;
		QTimer mExecutionTimer;
		QElapsedTimer mExecutionTime;
		QProgressDialog *mProgressDialog;
		int mActiveActionsCount;
		bool mExecutionPaused;
		bool mHasExecuted{false};
		static ExecutionStatus mExecutionStatus;
		bool mPauseInterrupt{false};
		int mPauseBefore;
		int mPauseAfter;
        Tools::Version mActionaVersion;
		Tools::Version mScriptVersion;
		bool mIsActExec;
        bool mShowDebuggerOnCodeError{true};

		Q_DISABLE_COPY(Executer)
	};
}

