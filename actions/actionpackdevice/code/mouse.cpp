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

#include "mouse.h"
#include "code/point.h"

namespace Code
{
	QScriptValue Mouse::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)

		return engine->newQObject(new Mouse, QScriptEngine::ScriptOwnership);
	}
	
	Mouse::Mouse()
		: QObject(),
		QScriptable()
	{
	}

	QScriptValue Mouse::position() const
	{
		return Point::constructor(mMouseDevice.cursorPosition(), context(), engine());
	}

	QScriptValue Mouse::move() const
	{
		mMouseDevice.setCursorPosition(Point::parameter(context()));
		
		return context()->thisObject();
	}
	
	bool Mouse::isButtonPressed(MouseDevice::Button button) const
	{
		return mMouseDevice.isButtonPressed(button);
	}
	
	QScriptValue Mouse::press(MouseDevice::Button button)
	{
		if(!mMouseDevice.pressButton(button))
			context()->throwError(tr("Unable to press the button"));
		
		return context()->thisObject();
	}

	QScriptValue Mouse::release(MouseDevice::Button button)
	{
		if(!mMouseDevice.releaseButton(button))
			context()->throwError(tr("Unable to release the button"));
		
		return context()->thisObject();
	}

	QScriptValue Mouse::click(MouseDevice::Button button)
	{
		if(!mMouseDevice.buttonClick(button))
			context()->throwError(tr("Unable to emulate a button click"));
		
		return context()->thisObject();
	}
	
	QScriptValue Mouse::wheel(int intensity) const
	{
		if(!mMouseDevice.wheel(intensity))
			context()->throwError(tr("Unable to emulate the wheel"));
		
		return context()->thisObject();
	}
}
