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

#include "execution.hpp"
#include "actiontools/script.hpp"
#include "actiontools/actionfactory.hpp"
#include "executionwindow.hpp"
#include "codeinitializer.hpp"
#include "actiontools/actiondefinition.hpp"
#include "actiontools/actionexception.hpp"
#include "actiontools/actioninstance.hpp"
#include "actiontools/numberformat.hpp"
#include "actiontools/code/image.hpp"
#include "actiontools/code/rawdata.hpp"
#include "actiontools/lineparameterdefinition.hpp"
#include "actiontools/ifactionparameterdefinition.hpp"
#include "actiontools/groupdefinition.hpp"
#include "codeactiona.hpp"
#include "codescript.hpp"
#include "codeconsole.hpp"
#include "actiontools/scriptengine.hpp"

#ifdef ACT_PROFILE
#include "tools/highresolutiontimer.hpp"
#endif

#include <QAction>
#include <QMainWindow>
#include <QApplication>
#include <QLocale>
#include <QProgressDialog>
#include <QJSValueIterator>
#include <QScreen>
#include <QRegularExpression>

namespace Execution
{
	Executer::Executer(QObject *parent)
		: QObject(parent),
		mExecutionWindow(new ExecutionWindow()),
		mConsoleWidget(new ActionTools::ConsoleWidget())
        
	{
        mExecutionTimer.setTimerType(Qt::PreciseTimer);

        connect(mExecutionWindow, &ExecutionWindow::canceled, this, &Executer::stopExecution);
        connect(mExecutionWindow, &ExecutionWindow::paused, this, &Executer::pauseExecution);
        connect(mExecutionWindow, &ExecutionWindow::debug, this, &Executer::debugExecution);
        connect(&mExecutionTimer, &QTimer::timeout, this, &Executer::updateTimerProgress);

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
               QVersionNumber actionaVersion,
			   QVersionNumber scriptVersion,
			   bool isActExec,
               QStandardItemModel *consoleModel,
               int consoleMaxEntries)
	{
		mScript = script;
        mScriptEngine = new ActionTools::ScriptEngine(this);

		mActionFactory = actionFactory;
		mShowExecutionWindow = showExecutionWindow;
		mExecutionWindowPosition = executionWindowPosition;
		mExecutionWindowScreen = executionWindowScreen;
		mShowConsoleWindow = showConsoleWindow;
		mConsoleWindowPosition = consoleWindowPosition;
		mConsoleWindowScreen = consoleWindowScreen;
		mCurrentActionIndex = 0;
		mExecutionStarted = false;
		mExecutionEnded = false;
		mExecuteOnlySelection = false;
		mProgressDialog = nullptr;
		mActiveActionsCount = 0;
		mExecutionPaused = false;
        mActionaVersion = actionaVersion;
		mScriptVersion = scriptVersion;
		mIsActExec = isActExec;
        mRuntimeStorage.clear();

        mConsoleWidget->setup(consoleModel, consoleMaxEntries);
		
		mExecutionTimer.setSingleShot(false);
        mExecutionTimer.setInterval(1);
		mConsoleWidget->updateClearButton();
	}
	
    ActionTools::ActionInstance *Executer::currentActionInstance() const
    {
        if(mCurrentActionIndex < 0 || mCurrentActionIndex >= mScript->actionCount())
            return nullptr;

        return mScript->actionAt(mCurrentActionIndex);
    }

