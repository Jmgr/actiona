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

#include "wheelinstance.h"
#include "actioninstanceexecutionhelper.h"

#include <QDebug>

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif
#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace Actions
{
	void WheelInstance::startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
	
		int intensity;
	
		if(!actionInstanceExecutionHelper.evaluateInteger(intensity, "intensity"))
			return;
	
	#ifdef Q_WS_X11
		Display *display = XOpenDisplay(0);
		if(!display)
		{
			emit executionException(FailedToSendInputException, tr("Unable to emulate wheel: cannot open display"));
			return;
		}
	
		int button;
		if(intensity < 0)
		{
			button = Button5;
			intensity = -intensity;
		}
		else
			button = Button4;
	
		bool result = true;
	
		for(int i = 0; i < intensity; ++i)
		{
			result &= XTestFakeButtonEvent(display, button, True, CurrentTime);
			result &= XTestFakeButtonEvent(display, button, False, CurrentTime);
		}
	
		XCloseDisplay(display);
	
		if(!result)
		{
			emit executionException(FailedToSendInputException, tr("Unable to emulate wheel: failed to send input"));
			return;
		}
	#endif
	#ifdef Q_WS_WIN
		INPUT input;
	
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		input.mi.time = 0;
		input.mi.dwExtraInfo = 0;
		input.mi.mouseData = intensity * WHEEL_DELTA;
	
		if(!SendInput(1, &input, sizeof(INPUT)))
		{
			emit executionException(FailedToSendInputException, tr("Unable to emulate wheel: failed to send input"));
			return;
		}
	#endif
	
		emit executionEnded();
	}
}

