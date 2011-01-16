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

#ifndef MOUSE_H
#define MOUSE_H

#include "../mousedevice.h"
#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class Mouse : public CodeClass
	{
		Q_OBJECT
		Q_ENUMS(MouseDevice::Button)

	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
		Mouse();

	public slots:
		QString toString() const					{ return "Mouse"; }
		QScriptValue position() const;
		QScriptValue move() const;
		bool isButtonPressed(MouseDevice::Button button) const;
		QScriptValue press(MouseDevice::Button button = MouseDevice::LeftButton);
		QScriptValue release(MouseDevice::Button button = MouseDevice::LeftButton);
		QScriptValue click(MouseDevice::Button button = MouseDevice::LeftButton);
		QScriptValue wheel(int intensity = 1) const;

	private:
		MouseDevice mMouseDevice;
	};
}

#endif // MOUSE_H
