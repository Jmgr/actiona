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

#ifndef ACTIONCODEINSTANCE_H
#define ACTIONCODEINSTANCE_H

#include "actionexecution.h"
#include "action.h"

class ActionCodeInstance : public ActionTools::Action
{
	Q_OBJECT

public:
	SCRIPT_CONSTRUCTOR(ActionCode)

	ActionCodeInstance(ActionTools::ActionInterface *interface, QObject *parent = 0)
		: ActionTools::Action(interface, parent)										{}
	ActionCodeInstance(QObject *parent = 0)
		: ActionTools::Action(0, parent)												{}
	~ActionCodeInstance()																{}

	void startExecution(ActionTools::Script *script, QScriptEngine *scriptEngine)
	{
		ActionTools::ActionExecution actionExecution(this, script, scriptEngine);
		QString code;

		if(!actionExecution.evaluateString(code, "code"))
			return;

		emit executionEnded();
	}

private:
	Q_DISABLE_COPY(ActionCodeInstance)
};

#endif // ACTIONCODEINSTANCE_H
