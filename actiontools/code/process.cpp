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

#include "process.h"

namespace Code
{
	QScriptValue Process::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Process *process = 0;
		
		switch(context->argumentCount())
		{
		case 0:
			process = new Process;
			break;
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Process *codeProcess = qobject_cast<Process*>(object))
					process = new Process(*codeProcess);
				else
					process = new Process(context->argument(0).toInt32());
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
	
	QScriptValue Process::constructor(int processId, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Process(processId), QScriptEngine::ScriptOwnership);
	}
	
	int Process::parameter(QScriptContext *context)
	{
		switch(context->argumentCount())
		{
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Process *process = qobject_cast<Process*>(object))
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
	
	QScriptValue Process::list(QScriptContext *context, QScriptEngine *engine)
	{
		QList<int> processesList = ActionTools::CrossPlatform::runningProcesses();

		QScriptValue back = engine->newArray(processesList.count());

		for(int index = 0; index < processesList.count(); ++index)
			back.setProperty(index, constructor(processesList.at(index), context, engine));

		return back;
	}
	
	Process::Process()
		: QObject(),
		QScriptable()
	{
		
	}

	Process::Process(const Process &other)
		: QObject(),
		QScriptable(),
		mProcessId(other.processId())
	{
		
	}

	Process::Process(int processId)
		: QObject(),
		QScriptable(),
		mProcessId(processId)
	{
		
	}
	
	Process &Process::operator=(Process other)
	{
		swap(other);
		
		return *this;
	}

	Process &Process::operator=(int processId)
	{
		swap(processId);
		
		return *this;
	}
	
	void Process::swap(Process &other)
	{
		std::swap(mProcessId, other.mProcessId);
	}

	void Process::swap(int &processId)
	{
		std::swap(mProcessId, processId);
	}
	
	int Process::processId() const
	{
		return mProcessId;
	}
	
	QScriptValue Process::clone() const
	{
		return constructor(mProcessId, context(), engine());
	}

	bool Process::equals(const QScriptValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(Process *otherProcess = qobject_cast<Process*>(object))
			return (otherProcess == this || otherProcess->mProcessId == mProcessId);
			
		return false;
	}

	QString Process::toString() const
	{
		return QString("Process [id: %1]").arg(processId());
	}
	
	int Process::id() const
	{
		return processId();
	}
	
	bool Process::kill(ActionTools::CrossPlatform::KillMode killMode, int timeout) const
	{
		return ActionTools::CrossPlatform::killProcess(mProcessId, killMode, timeout);
	}
	
	bool Process::isRunning() const
	{
		return (ActionTools::CrossPlatform::processStatus(mProcessId) == ActionTools::CrossPlatform::Running);
	}
}