    bool Executer::startExecution(bool onlySelection, const QString &filename)
	{
		Q_ASSERT(mScriptEngine);

	#ifdef ACT_PROFILE
        Tools::HighResolutionTimer timer(QStringLiteral("Executer::startExecution"));
	#endif

        CodeActiona::registerClass(*mScriptEngine);
        CodeActiona::setActExec(mIsActExec);
        CodeActiona::setActionaVersion(mActionaVersion);
        CodeActiona::setScriptVersion(mScriptVersion);

        mScriptEngine->setContext(ActionTools::ScriptEngine::ActionInit);
        CodeInitializer::initialize(*mScriptEngine, mActionFactory, filename);
        mScriptEngine->setContext(ActionTools::ScriptEngine::Parameters);

        CodeScript::registerClass(*mScriptEngine, *this);
        CodeConsole::registerClass(*mScriptEngine, *this);

		mExecuteOnlySelection = onlySelection;
		mCurrentActionIndex = 0;
		mActiveActionsCount = 0;
		mExecutionPaused = false;

		bool initSucceeded = true;
		int lastBeginProcedure = -1;

		mScript->clearProcedures();
		mScript->clearCallStack();

		const QMap<QString, ActionTools::Resource> &resources = mScript->resources();
        const auto keys = resources.keys();
        for(const QString &key: keys)
        {
            const ActionTools::Resource &resource = resources.value(key);
            QJSValue value;

            switch(resource.type())
            {
            case ActionTools::Resource::BinaryType:
            case ActionTools::Resource::TypeCount:
                    value = Code::CodeClass::construct<Code::RawData>(resource.data(), *mScriptEngine);
                break;
            case ActionTools::Resource::TextType:
				value = QString::fromUtf8(resource.data());
                break;
            case ActionTools::Resource::ImageType:
                {
                    QImage image;

                    if(!image.loadFromData(resource.data()))
                    {
                        mConsoleWidget->addResourceLine(ActionTools::ConsoleWidget::Error, tr("Invalid image resource"), key);

                        return false;
                    }

                    value = Code::CodeClass::construct<Code::Image>(image, *mScriptEngine);
                }
                break;
            }

            mScriptEngine->globalObject().setProperty(key, value);
        }

        mProcedures.clear();

		for(int actionIndex = 0; actionIndex < mScript->actionCount(); ++actionIndex)
		{
			ActionTools::ActionInstance *actionInstance = mScript->actionAt(actionIndex);
			actionInstance->reset();
            actionInstance->clearRuntimeParameters();

            auto key = actionInstance->definition()->id();
            auto it = mRuntimeStorage.find(key);
            if (it == mRuntimeStorage.end()) {
                auto storage = std::make_unique<ActionTools::RuntimeStorage>();
                it = mRuntimeStorage.emplace(key, std::move(storage)).first;
            }

            actionInstance->setupExecution(mScriptEngine, mScript, actionIndex, it->second.get());
			mActionEnabled.append(true);

			qint64 currentActionRuntimeId = -1;
			if(actionInstance)
				currentActionRuntimeId = actionInstance->runtimeId();

			if(canExecuteAction(actionIndex) == CanExecute)
			{
				++mActiveActionsCount;

				if(actionInstance->definition()->id() == QLatin1String("ActionBeginProcedure"))
				{
					if(lastBeginProcedure != -1)
					{
                        mConsoleWidget->addActionLine(ActionTools::ConsoleWidget::Error, tr("Invalid Begin procedure action, you have to end the previous procedure before starting another one"), currentActionRuntimeId);

						return false;
					}

					lastBeginProcedure = actionIndex;

					const ActionTools::SubParameter &nameParameter = actionInstance->subParameter(QStringLiteral("name"), QStringLiteral("value"));
                    const QString &procedureName = nameParameter.value();

					if(procedureName.isEmpty())
					{
                        mConsoleWidget->addActionLine(ActionTools::ConsoleWidget::Error, tr("A procedure name cannot be empty"), currentActionRuntimeId);

						return false;
					}

					if(mScript->findProcedure(procedureName) != -1)
					{
                        mConsoleWidget->addActionLine(ActionTools::ConsoleWidget::Error, tr("A procedure with the name \"%1\" has already been declared").arg(procedureName), currentActionRuntimeId);

						return false;
					}

					mScript->addProcedure(procedureName, actionIndex);
				}
				else if(actionInstance->definition()->id() == QLatin1String("ActionEndProcedure"))
				{
					if(lastBeginProcedure == -1)
					{
                        mConsoleWidget->addActionLine(ActionTools::ConsoleWidget::Error, tr("Invalid End procedure"), currentActionRuntimeId);

						return false;
					}

					ActionTools::ActionInstance *beginProcedureActionInstance = mScript->actionAt(lastBeginProcedure);

					actionInstance->setRuntimeParameter(QStringLiteral("procedureBeginLine"), lastBeginProcedure);
					beginProcedureActionInstance->setRuntimeParameter(QStringLiteral("procedureEndLine"), actionIndex);

                    mProcedures.emplace_back(lastBeginProcedure, actionIndex);

					lastBeginProcedure = -1;
				}
			}
		}

		if(lastBeginProcedure != -1)
		{
			ActionTools::ActionInstance *actionInstance = mScript->actionAt(lastBeginProcedure);
			qint64 actionRuntimeId = -1;
			if(actionInstance)
				actionRuntimeId = actionInstance->runtimeId();

            mConsoleWidget->addActionLine(ActionTools::ConsoleWidget::Error, tr("Begin procedure action without end procedure"), actionRuntimeId);

			return false;
		}

        if(!jumpsSanityChecks(mProcedures))
            return false;

		for(int parameterIndex = 0; parameterIndex < mScript->parameterCount(); ++parameterIndex)
		{
            setCurrentParameter(parameterIndex);

			const ActionTools::ScriptParameter &scriptParameter = mScript->parameter(parameterIndex);
            QRegularExpression nameRegExp(QStringLiteral("[a-z_][a-z0-9_]*"), QRegularExpression::CaseInsensitiveOption);

            if(!nameRegExp.match(scriptParameter.name()).hasMatch())
			{
                mConsoleWidget->addScriptParameterLine(ActionTools::ConsoleWidget::Error, tr("Incorrect parameter name: \"%1\"").arg(scriptParameter.name()),
                                                       parameterIndex);
				initSucceeded = false;
				continue;
			}

			QString value;
			if(scriptParameter.isCode())
			{
				QJSValue result = mScriptEngine->evaluate(scriptParameter.value());
				if(result.isError())
				{
                    mConsoleWidget->addScriptParameterLine(ActionTools::ConsoleWidget::Error, tr(R"(Error while evaluating parameter "%1", error message: "%2")")
														   .arg(scriptParameter.name())
														   .arg(result.toString()),
                                                           parameterIndex);
					initSucceeded = false;
					continue;
				}
				else
					value = result.toString();
			}
			else
				value = scriptParameter.value();

            mScriptEngine->globalObject().setProperty(scriptParameter.name(), value);
		}

		if(!initSucceeded || mScript->actionCount() == 0)
			return false;

		if(mShowExecutionWindow)
		{
            auto screens = QGuiApplication::screens();
            if(mExecutionWindowScreen < 0 || mExecutionWindowScreen >= screens.size())
                return false;
            QRect screenRect = screens[mExecutionWindowScreen]->availableGeometry();
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
            auto screens = QGuiApplication::screens();
            if(mExecutionWindowScreen < 0 || mExecutionWindowScreen >= screens.size())
                return false;
            QRect screenRect = screens[mExecutionWindowScreen]->availableGeometry();
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

        mScriptEngine->setContext(ActionTools::ScriptEngine::Actions);

		mHasExecuted = true;

		executeCurrentAction();

		return true;
	}

	void Executer::stopExecution()
	{
        if(!mExecutionStarted)
			return;

		mExecutionStarted = false;
		mExecutionStatus = Stopped;

        if(mScriptEngine)
            mScriptEngine->setInterrupted(true);

		mExecutionTimer.stop();

		if(mCurrentActionIndex >= 0 && mCurrentActionIndex < mScript->actionCount())
		{
			currentActionInstance()->disconnect();
			if(!mExecutionEnded)
                currentActionInstance()->doStopExecution();
		}

        mScript->executionStopped();

        if(mScriptEngine)
        {
            mScriptEngine->deleteLater();
            mScriptEngine = nullptr;
        }

        mRuntimeStorage.clear();

		delete mProgressDialog;
		mProgressDialog = nullptr;

        // HACK: workaround display glitches if the execution is very short.
        QTimer::singleShot(100, mExecutionWindow, &QWidget::hide);
        QTimer::singleShot(100, mConsoleWidget, &QWidget::hide);

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

		for(ActionTools::ActionException *actionException: actionInstance->definition()->exceptions())
		{
			if(actionException->id() == exception)
			{
				customException = true;
				break;
			}
		}

		if(!standardException && !customException)
		{
            mConsoleWidget->addDesignErrorLine(ActionTools::ConsoleWidget::Error,
                                               tr("Action design error: Invalid exception emitted (%1, line %2)")
											   .arg(actionInstance->definition()->name())
                                               .arg(mCurrentActionIndex+1));
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
					ActionTools::ActionInstance *currentAction = mScript->actionAt(mCurrentActionIndex);
					qint64 currentActionRuntimeId = -1;
					if(currentAction)
						currentActionRuntimeId = currentAction->runtimeId();

                    mConsoleWidget->addExceptionLine(ActionTools::ConsoleWidget::Error, tr("Invalid exception line: %1").arg(exceptionActionInstance.line()),
													 currentActionRuntimeId,
                                                     exception);
					shouldStopExecution = true;
				}
				else
				{
                    QJSValue script = mScriptEngine->globalObject().property(QStringLiteral("Script"));
                    script.setProperty(QStringLiteral("nextLine"), exceptionActionInstance.line());
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
            QString finalMessage = tr("Script line %1: ").arg(mCurrentActionIndex+1);

			ActionTools::ActionInstance *currentAction = mScript->actionAt(mCurrentActionIndex);
			qint64 currentActionRuntimeId = -1;
			if(currentAction)
				currentActionRuntimeId = currentAction->runtimeId();

            auto [line, column] = mScriptEngine->currentLineAndColumn();

            mConsoleWidget->addActionLine(exceptionType, finalMessage + message,
										currentActionRuntimeId,
										mScriptEngine->globalObject().property(QStringLiteral("currentParameter")).toString(),
										mScriptEngine->globalObject().property(QStringLiteral("currentSubParameter")).toString(),
                                        line,
                                        column);

			stopExecution();
		}
	}

    void Executer::actionExecutionEnded(bool stopScript)
	{
        if(stopScript)
        {
            stopExecution();
            return;
        }

		mExecutionTimer.stop();
		currentActionInstance()->disconnect();

		mExecutionStatus = PostPause;

		mExecutionTime.start();

        auto pauseAfter = currentActionInstance()->pauseAfter() + mScript->pauseAfter();
        if(pauseAfter > 0)
		{
            mExecutionTimer.start();
            mExecutionTime.start();
			mExecutionWindow->setProgressEnabled(true);
			mExecutionWindow->setProgressMinimum(0);
            mExecutionWindow->setProgressMaximum(pauseAfter);
			mExecutionWindow->setProgressValue(0);
		}
		else
        {
			mExecutionWindow->setProgressEnabled(false);

            startNextAction();
        }
		
		mExecutionEnded = true;
	}

	void Executer::disableAction(bool disable)
	{
		mActionEnabled[mCurrentActionIndex] = !disable;
	}

	void Executer::startNextAction()
	{
		mExecutionEnded = false;

		QJSValue script = mScriptEngine->globalObject().property(QStringLiteral("Script"));
		QString nextLineString = script.property(QStringLiteral("nextLine")).toString();
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
            case CannotJumpInsideProcedure:
                executionException(ActionTools::ActionException::CodeErrorException, tr("Cannot jump inside procedure"));
                return;
            case CannotJumpOutsideProcedure:
                executionException(ActionTools::ActionException::CodeErrorException, tr("Cannot jump outside procedure"));
                return;
            }
        }

