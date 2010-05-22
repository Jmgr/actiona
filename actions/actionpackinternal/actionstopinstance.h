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

#ifndef ACTIONSTOPINSTANCE_H
#define ACTIONSTOPINSTANCE_H

#include "actionexecution.h"
#include "action.h"

class ActionStopInstance : public ActionTools::Action
{
	Q_OBJECT

public:
	SCRIPT_CONSTRUCTOR(ActionStop)

	ActionStopInstance(ActionTools::ActionInterface *interface, QObject *parent = 0)
		: ActionTools::Action(interface, parent)										{}
	ActionStopInstance(QObject *parent = 0)
		: ActionTools::Action(0, parent)												{}
	~ActionStopInstance()																{}

	void startExecution(ActionTools::Script *script, QScriptEngine *scriptEngine)
	{
		Q_UNUSED(script);
		
		scriptEngine->evaluate("Script.stopExecution()");

		emit executionEnded();
	}

private:
	Q_DISABLE_COPY(ActionStopInstance)
};

#endif // ACTIONSTOPINSTANCE_H
