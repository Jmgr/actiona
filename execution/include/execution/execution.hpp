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

#include "execution_global.hpp"
#include "actiontools/consolewidget.hpp"

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
#include <QVersionNumber>

namespace ActionTools
{
	class Script;
	class ActionFactory;
	class ActionInstance;
    class ScriptEngine;
    class RuntimeStorage;
    class ElementDefinition;
}

class QStandardItemModel;
class QMainWindow;
class QProgressDialog;

namespace Execution
{
	class ExecutionWindow;

	class EXECUTIONSHARED_EXPORT Executer : public QObject
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
                   QVersionNumber actionaVersion,
                   QVersionNumber scriptVersion,
                   bool isActExec,
                   QStandardItemModel *consoleModel,
                   int consoleMaxEntries);

		ExecutionWindow *executionWindow() const			{ return mExecutionWindow; }
		ActionTools::ConsoleWidget *consoleWidget() const	{ return mConsoleWidget; }

		int currentActionIndex() const						{ return mCurrentActionIndex; }
		ActionTools::Script *script() const					{ return mScript; }

        ActionTools::ScriptEngine &scriptEngine() const     { return *mScriptEngine; }

        void setCurrentParameter(int parameter) { mCurrentParameter = parameter; }
        int currentParameter() const { return mCurrentParameter; }
		
        bool isExecuterRunning() const                      { return (mExecutionStatus != Stopped); }

        ActionTools::ActionInstance *currentActionInstance() const;

	public slots:
        bool startExecution(bool onlySelection, const QString &filename);
		void stopExecution();
		void pauseExecution();
		void debugExecution();

	signals:
		void executionStopped();
		void scriptError(int actionIndex, const QString &parameter, const QString &error);

	private slots:
		void executionException(int exception,
								const QString &message);
        void actionExecutionEnded(bool stopScript = false);
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
            UnselectedAction,
            CannotJumpInsideProcedure,
            CannotJumpOutsideProcedure,
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
        std::optional<int> lineToActionIndex(const QString &line) const;
        void detectJumpsInActionParameter(
            ActionTools::ActionInstance *actionInstance,
            int actionIndex,
            ActionTools::ElementDefinition *elementDefinition,
            QList<std::tuple<int, int>> &jumps) const;
        bool jumpsSanityChecks(const QList<std::tuple<int, int>> &procedures) const;

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
        ActionTools::ScriptEngine *mScriptEngine{nullptr};
        bool mIsEvaluatingScript{false};
		bool mExecuteOnlySelection;
		QList<bool> mActionEnabled;
		QTimer mExecutionTimer;
		QElapsedTimer mExecutionTime;
		QProgressDialog *mProgressDialog;
		int mActiveActionsCount;
		bool mExecutionPaused;
        bool mHasExecuted{false};
        ExecutionStatus mExecutionStatus{Stopped};
		bool mPauseInterrupt{false};
        QVersionNumber mActionaVersion;
		QVersionNumber mScriptVersion;
		bool mIsActExec;
        bool mShowDebuggerOnCodeError{true};
        int mCurrentParameter{0};
        std::unordered_map<QString, std::unique_ptr<ActionTools::RuntimeStorage>> mRuntimeStorage;
        QList<std::tuple<int, int>> mProcedures;

		Q_DISABLE_COPY(Executer)
	};
}