		bool doNotResetPreviousActions = script.property(QStringLiteral("doNotResetPreviousActions")).toBool();

        if(doNotResetPreviousActions)
        {
			script.setProperty(QStringLiteral("doNotResetPreviousActions"), false);
        }
        else if(mCurrentActionIndex >= 0)
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
			mExecutionTime.start();
			mExecutionWindow->setProgressEnabled(true);
			mExecutionWindow->setProgressMinimum(0);
			mExecutionWindow->setProgressMaximum(actionTimeout);
			mExecutionWindow->setProgressValue(0);
		}
		else
			mExecutionWindow->setProgressEnabled(false);

        currentActionInstance()->doStartExecution();
	}

	void Executer::updateTimerProgress()
	{
		if(mExecutionPaused)
			return;
		
		ActionTools::ActionInstance *actionInstance = currentActionInstance();
		switch(mExecutionStatus)
		{
		case PrePause:
            if(mExecutionTime.elapsed() >= actionInstance->pauseBefore() + mScript->pauseBefore())
			{
				mExecutionTimer.stop();
				startActionExecution();
			}
			mExecutionWindow->setProgressValue(mExecutionTime.elapsed());
			break;
        case Executing:
			if(mExecutionTime.elapsed() >= actionInstance->timeout())
			{
				mExecutionTimer.stop();
				actionInstance->disconnect();
                actionInstance->doStopExecution();
	
				executionException(ActionTools::ActionException::TimeoutException, QString());
			}
			mExecutionWindow->setProgressValue(mExecutionTime.elapsed());
			break;
		case PostPause:
            if(mExecutionTime.elapsed() >= actionInstance->pauseAfter() + mScript->pauseAfter())
			{
				mExecutionTimer.stop();
				startNextAction();
			}
			mExecutionWindow->setProgressValue(mExecutionTime.elapsed());
			break;
		default:
			Q_ASSERT(false && "updateTimerProgress() called, but execution is stopped");
			break;
		}
	}

	void Executer::showProgressDialog(const QString &title, int maximum)
	{
		if(!mProgressDialog)
			mProgressDialog = new QProgressDialog(nullptr, Qt::WindowStaysOnTopHint);

        connect(mProgressDialog, &QProgressDialog::canceled, this, &Executer::stopExecution);

		mProgressDialog->setWindowTitle(title);
		mProgressDialog->setMaximum(maximum);
        mProgressDialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
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
		mProgressDialog = nullptr;
	}

	void Executer::executionPaused()
	{
		mExecutionPaused = true;

        if(!mPauseInterrupt)
        {
            if(mScriptEngine)
                mScriptEngine->setInterrupted(false);
        }
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
		ActionTools::ActionInstance *currentAction = mScript->actionAt(currentActionIndex());
		qint64 currentActionRuntimeId = -1;
		if(currentAction)
			currentActionRuntimeId = currentAction->runtimeId();

        auto [line, column] = mScriptEngine->currentLineAndColumn();

        consoleWidget()->addUserLine(type, text,
									   currentActionRuntimeId,
									   mScriptEngine->globalObject().property(QStringLiteral("currentParameter")).toString(),
									   mScriptEngine->globalObject().property(QStringLiteral("currentSubParameter")).toString(),
                                       line,
                                       column);
	}

	void Executer::pauseOrDebug(bool debug)
	{
		if(mExecutionStatus == Stopped)
			return;

		mExecutionPaused = !mExecutionPaused;

		mPauseInterrupt = !debug;

		if(mScriptEngine->isEvaluating())
		{
            mScriptEngine->setInterrupted(mExecutionPaused);
		}
		else
		{
			ActionTools::ActionInstance *currentAction = currentActionInstance();
			if(currentAction)
			{
				if(mExecutionPaused)
                    currentAction->doPauseExecution();
				else
                    currentAction->doResumeExecution();
			}
		}

		mExecutionWindow->setPauseStatus(mExecutionPaused);
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

        QJSValue script = mScriptEngine->globalObject().property(QStringLiteral("Script"));
        bool overrideProcedureChecks = script.property(QStringLiteral("overrideProcedureChecks")).toBool();
        if(!overrideProcedureChecks)
        {
            bool isInProcedure = false;
            for(auto [begin, end]: mProcedures)
            {
                if(mCurrentActionIndex >= begin && mCurrentActionIndex <= end)
                {
                    isInProcedure = true;
                    break;
                }
            }
            bool isTargetInProcedure = false;
            for(auto [begin, end]: mProcedures)
            {
                if(index >= begin && index <= end)
                {
                    isTargetInProcedure = true;
                    break;
                }
            }
            if(isInProcedure && !isTargetInProcedure)
            {
                return CannotJumpOutsideProcedure;
            }
            if(!isInProcedure && isTargetInProcedure)
            {
                return CannotJumpInsideProcedure;
            }
        }

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

        int nextLine = mCurrentActionIndex + 2;
		if(nextLine > mScript->actionCount())
			nextLine = -1;

		QJSValue script = mScriptEngine->globalObject().property(QStringLiteral("Script"));
        script.setProperty(QStringLiteral("nextLine"), nextLine);
        script.setProperty(QStringLiteral("line"), mCurrentActionIndex + 1);

		ActionTools::ActionInstance *actionInstance = currentActionInstance();

        const ActionTools::ExceptionActionInstancesHash &exceptionActionInstancesHash = actionInstance->exceptionActionInstances();
        const ActionTools::ActionException::ExceptionActionInstance &exceptionAction = exceptionActionInstancesHash.value(ActionTools::ActionException::CodeErrorException);
        mShowDebuggerOnCodeError = (exceptionAction.action() == ActionTools::ActionException::StopExecutionExceptionAction);

        auto line = ActionTools::NumberFormat::labelIndexString(mCurrentActionIndex);
        auto actionLineOrLabel = QStringLiteral("");
        if(actionInstance->label().isEmpty())
            actionLineOrLabel = line;
        else
            actionLineOrLabel = QStringLiteral("%1 (%2)").arg(actionInstance->label()).arg(line);

        mExecutionWindow->setCurrentActionName(actionInstance->definition()->name(), actionLineOrLabel);
		mExecutionWindow->setCurrentActionColor(actionInstance->color());

        connect(actionInstance, &ActionTools::ActionInstance::executionEndedSignal, this, &Executer::actionExecutionEnded);
		connect(actionInstance, &ActionTools::ActionInstance::executionException, this, &Executer::executionException);
		connect(actionInstance, &ActionTools::ActionInstance::disableAction, this, &Executer::disableAction);
		connect(actionInstance, &ActionTools::ActionInstance::showProgressDialog, this, &Executer::showProgressDialog);
        connect(actionInstance, static_cast<void (ActionTools::ActionInstance::*)(int)>(&ActionTools::ActionInstance::updateProgressDialog),
                this, static_cast<void (Executer::*)(int)>(&Executer::updateProgressDialog));
        connect(actionInstance, static_cast<void (ActionTools::ActionInstance::*)(const QString &)>(&ActionTools::ActionInstance::updateProgressDialog),
                this, static_cast<void (Executer::*)(const QString &)>(&Executer::updateProgressDialog));
		connect(actionInstance, &ActionTools::ActionInstance::hideProgressDialog, this, &Executer::hideProgressDialog);
        connect(actionInstance, &ActionTools::ActionInstance::consolePrint, this, static_cast<void (Executer::*)(const QString &)>(&Executer::consolePrint));
		connect(actionInstance, &ActionTools::ActionInstance::consolePrintWarning, this, &Executer::consolePrintWarning);
		connect(actionInstance, &ActionTools::ActionInstance::consolePrintError, this, &Executer::consolePrintError);

        actionInstance->setupStartExecutionTime();

		mExecutionStatus = PrePause;

        auto pauseBefore = currentActionInstance()->pauseBefore() + mScript->pauseBefore();
        if(pauseBefore > 0)
		{
            mExecutionTimer.start();
            mExecutionTime.start();
			mExecutionWindow->setProgressEnabled(true);
			mExecutionWindow->setProgressMinimum(0);
            mExecutionWindow->setProgressMaximum(pauseBefore);
			mExecutionWindow->setProgressValue(0);
		}
		else
        {
			mExecutionWindow->setProgressEnabled(false);

            QTimer::singleShot(0, this, &Executer::startActionExecution);
        }

        mExecutionEnded = false;
	}

    std::optional<int> Executer::lineToActionIndex(const QString &line) const
    {
        bool ok = false;
        auto targetLine = line.toInt(&ok);

        if(!ok)
        {
            targetLine = mScript->labelLine(line);
            if(targetLine == -1)
                return {};
        }

        if(targetLine < 0 || targetLine >= mScript->actionCount())
            return {};

        return {targetLine};
    }

    void Executer::detectJumpsInActionParameter(
        ActionTools::ActionInstance *actionInstance,
        int actionIndex,
        ActionTools::ElementDefinition *elementDefinition,
        QList<std::tuple<int, int>> &jumps) const
    {
        if(qobject_cast<ActionTools::LineParameterDefinition*>(elementDefinition))
        {
            auto parameter = actionInstance->parameter(elementDefinition->name().original());
            auto value = parameter.subParameter(QStringLiteral("value"));

            if(value.isCode())
                return;

            auto targetLine = lineToActionIndex(value.value());
            if(!targetLine)
                return;

            jumps.emplace_back(actionIndex, targetLine.value());
        }
        else if(qobject_cast<ActionTools::IfActionParameterDefinition*>(elementDefinition))
        {
            auto parameter = actionInstance->parameter(elementDefinition->name().original());
            auto action = parameter.subParameter(QStringLiteral("action"));
            auto line = parameter.subParameter(QStringLiteral("line"));

            if(action.isCode())
                return;
            if(line.isCode())
                return;
            if(action.value() != QLatin1String("goto"))
                return;

            auto targetLine = lineToActionIndex(line.value());
            if(!targetLine)
                return;

            jumps.emplace_back(actionIndex, targetLine.value());
        }
        else if(auto group = qobject_cast<ActionTools::GroupDefinition*>(elementDefinition))
        {
            for(auto member: group->members())
                detectJumpsInActionParameter(actionInstance, actionIndex, member, jumps);
        }
    }

    bool Executer::jumpsSanityChecks(const QList<std::tuple<int, int>> &procedures) const
    {
        QList<std::tuple<int, int>> jumps;

        // Detect all jumps we know of (excludes code-based jumps)
        for(int actionIndex = 0; actionIndex < mScript->actionCount(); ++actionIndex)
        {
            if(canExecuteAction(actionIndex) != CanExecute)
                continue;

            auto actionInstance = mScript->actionAt(actionIndex);
            if(!actionInstance)
                continue;

            // Detect jumps in action parameters
            for(const auto &element: actionInstance->definition()->elements())
                detectJumpsInActionParameter(actionInstance, actionIndex, element, jumps);

            // Detect jumps in action exceptions
            for(auto exception = 0; exception < ActionTools::ActionException::ExceptionCount; ++exception)
            {
                auto exceptionActionInstance = actionInstance->exceptionActionInstance(static_cast<ActionTools::ActionException::Exception>(exception));

                if(exceptionActionInstance.action() != ActionTools::ActionException::GotoLineExceptionAction)
                    continue;

                auto line = exceptionActionInstance.line();
                auto targetLine = lineToActionIndex(line);
                if(!targetLine)
                    continue;

                jumps.emplace_back(actionIndex, targetLine.value());
            }
            for(const auto &actionException: actionInstance->definition()->exceptions())
            {
                auto exceptionActionInstance = actionInstance->exceptionActionInstance(static_cast<ActionTools::ActionException::Exception>(actionException->id()));

                if(exceptionActionInstance.action() != ActionTools::ActionException::GotoLineExceptionAction)
                    continue;

                auto line = exceptionActionInstance.line();
                auto targetLine = lineToActionIndex(line);
                if(!targetLine)
                    continue;

                jumps.emplace_back(actionIndex, targetLine.value());
            }
        }

        // Check if we have a jump from within a procedure
        for(auto [begin, end]: procedures)
        {
            if(canExecuteAction(begin) != CanExecute || canExecuteAction(end) != CanExecute)
                continue;

            for(auto [jumpStart, jumpTarget]: jumps)
            {
                // Are we jumping from within the procedure
                if(jumpStart > begin && jumpStart < end)
                {
                    // Are we jumping to outside the procedure
                    if(jumpTarget <= begin || jumpTarget >= end)
                    {
                        auto actionInstance = mScript->actionAt(jumpStart);
                        if(!actionInstance)
                            continue;

                        mConsoleWidget->addActionLine(ActionTools::ConsoleWidget::Error, tr("An action cannot transition from inside a procedure to outside of it"),
                                                      actionInstance->runtimeId());

                        return false;
                    }
                }

                // Are we jumping from outside the procedure
                if(jumpStart <= begin || jumpStart >= end)
                {
                    // Are we jumping to within the procedure
                    if(jumpTarget > begin && jumpTarget < end)
                    {
                        auto actionInstance = mScript->actionAt(jumpStart);
                        if(!actionInstance)
                            continue;

                        mConsoleWidget->addActionLine(ActionTools::ConsoleWidget::Error, tr("An action cannot transition from outside a procedure to inside of it"),
                                                      actionInstance->runtimeId());

                        return false;
                    }
                }
            }
        }

        return true;
    }
}
