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

#ifndef ACTIONLOOPINSTANCE_H
#define ACTIONLOOPINSTANCE_H

#include "actionexecution.h"
#include "action.h"

class ActionLoopInstance : public ActionTools::Action
{
	Q_OBJECT

public:
	SCRIPT_CONSTRUCTOR(ActionLoop)

	ActionLoopInstance(ActionTools::ActionInterface *interface, QObject *parent = 0)
		: ActionTools::Action(interface, parent), mInitialized(false), mCounter(0)		{}
	ActionLoopInstance(QObject *parent = 0)
		: ActionTools::Action(0, parent), mInitialized(false), mCounter(0)				{}
	~ActionLoopInstance()																{}

	void startExecution(ActionTools::Script *script, QScriptEngine *scriptEngine)
	{
		if(mInitialized && mCounter == 0)
		{
			emit executionEnded();
			return;
		}
		
		ActionTools::ActionExecution actionExecution(this, script, scriptEngine);
		QString line;
		int count;

		if(!actionExecution.evaluateString(line, "line"))
			return;
		
		if(!mInitialized)
		{
			mInitialized = true;
			
			if(!actionExecution.evaluateInteger(count, "count"))
				return;
			
			if(count <= 0)
			{
				emit executionEnded();
				return;
			}
			
			mCounter = count;
		}

		actionExecution.setNextLine(line);

		--mCounter;

		emit executionEnded();
	}
	
	void reset()
	{
		mInitialized = false;
		mCounter = 0;
	}

private:
	bool mInitialized;
	int mCounter;
	
	Q_DISABLE_COPY(ActionLoopInstance)
};

#endif // ACTIONLOOPINSTANCE_H
