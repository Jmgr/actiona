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

#pragma once

#include "../mousedevice.h"
#include "code/codeclass.h"
#include "systeminputlistener.h"
#include "systeminputrecorder.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class Mouse : public CodeClass
	{
		Q_OBJECT
		Q_ENUMS(Button)
		Q_PROPERTY(QScriptValue onMotion READ onMotion WRITE setOnMotion)
		Q_PROPERTY(QScriptValue onWheel READ onWheel WRITE setOnWheel)
		Q_PROPERTY(QScriptValue onButtonPressed READ onButtonPressed WRITE setOnButtonPressed)
		Q_PROPERTY(QScriptValue onButtonReleased READ onButtonReleased WRITE setOnButtonReleased)

	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

		enum Button
		{
			LeftButton,
			MiddleButton,
			RightButton
		};
		
		Mouse();
		~Mouse() override;

		void setOnMotion(const QScriptValue &onMotion)					{ mOnMotion = onMotion; }
		void setOnWheel(const QScriptValue &onWheel)					{ mOnWheel = onWheel; }
		void setOnButtonPressed(const QScriptValue &onButtonPressed)	{ mOnButtonPressed = onButtonPressed; }
		void setOnButtonReleased(const QScriptValue &onButtonReleased)	{ mOnButtonReleased = onButtonReleased; }

		QScriptValue onMotion() const									{ return mOnMotion; }
		QScriptValue onWheel() const									{ return mOnWheel; }
		QScriptValue onButtonPressed() const							{ return mOnButtonPressed; }
		QScriptValue onButtonReleased() const							{ return mOnButtonReleased; }

	public slots:
		QString toString() const override										{ return QStringLiteral("Mouse"); }
        bool equals(const QScriptValue &other) const override            { return defaultEqualsImplementation<Mouse>(other); }
		QScriptValue position() const;
		QScriptValue move() const;
		bool isButtonPressed(Button button = LeftButton) const;
		QScriptValue press(Button button = LeftButton);
		QScriptValue release(Button button = LeftButton);
		QScriptValue click(Button button = LeftButton);
		QScriptValue wheel(int intensity = 1) const;

	private:
		void mouseMotion(int x, int y);
		void mouseWheel(int intensity);
		void mouseButtonPressed(ActionTools::SystemInput::Button button);
		void mouseButtonReleased(ActionTools::SystemInput::Button button);

	private:
		MouseDevice mMouseDevice;
		QScriptValue mOnMotion;
		QScriptValue mOnWheel;
		QScriptValue mOnButtonPressed;
		QScriptValue mOnButtonReleased;
	};
}

Q_DECLARE_METATYPE(MouseDevice::Button)

