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

#ifndef ACTIONKILLPROCESSINSTANCE_H
#define ACTIONKILLPROCESSINSTANCE_H

#include "actionexecution.h"
#include "action.h"
#include "script.h"
#include "crossplatform.h"

class ActionKillProcessInstance : public ActionTools::Action
{
	Q_OBJECT
	Q_ENUMS(KillMode)

public:
	SCRIPT_CONSTRUCTOR(ActionKillProcess)
			
	enum KillMode
	{
		Graceful = ActionTools::CrossPlatform::Graceful,
		Forceful = ActionTools::CrossPlatform::Forceful,
		GracefulThenForceful = ActionTools::CrossPlatform::GracefulThenForceful
	};

	ActionKillProcessInstance(ActionTools::ActionInterface *interface, QObject *parent = 0)
		: ActionTools::Action(interface, parent)												{}
	ActionKillProcessInstance(QObject *parent = 0)
		: ActionTools::Action(0, parent)														{}
	~ActionKillProcessInstance()																{}
	
	static ActionTools::StringListPair killModes;

	void startExecution(ActionTools::Script *script, QScriptEngine *scriptEngine)
	{
		ActionTools::ActionExecution actionExecution(this, script, scriptEngine);
		int processId;
		
		if(!actionExecution.evaluateInteger(processId, "processId"))
			return;
		
		if(processId != 0)
			ActionTools::CrossPlatform::killProcess(processId, ActionTools::CrossPlatform::GracefulThenForceful, 100);
		
		emit executionEnded();
	}
	
public slots:
	bool killProcess(int processId, KillMode killMode = GracefulThenForceful, int timeout = 1000)
	{
		return ActionTools::CrossPlatform::killProcess(processId, static_cast<ActionTools::CrossPlatform::KillProcessMode>(killMode), timeout);
	}
	
private:
	Q_DISABLE_COPY(ActionKillProcessInstance)
};

#endif // ACTIONKILLPROCESSINSTANCE_H
