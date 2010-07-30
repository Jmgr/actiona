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

#include "actionkeyinstance.h"
#include "consolewidget.h"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif
#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

void ActionKeyInstance::startExecution()
{
	ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());

	int button;
	int amount;
	QString position;

	if(!actionInstanceExecutionHelper.evaluateInteger(button, "button") ||
	   !actionInstanceExecutionHelper.evaluateInteger(amount, "amount") ||
	   !actionInstanceExecutionHelper.evaluateString(position, "position"))
		return;

	emit executionEnded();
}

void ActionKeyInstance::stopExecution()
{
	
}

void ActionKeyInstance::stopLongTermExecution()
{
	
}
