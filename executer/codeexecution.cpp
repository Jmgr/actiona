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

#include "codeexecution.h"
#include "scriptagent.h"

namespace LibExecuter
{
	ScriptAgent *CodeExecution::mScriptAgent = 0;

	QScriptValue CodeExecution::constructor(QScriptContext *context, QScriptEngine *)
	{
		return context->thisObject();
	}

	QScriptValue CodeExecution::pause(QScriptContext *context, QScriptEngine *)
	{
		if(mScriptAgent)
			mScriptAgent->setPauseDuration(static_cast<quint64>(context->argument(0).toNumber()));

		return QScriptValue();
	}
	
	QScriptValue CodeExecution::sleep(QScriptContext *context, QScriptEngine *engine)
	{
		pause(context, engine);

		return QScriptValue();
	}
	
	QScriptValue CodeExecution::stop(QScriptContext *, QScriptEngine *)
	{
		if(mScriptAgent)
			mScriptAgent->emitStopExecution();

		return QScriptValue();
	}
}
