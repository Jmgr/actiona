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

#include "actionclickinstance.h"
#include "consolewidget.h"

#ifdef Q_WS_X11
#include "xdisplayhelper.h"
#endif

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <QPoint>
#include <QTimer>

ActionTools::StringListPair ActionClickInstance::buttons = qMakePair(
		QStringList() << "left" << "middle" << "right",
		QStringList() << tr("Left") << tr("Middle") << tr("Right"));
ActionTools::StringListPair ActionClickInstance::actions = qMakePair(
		QStringList() << "click" << "press" << "release",
		QStringList() << tr("Press and release") << tr("Press") << tr("Release"));

bool ActionClickInstance::mPressedButtonStatus[3] = {false};

void ActionClickInstance::startExecution()
{
	ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());

	Action action;
	Button button;
	QPoint position;
	int amount;

	if(!actionInstanceExecutionHelper.evaluateListElement(action, "action", "value", actions) ||
	   !actionInstanceExecutionHelper.evaluateListElement(button, "button", "value", buttons) ||
	   !actionInstanceExecutionHelper.evaluatePoint(position, "position") ||
	   !actionInstanceExecutionHelper.evaluateInteger(amount, "amount"))
		return;

	if(action != ClickAction)
		amount = 1;

	if(amount <= 0)
	{
		actionInstanceExecutionHelper.setCurrentParameter("amount");
		emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid click amount"));
		return;
	}

	if(mPressedButtonStatus[button] && action != ReleaseAction)
	{
		actionInstanceExecutionHelper.setCurrentParameter("action");
		emit executionException(InvalidActionException, tr("Cannot press the button because it's already pressed"));
		return;
	}

	if(!mPressedButtonStatus[button] && action == ReleaseAction)
	{
		actionInstanceExecutionHelper.setCurrentParameter("action");
		emit executionException(InvalidActionException, tr("Cannot release the button because it's not pressed"));
		return;
	}

#ifdef Q_WS_X11
	ActionTools::XDisplayHelper xDisplayHelper;
	if(!xDisplayHelper.display())
	{
		emit executionException(FailedToSendInputException, tr("Unable to emulate click: cannot open display"));
		return;
	}

	Window rootWindow = XRootWindow(xDisplayHelper.display(), XDefaultScreen(xDisplayHelper.display()));
	int previousX, previousY;
	Window unusedWindow;
	int unusedInt;
	unsigned int unusedUnsignedInt;

	if(action == ClickAction)
	{
		if(!XQueryPointer(xDisplayHelper.display(), rootWindow, &unusedWindow, &unusedWindow, &previousX, &previousY, &unusedInt, &unusedInt, &unusedUnsignedInt))
		{
			emit executionException(FailedToSendInputException, tr("Unable to emulate click: query pointer failed"));
			return;
		}
	}

	if(!XTestFakeMotionEvent(xDisplayHelper.display(), -1, position.x(), position.y(), CurrentTime))
	{
		emit executionException(FailedToSendInputException, tr("Unable to emulate click: fake motion event failed"));
		return;
	}

	XFlush(xDisplayHelper.display());

	int x11Button;
	switch(static_cast<Button>(button))
	{
	case MiddleButton:
		x11Button = Button2;
		break;
	case RightButton:
		x11Button = Button3;
		break;
	case LeftButton:
	default:
		x11Button = Button1;
		break;
	}

	for(int i = 0; i < amount; ++i)
	{
		if(action == ClickAction || action == PressAction)
		{
			if(!XTestFakeButtonEvent(xDisplayHelper.display(), x11Button, True, CurrentTime))
			{
				emit executionException(FailedToSendInputException, tr("Unable to emulate click: fake button event failed"));
				return;
			}
		}
		if(action == ClickAction || action == ReleaseAction)
		{
			if(!XTestFakeButtonEvent(xDisplayHelper.display(), x11Button, False, CurrentTime))
			{
				emit executionException(FailedToSendInputException, tr("Unable to emulate click: fake button event failed"));
				return;
			}
		}

		XFlush(xDisplayHelper.display());
	}

	if(action == ClickAction)
	{
		if(!XTestFakeMotionEvent(xDisplayHelper.display(), -1, previousX, previousY, CurrentTime))
		{
			emit executionException(FailedToSendInputException, tr("Unable to emulate click: fake motion event failed"));
			return;
		}

		XFlush(xDisplayHelper.display());
	}
#endif

