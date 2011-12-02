/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#include "mouse.h"
#include "code/point.h"

#include <QDebug>

/*
#ifdef Q_WS_X11
#include <X11/Xlib.h>
#include <QX11Info>
#endif
*/

namespace Code
{
	QScriptValue Mouse::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		return CodeClass::constructor(new Mouse, context, engine);
	}
	
	Mouse::Mouse()
		: CodeClass()
	{
		nativeEventFilteringApp->installNativeEventFilter(this);
/*
#ifdef Q_WS_X11
		if(XGrabPointer(QX11Info::display(), DefaultRootWindow(QX11Info::display()), True, ButtonReleaseMask | ButtonPressMask | PointerMotionMask, GrabModeAsync, GrabModeAsync,
						None, None, CurrentTime) != GrabSuccess)
		{
			qDebug() << "Unable to grab the pointer.";
		}
		if(XGrabKeyboard(QX11Info::display(), DefaultRootWindow(QX11Info::display()), True, GrabModeAsync, GrabModeAsync, CurrentTime) != GrabSuccess)
		{
			qDebug() << "Unable to grab the keyboard.";
		}
#endif
*/
	}

	Mouse::~Mouse()
	{
		/*
#ifdef Q_WS_X11
		XUngrabPointer(QX11Info::display(), CurrentTime);
		XUngrabKeyboard(QX11Info::display(), CurrentTime);
#endif
*/
		nativeEventFilteringApp->removeNativeEventFilter(this);
	}

	QScriptValue Mouse::position() const
	{
		return Point::constructor(mMouseDevice.cursorPosition(), engine());
	}

	QScriptValue Mouse::move() const
	{
		mMouseDevice.setCursorPosition(Point::parameter(context(), engine()));
		
		return thisObject();
	}
	
	bool Mouse::isButtonPressed(MouseDevice::Button button) const
	{
		return mMouseDevice.isButtonPressed(button);
	}
	
	QScriptValue Mouse::press(MouseDevice::Button button)
	{
		if(!mMouseDevice.pressButton(button))
			throwError("PressButtonError", tr("Unable to press the button"));
		
		return thisObject();
	}

	QScriptValue Mouse::release(MouseDevice::Button button)
	{
		if(!mMouseDevice.releaseButton(button))
			throwError("ReleaseButtonError", tr("Unable to release the button"));
		
		return thisObject();
	}

	QScriptValue Mouse::click(MouseDevice::Button button)
	{
		if(!mMouseDevice.buttonClick(button))
			throwError("ClickError", tr("Unable to emulate a button click"));
		
		return thisObject();
	}
	
	QScriptValue Mouse::wheel(int intensity) const
	{
		if(!mMouseDevice.wheel(intensity))
			throwError("WheelError", tr("Unable to emulate the wheel"));
		
		return thisObject();
	}

	/*
#ifdef Q_WS_X11
	bool Mouse::x11EventFilter(XEvent *event)
	{
		if(event->type == ButtonRelease)
		{
			XButtonEvent *bevent = (XButtonEvent *)event;

			qDebug() << "release " << bevent->button;
		}
		else if(event->type == ButtonPress)
		{
			XButtonEvent *bevent = (XButtonEvent *)event;

			qDebug() << "press " << bevent->button;
		}
		else if(event->type == MotionNotify)
			qDebug() << "motion " << QCursor::pos();
		else if(event->type == KeyPress)
			qDebug() << "key press";
		else if(event->type == KeyRelease)
			qDebug() << "key release";
		return false;
	}
#endif
*/
}
