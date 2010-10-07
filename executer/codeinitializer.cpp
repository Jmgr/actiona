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

#include "codeinitializer.h"
#include "executer.h"
#include "code/image.h"
#include "code/rawdata.h"
#include "code/algorithms.h"
#include "code/color.h"
#include "code/point.h"
#include "code/size.h"
#include "code/rect.h"
#include "code/window.h"
#include "actionpack.h"
#include "actionfactory.h"
#include "codeexecution.h"
#include "codeconsole.h"

#include <QScriptEngine>

namespace LibExecuter
{
	void CodeInitializer::initialize(QScriptEngine *scriptEngine, ScriptAgent *scriptAgent, ActionTools::ActionFactory *actionFactory)
	{
		scriptAgent->setContext(ScriptAgent::ActionInit);
		
		scriptEngine->setProcessEventsInterval(50);

		QScriptValue codeExecution = scriptEngine->newQObject(new CodeExecution(scriptAgent), QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
		scriptEngine->globalObject().setProperty("execution", codeExecution);
		
		QScriptValue codeConsole = scriptEngine->newQObject(new CodeConsole(), QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeChildObjects | QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeDeleteLater);
		scriptEngine->globalObject().setProperty("console", codeConsole);
		
		addCodeClass<Code::RawData>("RawData", scriptEngine);
		addCodeClass<Code::Image>("Image", scriptEngine);
		addCodeClass<Code::Algorithms>("Algorithms", scriptEngine);
		addCodeClass<Code::Color>("Color", scriptEngine);
		addCodeClass<Code::Point>("Point", scriptEngine);
		addCodeClass<Code::Size>("Size", scriptEngine);
		addCodeClass<Code::Rect>("Rect", scriptEngine);
		QScriptValue windowValue = addCodeClass<Code::Window>("Window", scriptEngine);
		windowValue.setProperty("find", scriptEngine->newFunction(&Code::Window::find));//TODO : Think of a simpler method to add static stuff
		
		int actionPackCount = actionFactory->actionPackCount();
		for(int actionPackIndex = 0; actionPackIndex < actionPackCount; ++actionPackIndex)
		{
			ActionTools::ActionPack *actionPack = actionFactory->actionPack(actionPackIndex);

			actionPack->codeInit(scriptEngine);
		}

		scriptAgent->setContext(ScriptAgent::Parameters);
	}
}
