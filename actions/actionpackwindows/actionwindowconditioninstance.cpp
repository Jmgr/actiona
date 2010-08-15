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

#include "actionwindowconditioninstance.h"
#include "actioninstanceexecutionhelper.h"
#include "ifactionvalue.h"
#include "windowhandle.h"

#include <QxtWindowSystem>
#include <QRegExp>

ActionTools::StringListPair ActionWindowConditionInstance::conditions = qMakePair(
		QStringList() << "created" << "closed",
		QStringList() << QObject::tr("Created") << QObject::tr("Closed"));

ActionWindowConditionInstance::ActionWindowConditionInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
	: ActionTools::ActionInstance(definition, parent)
{
}

void ActionWindowConditionInstance::startExecution()
{
	ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
	QString title;
	Condition condition;
	ActionTools::IfActionValue ifTrue;
	ActionTools::IfActionValue ifFalse;
	QString xCoordinate;
	QString yCoordinate;
	QString width;
	QString height;
	QString processId;
	
	if(!actionInstanceExecutionHelper.evaluateString(title, "title") ||
		!actionInstanceExecutionHelper.evaluateListElement(condition, conditions, "condition") ||
		!actionInstanceExecutionHelper.evaluateIfAction(ifTrue, "ifTrue") ||
		!actionInstanceExecutionHelper.evaluateIfAction(ifFalse, "ifFalse") ||
		!actionInstanceExecutionHelper.evaluateVariable(xCoordinate, "xCoordinate") ||
		!actionInstanceExecutionHelper.evaluateVariable(yCoordinate, "yCoordinate") ||
		!actionInstanceExecutionHelper.evaluateVariable(width, "width") ||
		!actionInstanceExecutionHelper.evaluateVariable(height, "height") ||
		!actionInstanceExecutionHelper.evaluateVariable(processId, "processId"))
		return;
	
	QRegExp titleRegExp(title, Qt::CaseSensitive, QRegExp::WildcardUnix);
	ActionTools::WindowHandle foundWindow;
	
	foreach(const WId &windowId, QxtWindowSystem::windows())
	{
		ActionTools::WindowHandle windowHandle(windowId);
		if(titleRegExp.exactMatch(windowHandle.title()))
		{
			foundWindow = windowHandle;
			break;
		}
	}
	
	if(foundWindow.isValid())
	{
		QRect windowRect = foundWindow.rect();
		
		actionInstanceExecutionHelper.setVariable(xCoordinate, windowRect.x());
		actionInstanceExecutionHelper.setVariable(yCoordinate, windowRect.y());
		actionInstanceExecutionHelper.setVariable(width, windowRect.width());
		actionInstanceExecutionHelper.setVariable(height, windowRect.height());
		actionInstanceExecutionHelper.setVariable(processId, foundWindow.processId());
	}
	
	ActionTools::IfActionValue action;
	switch(condition)
	{
	case Created:
		if(foundWindow.isValid())
			action = ifTrue;
		else
			action = ifFalse;
		break;
	case Closed:
		if(foundWindow.isValid())
			action = ifFalse;
		else
			action = ifTrue;
		break;
	}
	
	if(action.action() == ActionTools::IfActionValue::GOTO)
		actionInstanceExecutionHelper.setNextLine(action.line());

	emit executionEnded();
}
