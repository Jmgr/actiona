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

	Contact: jmgr@jmgr.info
*/

#include "keyboard.hpp"
#include "backend/keyboard.hpp"

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
        auto &keyboard = Backend::Instance::keyboard();

        try
        {
            keyboard.pressKey(key, true, false);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("KeyboardError"), e.what());
        }
		
		return thisObject();
	}
	
	QScriptValue Keyboard::releaseKey(const QString &key)
	{
        auto &keyboard = Backend::Instance::keyboard();

        try
        {
            keyboard.pressKey(key, false, false);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("KeyboardError"), e.what());
        }
		
		return thisObject();
	}
	
	QScriptValue Keyboard::triggerKey(const QString &key)
	{
        auto &keyboard = Backend::Instance::keyboard();

        try
        {
            keyboard.pressKey(key, true, false);
            keyboard.pressKey(key, false, false);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("KeyboardError"), e.what());
        }
		
		return thisObject();
	}
	
    QScriptValue Keyboard::writeText(const QString &text, int delay, bool noUnicodeCharacters) const
	{
        auto &keyboard = Backend::Instance::keyboard();

        try
        {
            keyboard.writeText(text, delay, noUnicodeCharacters);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("KeyboardError"), e.what());
        }

		
		return thisObject();
	}
}
