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

#include "actiontextinstance.h"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif
#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <QTimer>

void ActionTextInstance::startExecution()
{
	ActionTools::ActionExecution actionExecution(this, script(), scriptEngine());

	QString text;

	if(!actionExecution.evaluateString(text, "text"))
		return;
	
	//TODO

#ifdef Q_WS_X11
	
#endif
#ifdef Q_WS_WIN

#endif
#ifdef Q_WS_MAC
	//TODO_MAC
#endif

	QTimer::singleShot(1, this, SIGNAL(executionEnded()));
}
