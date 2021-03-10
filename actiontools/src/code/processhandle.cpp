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

#include "actiontools/code/processhandle.hpp"
#include "actiontools/code/codetools.hpp"
#include "backend/process.hpp"

#include <QProcess>

namespace Code
{
	QScriptValue ProcessHandle::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		ProcessHandle *process = nullptr;
		
		switch(context->argumentCount())
		{
		case 0:
			process = new ProcessHandle;
			break;
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(auto codeProcess = qobject_cast<ProcessHandle*>(object))
					process = new ProcessHandle(*codeProcess);
				else
					process = new ProcessHandle(context->argument(0).toInt32());
			}
			break;
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			break;
		}
		
		if(!process)
			return engine->undefinedValue();

		return CodeClass::constructor(process, context, engine);
	}
	
	QScriptValue ProcessHandle::constructor(int processId, QScriptEngine *engine)
	{
		return CodeClass::constructor(new ProcessHandle(processId), engine);
	}
	
	int ProcessHandle::parameter(QScriptContext *context, QScriptEngine *engine)
	{
		switch(context->argumentCount())
		{
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(auto process = qobject_cast<ProcessHandle*>(object))
					return process->processId();
				else
					return context->argument(0).toInt32();
			}
			return -1;
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			return -1;
		}
	}

	void ProcessHandle::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassToScriptEngine<ProcessHandle>(scriptEngine);
	}
	
	ProcessHandle::ProcessHandle()
		: CodeClass()
	{
		
	}

	ProcessHandle::ProcessHandle(const ProcessHandle &other)
		: CodeClass(),
		mProcessId(other.processId())
	{
		
	}

	ProcessHandle::ProcessHandle(int processId)
		: CodeClass(),
		mProcessId(processId)
	{
		
	}
	
	ProcessHandle &ProcessHandle::operator=(ProcessHandle other)
	{
		swap(other);
		
		return *this;
	}

	ProcessHandle &ProcessHandle::operator=(int processId)
	{
		swap(processId);
		
		return *this;
	}
	
	void ProcessHandle::swap(ProcessHandle &other)
	{
		std::swap(mProcessId, other.mProcessId);
	}

	void ProcessHandle::swap(int &processId)
	{
		std::swap(mProcessId, processId);
	}
	
	int ProcessHandle::processId() const
	{
		return mProcessId;
	}
	
	QScriptValue ProcessHandle::clone() const
	{
		return constructor(mProcessId, engine());
	}

	bool ProcessHandle::equals(const QScriptValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(auto otherProcess = qobject_cast<ProcessHandle*>(object))
			return (otherProcess == this || otherProcess->mProcessId == mProcessId);
			
		return false;
	}

	QString ProcessHandle::toString() const
	{
		return QStringLiteral("ProcessHandle {id: %1}").arg(processId());
	}
	
	int ProcessHandle::id() const
	{
        return processId();
    }

    int ProcessHandle::parentId() const
    {
        try
        {
            return Backend::Instance::process().parentProcess(mProcessId);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("GetProcessParentError"), tr("Failed to get the process parent id: %1").arg(e.what()));
            return 0;
        }

#ifndef Q_OS_WIN // TODO
        QProcess process;
        process.start(QStringLiteral("ps"), {QStringLiteral("h") , QStringLiteral("-p %1").arg(id()), QStringLiteral("-oppid")}, QIODevice::ReadOnly);
        if(!process.waitForStarted(2000) || !process.waitForReadyRead(2000) || !process.waitForFinished(2000) || process.exitCode() != 0)
        {
			throwError(QStringLiteral("GetProcessError"), tr("Failed to get the process parent id"));
            return 0;
        }

        bool ok = true;
        int result = process.readAll().trimmed().toInt(&ok);

        if(!ok)
        {
			throwError(QStringLiteral("GetProcessError"), tr("Failed to get the process parent id"));
            return 0;
        }

        return result;
#endif
    }
	
	bool ProcessHandle::kill(KillMode killMode, int timeout) const
	{
        try
        {
            Backend::Instance::process().killProcess(mProcessId, static_cast<Backend::Process::KillMode>(killMode), timeout);
        }
        catch(const Backend::BackendError &)
        {
            return false;
        }

        return true;
	}
	
	bool ProcessHandle::isRunning() const
	{
        try
        {
            return (Backend::Instance::process().processStatus(mProcessId) == Backend::Process::ProcessStatus::Running);
        }
        catch(const Backend::BackendError &)
        {
            return false;
        }
	}

	QString ProcessHandle::command() const
	{
        try
        {
            return Backend::Instance::process().processCommand(mProcessId);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("GetProcessCommandError"), tr("Failed to get the process command: %1").arg(e.what()));
            return {};
        }
#ifndef Q_OS_WIN // TODO
		QProcess process;
        process.start(QStringLiteral("ps"), {QStringLiteral("h"), QStringLiteral("-p %1").arg(id()), QStringLiteral("-ocommand")}, QIODevice::ReadOnly);
		if(!process.waitForStarted(2000) || !process.waitForReadyRead(2000) || !process.waitForFinished(2000) || process.exitCode() != 0)
		{
			throwError(QStringLiteral("GetProcessError"), tr("Failed to get the process command"));
			return QString();
		}

		return QLatin1String(process.readAll().trimmed());
#endif
	}

	ProcessHandle::Priority ProcessHandle::priority() const
	{
        try
        {
            return static_cast<ProcessHandle::Priority>(Backend::Instance::process().processPriority(mProcessId));
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("GetProcessPriorityError"), tr("Failed to get the process priority: %1").arg(e.what()));
            return {};
        }

#ifndef Q_OS_WIN // TODO
		throwError(QStringLiteral("OperatingSystemError"), tr("This is not available under your operating system"));
		return Normal;
#endif
	}
}
