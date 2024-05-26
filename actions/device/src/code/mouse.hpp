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

#pragma once

#include "../mousedevice.hpp"
#include "actiontools/code/codeclass.hpp"
#include "actiontools/systeminput.hpp"

#include <QJSValue>

namespace Code
{
    class Point;

	class Mouse : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(QJSValue onMotion READ onMotion WRITE setOnMotion)
		Q_PROPERTY(QJSValue onWheel READ onWheel WRITE setOnWheel)
		Q_PROPERTY(QJSValue onButtonPressed READ onButtonPressed WRITE setOnButtonPressed)
		Q_PROPERTY(QJSValue onButtonReleased READ onButtonReleased WRITE setOnButtonReleased)

	public:
		enum Button
		{
			LeftButton,
			MiddleButton,
			RightButton
		};
        Q_ENUM(Button)
		
        Q_INVOKABLE Mouse();
        Q_INVOKABLE explicit Mouse(const QJSValue &parameters);
		~Mouse() override;

		void setOnMotion(const QJSValue &onMotion)					{ mOnMotion = onMotion; }
		void setOnWheel(const QJSValue &onWheel)					{ mOnWheel = onWheel; }
		void setOnButtonPressed(const QJSValue &onButtonPressed)	{ mOnButtonPressed = onButtonPressed; }
		void setOnButtonReleased(const QJSValue &onButtonReleased)	{ mOnButtonReleased = onButtonReleased; }

		QJSValue onMotion() const									{ return mOnMotion; }
		QJSValue onWheel() const									{ return mOnWheel; }
		QJSValue onButtonPressed() const							{ return mOnButtonPressed; }
		QJSValue onButtonReleased() const							{ return mOnButtonReleased; }

        Q_INVOKABLE QString toString() const override										{ return QStringLiteral("Mouse"); }
        Q_INVOKABLE QJSValue position() const;
        Q_INVOKABLE Mouse *move(const Point *point);
        Q_INVOKABLE bool isButtonPressed(Button button = LeftButton) const;
        Q_INVOKABLE Mouse *press(Button button = LeftButton);
        Q_INVOKABLE Mouse *release(Button button = LeftButton);
        Q_INVOKABLE Mouse *click(Button button = LeftButton);
        Q_INVOKABLE Mouse *wheel(int intensity = 1);

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private:
		void mouseMotion(int x, int y);
		void mouseWheel(int intensity);
		void mouseButtonPressed(ActionTools::SystemInput::Button button);
		void mouseButtonReleased(ActionTools::SystemInput::Button button);

	private:
		MouseDevice mMouseDevice;
		QJSValue mOnMotion;
		QJSValue mOnWheel;
		QJSValue mOnButtonPressed;
		QJSValue mOnButtonReleased;
	};
}
