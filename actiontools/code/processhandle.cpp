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
			context->throwError("Incorrect parameter count");
			break;
		}
		
		if(!process)
			return engine->undefinedValue();

		return engine->newQObject(process, QScriptEngine::ScriptOwnership);
	}
	
	QScriptValue ProcessHandle::constructor(int processId, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new ProcessHandle(processId), QScriptEngine::ScriptOwnership);
	}
	
	int ProcessHandle::parameter(QScriptContext *context)
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
			context->throwError("Incorrect parameter count");
			return -1;
		}
	}
	
	QScriptValue ProcessHandle::list(QScriptContext *context, QScriptEngine *engine)
	{
		QList<int> processesList = ActionTools::CrossPlatform::runningProcesses();

		QScriptValue back = engine->newArray(processesList.count());

		for(int index = 0; index < processesList.count(); ++index)
			back.setProperty(index, constructor(processesList.at(index), context, engine));

		return back;
	}
	
	ProcessHandle::ProcessHandle()
		: QObject(),
		QScriptable()
	{
		
	}

	ProcessHandle::ProcessHandle(const ProcessHandle &other)
		: QObject(),
		QScriptable(),
		mProcessId(other.processId())
	{
		
	}

	ProcessHandle::ProcessHandle(int processId)
		: QObject(),
		QScriptable(),
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
		return QString("Process [id: %1]").arg(processId());
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
}
