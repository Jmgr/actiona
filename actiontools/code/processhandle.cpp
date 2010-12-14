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

#include "processhandle.h"
#include "codetools.h"

#include <QProcess>

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>
#endif

namespace Code
{
	QScriptValue ProcessHandle::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		ProcessHandle *process = 0;
		
		switch(context->argumentCount())
		{
		case 0:
			process = new ProcessHandle;
			break;
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(ProcessHandle *codeProcess = qobject_cast<ProcessHandle*>(object))
					process = new ProcessHandle(*codeProcess);
				else
					process = new ProcessHandle(context->argument(0).toInt32());
			}
			break;
		default:
			throwError(context, engine, "ParameterCountError", tr("Incorrect parameter count"));
			break;
		}
		
		if(!process)
			return engine->undefinedValue();

		return CodeClass::constructor(process, context, engine);
	}
	
	QScriptValue ProcessHandle::constructor(int processId, QScriptContext *context, QScriptEngine *engine)
	{
		return CodeClass::constructor(new ProcessHandle(processId), context, engine);
	}
	
	int ProcessHandle::parameter(QScriptContext *context, QScriptEngine *engine)
	{
		switch(context->argumentCount())
		{
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(ProcessHandle *process = qobject_cast<ProcessHandle*>(object))
					return process->processId();
				else
					return context->argument(0).toInt32();
			}
			return -1;
		default:
			throwError(context, engine, "ParameterCountError", tr("Incorrect parameter count"));
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
		return constructor(mProcessId, context(), engine());
	}

	bool ProcessHandle::equals(const QScriptValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(ProcessHandle *otherProcess = qobject_cast<ProcessHandle*>(object))
			return (otherProcess == this || otherProcess->mProcessId == mProcessId);
			
		return false;
	}

	QString ProcessHandle::toString() const
	{
		return QString("ProcessHandle [id: %1]").arg(processId());
	}
	
	int ProcessHandle::id() const
	{
		return processId();
	}
	
	bool ProcessHandle::kill(ActionTools::CrossPlatform::KillMode killMode, int timeout) const
	{
		return ActionTools::CrossPlatform::killProcess(mProcessId, killMode, timeout);
	}
	
	bool ProcessHandle::isRunning() const
	{
		return (ActionTools::CrossPlatform::processStatus(mProcessId) == ActionTools::CrossPlatform::Running);
	}

	QString ProcessHandle::command() const
	{
#ifdef Q_WS_WIN
		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, id());
		if(!process)
		{
			throwError("OpenProcessError", tr("Unable to open the process"));
			return QString();
		}

		TCHAR buffer[256];
		if(!GetModuleFileNameEx(process, NULL, buffer, 256))
		{
			throwError("GetModuleFilenameError", tr("Unable to retrieve the executable filename"));
			return QString();
		}

		CloseHandle(process);

		return QString::fromWCharArray(buffer);
#else
		QProcess process;
		process.start(QString("ps h -p %1 -ocommand").arg(id()), QIODevice::ReadOnly);
		if(!process.waitForStarted(2000) || !process.waitForReadyRead(2000) || !process.waitForFinished(2000) || process.exitCode() != 0)
		{
			throwError("GetProcessError", tr("Failed to get the process command"));
			return QString();
		}

		return process.readAll();
#endif
	}

	ProcessHandle::Priority ProcessHandle::priority() const
	{
#ifdef Q_WS_WIN
		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id());
		if(!process)
		{
			throwError("OpenProcessError", tr("Unable to open the process"));
			return Normal;
		}

		int priority = GetPriorityClass(process);
		CloseHandle(process);

		switch(priority)
		{
		case ABOVE_NORMAL_PRIORITY_CLASS:
			return AboveNormal;
		case BELOW_NORMAL_PRIORITY_CLASS:
			return BelowNormal;
		case HIGH_PRIORITY_CLASS:
			return High;
		case IDLE_PRIORITY_CLASS:
			return Idle;
		case NORMAL_PRIORITY_CLASS:
			return Normal;
		case REALTIME_PRIORITY_CLASS:
			return Realtime;
		default:
			throwError("GetPriorityClassError", tr("Unable to retrieve the process priority"));
			return Normal;
		}
#else
		throwError("OperatingSystemError", tr("This is not available under your operating system"));
		return Normal;
#endif
	}
}
