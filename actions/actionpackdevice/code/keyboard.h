/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../keyboarddevice.h"
#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class Keyboard : public CodeClass
	{
		Q_OBJECT

	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
		Keyboard();

	public slots:
		QString toString() const					{ return "Keyboard"; }
		QScriptValue pressKey(const QString &key);
		QScriptValue releaseKey(const QString &key);
		QScriptValue triggerKey(const QString &key);
		QScriptValue writeText(const QString &text, int delay = 0) const;
        virtual bool equals(const QScriptValue &other) const { Q_UNUSED(other) return false; }

	private:
		KeyboardDevice mKeyboardDevice;
	};
}

#endif // KEYBOARD_H
