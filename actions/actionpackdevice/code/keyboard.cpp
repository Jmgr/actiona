/*
	Actiona
	Copyright (C) 2005 Jonathan Mercier-Ganady

	Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actiona is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "keyboard.h"

namespace Code
{
	QScriptValue Keyboard::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		return CodeClass::constructor(new Keyboard, context, engine);
	}
	
	Keyboard::Keyboard()
		: CodeClass()
	{
	}
	
	QScriptValue Keyboard::pressKey(const QString &key)
	{
		if(!mKeyboardDevice.pressKey(key))
			throwError(QStringLiteral("PressKeyError"), tr("Unable to press the key"));
		
		return thisObject();
	}
	
	QScriptValue Keyboard::releaseKey(const QString &key)
	{
		if(!mKeyboardDevice.releaseKey(key))
			throwError(QStringLiteral("ReleaseKeyError"), tr("Unable to release the key"));
		
		return thisObject();
	}
	
	QScriptValue Keyboard::triggerKey(const QString &key)
	{
		if(!mKeyboardDevice.triggerKey(key))
			throwError(QStringLiteral("TriggerKeyError"), tr("Unable to trigger the key"));
		
		return thisObject();
	}
	
    QScriptValue Keyboard::writeText(const QString &text, int delay, bool noUnicodeCharacters) const
	{
        if(!mKeyboardDevice.writeText(text, delay, noUnicodeCharacters))
			throwError(QStringLiteral("WriteTextError"), tr("Unable to write the text"));
		
		return thisObject();
	}
}
