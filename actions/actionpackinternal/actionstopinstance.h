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

#include "actioninstance.h"

class ActionStopInstance : public ActionTools::ActionInstance
{
	Q_OBJECT

public:
	ActionStopInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
		: ActionTools::ActionInstance(definition, parent)										{}

	void startExecution()
	{
		scriptEngine()->evaluate("Script.stopExecution()");

		emit executionEnded();
	}

private:
	Q_DISABLE_COPY(ActionStopInstance)
};

#endif // ACTIONSTOPINSTANCE_H
