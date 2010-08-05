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

#ifndef ACTIONMOVECURSORINSTANCE_H
#define ACTIONMOVECURSORINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"

#include <QCursor>

class ActionMoveCursorInstance : public ActionTools::ActionInstance
{
	Q_OBJECT

public:
	ActionMoveCursorInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
		: ActionTools::ActionInstance(definition, parent)										{}

	void startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
	
		QPoint position;
	
		if(!actionInstanceExecutionHelper.evaluatePoint(position, "position"))
			return;
		
		QCursor::setPos(position);
	
		emit executionEnded();
	}
	
public slots:
	void move(int x, int y)
	{
		QCursor::setPos(x, y);
	}

private:
	Q_DISABLE_COPY(ActionMoveCursorInstance)
};

#endif // ACTIONMOVECURSORINSTANCE_H
