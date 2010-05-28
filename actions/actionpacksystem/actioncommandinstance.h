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

#ifndef ACTIONCOMMANDINSTANCE_H
#define ACTIONCOMMANDINSTANCE_H

#include "actionexecution.h"
#include "action.h"
#include "script.h"

#include <QProcess>

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

class ActionCommandInstance : public ActionTools::Action
{
	Q_OBJECT

public:
	ActionCommandInstance(ActionTools::ActionInterface *interface, QObject *parent = 0)
		: ActionTools::Action(interface, parent), mProcess(new QProcess(this))
	{
		connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
	}
	ActionCommandInstance(QObject *parent = 0)
		: ActionTools::Action(0, parent), mProcess(new QProcess(this))
	{
		connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
	}
	~ActionCommandInstance()																	{}

	void startExecution()
	{
		ActionTools::ActionExecution actionExecution(this, script(), scriptEngine());
		QString command;
		QString parameters;
		QString workingDirectory;
		QString processId;

		if(!actionExecution.evaluateString(command, "command") ||
			!actionExecution.evaluateString(parameters, "parameters") ||
			!actionExecution.evaluateString(workingDirectory, "workingDirectory") ||
			!actionExecution.evaluateVariable(mExitCodeVariable, "exitCode") ||
			!actionExecution.evaluateVariable(processId, "processId") ||
			!actionExecution.evaluateVariable(mOutputVariable, "output") ||
			!actionExecution.evaluateVariable(mErrorOutputVariable, "errorOutput") ||
			!actionExecution.evaluateVariable(mExitStatusVariable, "exitStatus"))
			return;
		
		mProcess->setWorkingDirectory(workingDirectory);
		
		QStringList parameterList = parameters.split(QChar(' '));
		mProcess->start(command, parameters.isEmpty() ? QStringList() : parameterList);
		
#ifdef Q_WS_WIN
		_PROCESS_INFORMATION *processInformation = mProcess->pid();
		if(processInformation)
			script()->setVariable(processId, QString::number(processInformation->dwProcessId));
		else
			script()->setVariable(processId, "0");
#else
		script()->setVariable(processId, QString::number(mProcess->pid()));
#endif
	}
	
	void stopExecution()
	{
		mProcess->kill();
	}
	
public slots:
	qint64 executeDetached(const QString &command, const QString &parameters = "", const QString &workingDirectory = "") const
	{
		qint64 processId;
		QStringList parameterList = parameters.split(QChar(' '));
		
		bool started = QProcess::startDetached(command, parameters.isEmpty() ? QStringList() : parameterList, workingDirectory, &processId);
		
		if(started)
			return processId;
		else
			return 0;
	}
	QString execute(const QString &command, const QString &parameters = "", const QString &workingDirectory = "", int maxExecutionTime = -1) const
	{
		QProcess process;
		QStringList parameterList = parameters.split(QChar(' '));
		
		process.setWorkingDirectory(workingDirectory);
		process.start(command, parameters.isEmpty() ? QStringList() : parameterList);
		process.waitForFinished(maxExecutionTime);
		
		if(process.state() != QProcess::NotRunning)
		{
			process.kill();
			process.waitForFinished(3000);
		}
		
		const QString &standardOutput = QString::fromUtf8(process.readAllStandardOutput());
		const QString &errorOutput = QString::fromUtf8(process.readAllStandardError());
	
		if(standardOutput.isEmpty())
			return errorOutput;
		
		return standardOutput;
	}
	
private slots:
	void processFinished(int exitCode, QProcess::ExitStatus exitStatus)
	{
		script()->setVariable(mExitCodeVariable, QString::number(exitCode));
		
		QString output = QString::fromUtf8(mProcess->readAllStandardOutput());
		script()->setVariable(mOutputVariable, output.trimmed());
		
		QString errorOutput = QString::fromUtf8(mProcess->readAllStandardError());
		script()->setVariable(mErrorOutputVariable, errorOutput.trimmed());
		
		switch(exitStatus)
		{
		case QProcess::NormalExit:
			script()->setVariable(mExitStatusVariable, "normal");
			break;
		case QProcess::CrashExit:
			script()->setVariable(mExitStatusVariable, "crash");
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
	
	Q_DISABLE_COPY(ActionCommandInstance)
};

#endif // ACTIONCOMMANDINSTANCE_H
