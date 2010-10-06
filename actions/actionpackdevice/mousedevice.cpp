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

#include "mousedevice.h"

#include <QCursor>

#ifdef Q_WS_X11
#include "xdisplayhelper.h"
#endif

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

MouseDevice::MouseDevice()
{
	for(int i = 0; i < ButtonCount; ++i)
		mPressedButtons[i] = false;
}

MouseDevice::~MouseDevice()
{
	for(int i = 0; i < ButtonCount; ++i)
	{
		if(mPressedButtons[i])
			releaseButton(static_cast<Button>(i));
	}
}

bool MouseDevice::isButtonPressed(Button button) const
{
#ifdef Q_WS_WIN
	switch(button)
	{
	case LeftButton:
		return (GetAsyncKeyState(GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON) & 0x8000);
	case MiddleButton:
		return (GetAsyncKeyState(VK_MBUTTON) & 0x8000);
	case RightButton:
		return (GetAsyncKeyState(GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON) & 0x8000);
	default:
		return false;
	}
#endif
#ifdef Q_WS_X11
	ActionTools::XDisplayHelper xDisplayHelper;
	if(!xDisplayHelper.display())
		return false;

	Window unusedWindow;
	int unusedInt;
	unsigned int buttonMask;
	if(!XQueryPointer(xDisplayHelper.display(),
					  XDefaultRootWindow(xDisplayHelper.display()),
					  &unusedWindow,
					  &unusedWindow,
					  &unusedInt,
					  &unusedInt,
					  &unusedInt,
					  &unusedInt,
					  &buttonMask))
		return false;

	switch(button)
	{
	case LeftButton:
		return (buttonMask & Button1Mask);
	case MiddleButton:
		return (buttonMask & Button2Mask);
	case RightButton:
		return (buttonMask & Button3Mask);
	default:
		return false;
	}
#endif
}

QPoint MouseDevice::cursorPosition() const
{
	return QCursor::pos();
}

void MouseDevice::setCursorPosition(const QPoint &position) const
{
	QCursor::setPos(position);
}

void MouseDevice::pressButton(Button button)
{
	if(mPressedButtons[button])
		return;


}

void MouseDevice::releaseButton(Button button)
{
	if(!mPressedButtons[button])
		return;


}
