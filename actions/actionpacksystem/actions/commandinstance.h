/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef COMMANDINSTANCE_H
#define COMMANDINSTANCE_H

#include "actioninstance.h"
#include "script.h"

#include <QProcess>

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

namespace Actions
{
	class CommandInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum Exceptions
		{
			FailedToStartException = ActionTools::ActionException::UserException
		};

		CommandInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent), mProcess(new QProcess(this))
		{
			connect(mProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
			connect(mProcess, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
		}

		void startExecution()
		{
			bool ok = true;

			QString command = evaluateString(ok, "command");
			QString parameters = evaluateString(ok, "parameters");
			QString workingDirectory = evaluateString(ok, "workingDirectory");
			mExitCodeVariable = evaluateVariable(ok, "exitCode");
			QString processId = evaluateVariable(ok, "processId");
			mOutputVariable = evaluateVariable(ok, "output");
			mErrorOutputVariable = evaluateVariable(ok, "errorOutput");
			mExitStatusVariable = evaluateVariable(ok, "exitStatus");

			if(!ok)
				return;

			mProcess->setWorkingDirectory(workingDirectory);

			connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
			connect(mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

			QStringList parameterList = parameters.split(QChar(' '));
			mProcess->start(command, parameters.isEmpty() ? QStringList() : parameterList);
			setVariable(mOutputVariable, QString());
			setVariable(mErrorOutputVariable, QString());

	#ifdef Q_WS_WIN
			_PROCESS_INFORMATION *processInformation = mProcess->pid();
			if(processInformation)
                setVariable(processId, QString::number(processInformation->dwProcessId));
			else
                setVariable(processId, "0");
	#else
            setVariable(processId, QString::number(mProcess->pid()));
	#endif
		}

		void stopExecution()
		{
			terminate();
		}

	private slots:
		void readyReadStandardOutput()
		{
			QString output = QString::fromUtf8(mProcess->readAllStandardOutput());
			setVariable(mOutputVariable, output.trimmed());
		}

		void readyReadStandardError()
		{
			QString errorOutput = QString::fromUtf8(mProcess->readAllStandardError());
			setVariable(mErrorOutputVariable, errorOutput.trimmed());
		}

		void processError(QProcess::ProcessError error)
		{
			switch(error)
			{
			case QProcess::FailedToStart:
				terminate();

				emit executionException(FailedToStartException, tr("Failed to start the command. %1").arg(mProcess->errorString()));
				return;
			default:
				return;
			}
		}

		void processFinished(int exitCode, QProcess::ExitStatus exitStatus)
		{
            setVariable(mExitCodeVariable, QString::number(exitCode));

			switch(exitStatus)
			{
			case QProcess::NormalExit:
                setVariable(mExitStatusVariable, "normal");
				break;
			case QProcess::CrashExit:
                setVariable(mExitStatusVariable, "crash");
				break;
			}

			emit executionEnded();
		}

	private:
		void terminate()
		{
			mProcess->disconnect();
			mProcess->kill();
		}

		QProcess *mProcess;
		QString mExitCodeVariable;
		QString mOutputVariable;
		QString mErrorOutputVariable;
		QString mExitStatusVariable;

		Q_DISABLE_COPY(CommandInstance)
	};
}

#endif // COMMANDINSTANCE_H
