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

#ifndef ACTIONPAUSEINSTANCE_H
#define ACTIONPAUSEINSTANCE_H

#include "actionexecution.h"
#include "actioninstance.h"

#include <QTimer>

class ActionPauseInstance : public ActionTools::ActionInstance
{
	Q_OBJECT

public:
	ActionPauseInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
		: ActionTools::ActionInstance(definition, parent)										{}

	void startExecution()
	{
		ActionTools::ActionExecution actionExecution(this, script(), scriptEngine());
		int duration;

		if(!actionExecution.evaluateInteger(duration, "duration"))
			return;

		QTimer::singleShot(duration, this, SIGNAL(executionEnded()));
	}

private:
	Q_DISABLE_COPY(ActionPauseInstance)
};

#endif // ACTIONPAUSEINSTANCE_H
