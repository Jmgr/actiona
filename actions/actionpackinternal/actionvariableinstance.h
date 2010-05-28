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

#ifndef ACTIONVARIABLEINSTANCE_H
#define ACTIONVARIABLEINSTANCE_H

#include "actionexecution.h"
#include "action.h"
#include "script.h"

class ActionVariableInstance : public ActionTools::Action
{
	Q_OBJECT

public:
	ActionVariableInstance(ActionTools::ActionInterface *interface, QObject *parent = 0)
		: ActionTools::Action(interface, parent)										{}
	ActionVariableInstance(QObject *parent = 0)
		: ActionTools::Action(0, parent)												{}
	~ActionVariableInstance()															{}

	void startExecution()
	{
		ActionTools::ActionExecution actionExecution(this, script(), scriptEngine());
		QString variable;
		QString value;

		if(!actionExecution.evaluateString(variable, "variable") ||
		   !actionExecution.evaluateString(value, "value"))
			return;
		
		script()->setVariable(variable, value);

		emit executionEnded();
	}
	
public slots:
	void setVariable(const QString &name, const QVariant &value)
	{
		script()->setVariable(name, value);
	}

	QVariant variable(const QString &name)
	{
		return script()->variable(name);
	}

private:
	Q_DISABLE_COPY(ActionVariableInstance)
};

#endif // ACTIONVARIABLEINSTANCE_H
