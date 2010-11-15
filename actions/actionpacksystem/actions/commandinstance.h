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

#ifndef COMMANDINSTANCE_H
#define COMMANDINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"
#include "script.h"

#include <QProcess>

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace Actions
{
	class CommandInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		CommandInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent), mProcess(new QProcess(this))
		{
			connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
		}

		void startExecution()
		{
			ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
			QString command;
			QString parameters;
			QString workingDirectory;
			QString processId;

			if(!actionInstanceExecutionHelper.evaluateString(command, "command") ||
				!actionInstanceExecutionHelper.evaluateString(parameters, "parameters") ||
				!actionInstanceExecutionHelper.evaluateString(workingDirectory, "workingDirectory") ||
				!actionInstanceExecutionHelper.evaluateVariable(mExitCodeVariable, "exitCode") ||
				!actionInstanceExecutionHelper.evaluateVariable(processId, "processId") ||
				!actionInstanceExecutionHelper.evaluateVariable(mOutputVariable, "output") ||
				!actionInstanceExecutionHelper.evaluateVariable(mErrorOutputVariable, "errorOutput") ||
				!actionInstanceExecutionHelper.evaluateVariable(mExitStatusVariable, "exitStatus"))
				return;

			mProcess->setWorkingDirectory(workingDirectory);

			QStringList parameterList = parameters.split(QChar(' '));
			mProcess->start(command, parameters.isEmpty() ? QStringList() : parameterList);

	#ifdef Q_WS_WIN
			_PROCESS_INFORMATION *processInformation = mProcess->pid();
			if(processInformation)
				actionInstanceExecutionHelper.setVariable(processId, QString::number(processInformation->dwProcessId));
			else
				actionInstanceExecutionHelper.setVariable(processId, "0");
	#else
			actionInstanceExecutionHelper.setVariable(processId, QString::number(mProcess->pid()));
	#endif
		}

		void stopExecution()
		{
			mProcess->kill();
		}

	private slots:
		void processFinished(int exitCode, QProcess::ExitStatus exitStatus)
		{
			ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
			actionInstanceExecutionHelper.setVariable(mExitCodeVariable, QString::number(exitCode));

			QString output = QString::fromUtf8(mProcess->readAllStandardOutput());
			actionInstanceExecutionHelper.setVariable(mOutputVariable, output.trimmed());

			QString errorOutput = QString::fromUtf8(mProcess->readAllStandardError());
			actionInstanceExecutionHelper.setVariable(mErrorOutputVariable, errorOutput.trimmed());

			switch(exitStatus)
			{
			case QProcess::NormalExit:
				actionInstanceExecutionHelper.setVariable(mExitStatusVariable, "normal");
				break;
			case QProcess::CrashExit:
				actionInstanceExecutionHelper.setVariable(mExitStatusVariable, "crash");
				break;
			}

			emit executionEnded();
		}

	private:
		QProcess *mProcess;
		QString mExitCodeVariable;
		QString mOutputVariable;
		QString mErrorOutputVariable;
		QString mExitStatusVariable;

		Q_DISABLE_COPY(CommandInstance)
	};
}

#endif // COMMANDINSTANCE_H
