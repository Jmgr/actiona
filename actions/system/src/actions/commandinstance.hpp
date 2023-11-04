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

#include "actiontools/actioninstance.hpp"
#include "actiontools/script.hpp"

#include <QProcess>

#ifdef Q_OS_WIN
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

		CommandInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent), mProcess(new QProcess(this))
		{
            connect(mProcess, &QProcess::readyReadStandardOutput, this, &CommandInstance::readyReadStandardOutput);
            connect(mProcess, &QProcess::readyReadStandardError, this, &CommandInstance::readyReadStandardError);
		}

		void startExecution() override
		{
			bool ok = true;

			QString command = evaluateString(ok, QStringLiteral("command"));
			QString parameters = evaluateString(ok, QStringLiteral("parameters"));
			QString workingDirectory = evaluateString(ok, QStringLiteral("workingDirectory"));
			mExitCodeVariable = evaluateVariable(ok, QStringLiteral("exitCode"));
			QString processId = evaluateVariable(ok, QStringLiteral("processId"));
			mOutputVariable = evaluateVariable(ok, QStringLiteral("output"));
			mErrorOutputVariable = evaluateVariable(ok, QStringLiteral("errorOutput"));
			mExitStatusVariable = evaluateVariable(ok, QStringLiteral("exitStatus"));

			if(!ok)
				return;

			mProcess->setWorkingDirectory(workingDirectory);

            connect(mProcess, static_cast<void (QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished), this, &CommandInstance::processFinished);
	    connect(mProcess, static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred), this, &CommandInstance::processError);

			QStringList parameterList = parameters.split(QLatin1Char(' '));
            mProcess->start(command, parameters.isEmpty() ? QStringList{} : parameterList);
			setVariable(mOutputVariable, QString());
			setVariable(mErrorOutputVariable, QString());
			setVariable(processId, QString::number(mProcess->processId()));
		}

		void stopExecution() override
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
				setVariable(mExitStatusVariable, QStringLiteral("normal"));
				break;
			case QProcess::CrashExit:
				setVariable(mExitStatusVariable, QStringLiteral("crash"));
				break;
			}

			executionEnded();
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