#ifdef Q_WS_WIN
	POINT previousPosition;

	if(action == ClickAction)
	{
		if(!GetCursorPos(&previousPosition))
		{
			emit executionException(FailedToSendInputException, tr("Get cursor position failed"));
			return;
		}
	}

	if(!SetCursorPos(position.x(), position.y()))
	{
		emit executionException(FailedToSendInputException, tr("Set cursor position failed"));
		return;
	}

	int winButton;
	switch(static_cast<Button>(button))
	{
	case MiddleButton:
		winButton = MOUSEEVENTF_MIDDLEDOWN;
		break;
	case RightButton:
		winButton = MOUSEEVENTF_RIGHTDOWN;
		break;
	case LeftButton:
	default:
		winButton = MOUSEEVENTF_LEFTDOWN;
		break;
	}

	INPUT pressInput;
	pressInput.type = INPUT_MOUSE;
	MOUSEINPUT mousePressInput;
	mousePressInput.dx = 0;
	mousePressInput.dy = 0;
	mousePressInput.mouseData = 0;
	mousePressInput.dwFlags = winButton | MOUSEEVENTF_ABSOLUTE;
	mousePressInput.time = 0;
	pressInput.mi = mousePressInput;

	switch(static_cast<Button>(button))
	{
	case MiddleButton:
		winButton = MOUSEEVENTF_MIDDLEUP;
		break;
	case RightButton:
		winButton = MOUSEEVENTF_RIGHTUP;
		break;
	case LeftButton:
	default:
		winButton = MOUSEEVENTF_LEFTUP;
		break;
	}

	INPUT releaseInput;
	releaseInput.type = INPUT_MOUSE;
	MOUSEINPUT mouseReleaseInput;
	mouseReleaseInput.dx = 0;
	mouseReleaseInput.dy = 0;
	mouseReleaseInput.mouseData = 0;
	mouseReleaseInput.dwFlags = winButton | MOUSEEVENTF_ABSOLUTE;
	mouseReleaseInput.time = 0;
	releaseInput.mi = mouseReleaseInput;

	for(int i = 0; i < amount; ++i)
	{
		if(action == ClickAction || action == PressAction)
		{
			if(!SendInput(1, &pressInput, sizeof(INPUT)))
			{
				emit executionException(FailedToSendInputException, tr("Send input failed"));
				return;
			}
		}

		if(action == ClickAction || action == ReleaseAction)
		{
			if(!SendInput(1, &releaseInput, sizeof(INPUT)))
			{
				emit executionException(FailedToSendInputException, tr("Send input failed"));
				return;
			}
		}
	}

	if(action == ClickAction)
	{
		if(!SetCursorPos(previousPosition.x, previousPosition.y))
		{
			emit executionException(FailedToSendInputException, tr("Set cursor position failed"));
			return;
		}
	}
#endif
#ifdef Q_WS_MAC
	//TODO_MAC
#endif

	switch(action)
	{
	case PressAction:
		mPressedButtonStatus[button] = true;
		break;
	case ReleaseAction:
		mPressedButtonStatus[button] = false;
		break;
	default:
		break;
	}

	QTimer::singleShot(1, this, SIGNAL(executionEnded()));
}

void ActionClickInstance::stopLongTermExecution()
{
	//Release any pressed button, so that we don't end with an invalid state
	for(int button = 0; button < 3; ++button)
	{
		if(mPressedButtonStatus[button])
		{
#ifdef Q_WS_X11
			int x11Button;
			switch(static_cast<Button>(button))
			{
			case MiddleButton:
				x11Button = Button2;
				break;
			case RightButton:
				x11Button = Button3;
				break;
			case LeftButton:
			default:
				x11Button = Button1;
				break;
			}

			ActionTools::XDisplayHelper xDisplayHelper;
			if(!XTestFakeButtonEvent(xDisplayHelper.display(), x11Button, False, CurrentTime))
				continue;

			XFlush(xDisplayHelper.display());
#endif
#ifdef Q_WS_WIN
			int winButton;
			switch(static_cast<Button>(button))
			{
			case MiddleButton:
				winButton = MOUSEEVENTF_MIDDLEUP;
				break;
			case RightButton:
				winButton = MOUSEEVENTF_RIGHTUP;
				break;
			case LeftButton:
			default:
				winButton = MOUSEEVENTF_LEFTUP;
				break;
			}

			INPUT releaseInput;
			releaseInput.type = INPUT_MOUSE;
			MOUSEINPUT mouseReleaseInput;
			mouseReleaseInput.dx = 0;
			mouseReleaseInput.dy = 0;
			mouseReleaseInput.mouseData = 0;
			mouseReleaseInput.dwFlags = winButton | MOUSEEVENTF_ABSOLUTE;
			mouseReleaseInput.time = 0;
			releaseInput.mi = mouseReleaseInput;

			SendInput(1, &releaseInput, sizeof(INPUT));
#endif
		}
	}
}
