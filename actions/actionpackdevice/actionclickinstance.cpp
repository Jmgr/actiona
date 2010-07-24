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
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
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

void ActionClickInstance::startExecution()
{
	ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());

	int button;
	int amount;
	QString position;

	if(!actionInstanceExecutionHelper.evaluateListElement(button, "button", "value", buttons) ||
	   !actionInstanceExecutionHelper.evaluateInteger(amount, "amount") ||
	   !actionInstanceExecutionHelper.evaluateString(position, "position"))
		return;

	if(amount <= 0)
	{
		actionInstanceExecutionHelper.setCurrentParameter("amount");
		emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid click amount"));
		return;
	}

	QStringList stringCoordinates = position.split(QChar(':'));

	if(stringCoordinates.count() != 2)
	{
		actionInstanceExecutionHelper.setCurrentParameter("position");
		emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid click coordinates"));
		return;
	}

	QPoint coordinates;
	bool okX, okY;

	coordinates.setX(stringCoordinates.at(0).toInt(&okX));
	coordinates.setY(stringCoordinates.at(1).toInt(&okY));

	if(!okX || !okY)
	{
		actionInstanceExecutionHelper.setCurrentParameter("position");
		emit executionException(ActionTools::ActionException::BadParameterException, tr("Invalid click coordinates"));
		return;
	}

#ifdef Q_WS_X11
	Display *display = XOpenDisplay(0);
	if(!display)
	{
		emit executionException(FailedToSendInputException, tr("Unable to emulate click: cannot open display"));
		return;
	}

	Window rootWindow = DefaultRootWindow(display);
	int previousX, previousY;
	Window unusedWindow;
	int unusedInt;
	unsigned int unusedUnsignedInt;

	XQueryPointer(display, rootWindow, &unusedWindow, &unusedWindow, &previousX, &previousY, &unusedInt, &unusedInt, &unusedUnsignedInt);
	XTestFakeMotionEvent(display, -1, coordinates.x(), coordinates.y(), 0);

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

	//TODO : Look why this doesn't work...

	for(int i = 0; i < amount; ++i)
	{
		XTestFakeButtonEvent(display, x11Button, True, i+1);
		XTestFakeButtonEvent(display, x11Button, False, i+2);
	}

	XTestFakeMotionEvent(display, -1, previousX, previousY, amount*2+1);
	XCloseDisplay(display);
#endif
#ifdef Q_WS_WIN
	POINT previousPosition;

	GetCursorPos(&previousPosition);
	SetCursorPos(coordinates.x(), coordinates.y());

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
	mousePressInput.dx = coordinates.x();
	mousePressInput.dy = coordinates.y();
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
	mouseReleaseInput.dx = coordinates.x();
	mouseReleaseInput.dy = coordinates.y();
	mouseReleaseInput.mouseData = 0;
	mouseReleaseInput.dwFlags = winButton | MOUSEEVENTF_ABSOLUTE;
	mouseReleaseInput.time = 0;
	releaseInput.mi = mousePressInput;

	for(int i = 0; i < amount; ++i)
	{
		SendInput(1, &pressInput, sizeof(INPUT));
		SendInput(1, &releaseInput, sizeof(INPUT));
	}

	SetCursorPos(previousPosition.x, previousPosition.y);
	//TODO : Send FailedToSendInputException on fail
#endif
#ifdef Q_WS_MAC
	//TODO_MAC
#endif

	QTimer::singleShot(1, this, SIGNAL(executionEnded()));
}
