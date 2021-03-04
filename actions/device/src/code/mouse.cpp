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

#include "mouse.hpp"
#include "actiontools/code/point.hpp"
#include "backend/mouse.hpp"

#include <QScriptValueIterator>

namespace Code
{
	QScriptValue Mouse::constructor(QScriptContext *context, QScriptEngine *engine)
	{
        return CodeClass::constructor(new Mouse, context, engine);
	}
	
	Mouse::Mouse()
		: CodeClass()
	{
	}

    Mouse::~Mouse() = default;

	QScriptValue Mouse::position() const
	{
        auto &mouse = Backend::Instance::mouse();

        try
        {
            return Point::constructor(mouse.cursorPosition(), engine());
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("MouseError"), e.what());
            return thisObject();
        }
	}

	QScriptValue Mouse::move() const
	{
        auto &mouse = Backend::Instance::mouse();

        try
        {
            mouse.setCursorPosition(Point::parameter(context(), engine()));
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("MouseError"), e.what());
        }
		
		return thisObject();
	}
	
	bool Mouse::isButtonPressed(Button button) const
	{
        auto &mouse = Backend::Instance::mouse();

        try
        {
            return mouse.isButtonPressed(static_cast<Backend::Mouse::Button>(button));
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("MouseError"), e.what());
            return false;
        }
	}
	
	QScriptValue Mouse::press(Button button)
	{
        auto &mouse = Backend::Instance::mouse();

        try
        {
            mouse.pressButton(static_cast<Backend::Mouse::Button>(button), true);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("MouseError"), e.what());
        }
		
		return thisObject();
	}

	QScriptValue Mouse::release(Button button)
	{
        auto &mouse = Backend::Instance::mouse();

        try
        {
            mouse.pressButton(static_cast<Backend::Mouse::Button>(button), false);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("MouseError"), e.what());
        }

		return thisObject();
	}

	QScriptValue Mouse::click(Button button)
	{
        auto &mouse = Backend::Instance::mouse();

        try
        {
            mouse.pressButton(static_cast<Backend::Mouse::Button>(button), true);
            mouse.pressButton(static_cast<Backend::Mouse::Button>(button), false);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("MouseError"), e.what());
        }

		return thisObject();
	}
	
	QScriptValue Mouse::wheel(int intensity) const
	{
        auto &mouse = Backend::Instance::mouse();

        try
        {
            mouse.wheel(intensity);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("MouseError"), e.what());
        }
		
		return thisObject();
	}
}
