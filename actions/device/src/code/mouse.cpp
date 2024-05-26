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

#include <QJSValueIterator>

namespace Code
{
	Mouse::Mouse()
		: CodeClass()
	{
	}

    Mouse::Mouse(const QJSValue &parameters)
        : Mouse()
    {
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return;
        }

        QJSValueIterator it(parameters);

        while(it.hasNext())
        {
            it.next();

            if(it.name() == QLatin1String("onMotion"))
                mOnMotion = it.value();
            else if(it.name() == QLatin1String("onWheel"))
                mOnWheel = it.value();
            else if(it.name() == QLatin1String("onButtonPressed"))
                mOnButtonPressed = it.value();
            else if(it.name() == QLatin1String("onButtonReleased"))
                mOnButtonReleased = it.value();
        }
    }

    Mouse::~Mouse() = default;

	QJSValue Mouse::position() const
	{
        return CodeClass::construct<Point>(mMouseDevice.cursorPosition());
	}

    Mouse *Mouse::move(const Point *point)
    {
        mMouseDevice.setCursorPosition(point->point());
		
        return this;
	}
	
	bool Mouse::isButtonPressed(Button button) const
	{
		return mMouseDevice.isButtonPressed(static_cast<MouseDevice::Button>(button));
	}
	
    Mouse *Mouse::press(Button button)
	{
		if(!mMouseDevice.pressButton(static_cast<MouseDevice::Button>(button)))
			throwError(QStringLiteral("PressButtonError"), tr("Unable to press the button"));
		
        return this;
	}

    Mouse *Mouse::release(Button button)
	{
		if(!mMouseDevice.releaseButton(static_cast<MouseDevice::Button>(button)))
			throwError(QStringLiteral("ReleaseButtonError"), tr("Unable to release the button"));
		
        return this;
	}

    Mouse *Mouse::click(Button button)
	{
		if(!mMouseDevice.buttonClick(static_cast<MouseDevice::Button>(button)))
			throwError(QStringLiteral("ClickError"), tr("Unable to emulate a button click"));
		
        return this;
	}
	
    Mouse *Mouse::wheel(int intensity)
	{
		if(!mMouseDevice.wheel(intensity))
			throwError(QStringLiteral("WheelError"), tr("Unable to emulate the wheel"));
		
        return this;
	}

    void Mouse::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Mouse*>("const Mouse *");

        CodeClass::registerClass<Mouse>(QStringLiteral("Mouse"), scriptEngine);
    }

	void Mouse::mouseMotion(int x, int y)
	{
        if(!mOnMotion.isUndefined())
            mOnMotion.call(QJSValueList() << x << y);
	}

	void Mouse::mouseWheel(int intensity)
	{
        if(!mOnWheel.isUndefined())
            mOnWheel.call(QJSValueList() << intensity);
	}

	void Mouse::mouseButtonPressed(ActionTools::SystemInput::Button button)
	{
        if(!mOnButtonPressed.isUndefined())
            mOnButtonPressed.call(QJSValueList() << button);
	}

	void Mouse::mouseButtonReleased(ActionTools::SystemInput::Button button)
	{
        if(!mOnButtonReleased.isUndefined())
            mOnButtonReleased.call(QJSValueList() << button);
	}
}
