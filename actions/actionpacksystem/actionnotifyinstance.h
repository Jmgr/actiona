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

#ifndef ACTIONNOTIFYINSTANCE_H
#define ACTIONNOTIFYINSTANCE_H

#include "actioninstance.h"
#include "script.h"

struct _NotifyNotification;

class ActionNotifyInstance : public ActionTools::ActionInstance
{
	Q_OBJECT

public:
	ActionNotifyInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0);
	~ActionNotifyInstance();

	void startExecution();

private:
	_NotifyNotification *mNotification;
	
	Q_DISABLE_COPY(ActionNotifyInstance)
};

#endif // ACTIONNOTIFYINSTANCE_H
