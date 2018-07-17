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

	Contact : jmgr@jmgr.info
*/

#include "codeexecution.h"
#include "scriptagent.h"
#include "crossplatform.h"

#include <QScriptEngine>

namespace LibExecuter
{
	ScriptAgent *CodeExecution::mScriptAgent = nullptr;

	QScriptValue CodeExecution::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

		return context->thisObject();
    }

	QScriptValue CodeExecution::pause(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

		ActionTools::CrossPlatform::sleep(context->argument(0).toInt32());

		return context->thisObject();
	}
	
	QScriptValue CodeExecution::sleep(QScriptContext *context, QScriptEngine *engine)
	{
		pause(context, engine);

		return context->thisObject();
	}
	
	QScriptValue CodeExecution::stop(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

		if(mScriptAgent)
			mScriptAgent->stopExecution();

		return context->thisObject();
	}
}
