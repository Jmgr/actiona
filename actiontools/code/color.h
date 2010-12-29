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

#ifndef COLOR_H
#define COLOR_H

#include "actiontools_global.h"

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>
#include <QColor>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Color : public QObject, public QScriptable
	{
		Q_OBJECT
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(const QColor &color, QScriptContext *context, QScriptEngine *engine);

		static void registerClass(QScriptEngine *scriptEngine);
		
		Color();
		Color(const Color &other);
		Color(const QColor &color);
		
		Color &operator=(Color other);
		Color &operator=(QColor color);
		
		void swap(Color &other);
		void swap(QColor &color);
		
		const QColor &color() const;
		
	public slots:
		QScriptValue clone() const;
		bool equals(const QScriptValue &other) const;
		QString toString() const;
		QScriptValue setRed(int red);
		QScriptValue setGreen(int green);
		QScriptValue setBlue(int blue);
		QScriptValue setAlpha(int alpha);
		QScriptValue setCmyk(int cyan, int magenta, int yellow, int black, int alpha = 255);
		QScriptValue setHsl(int hue, int saturation, int lightness, int alpha = 255);
		QScriptValue setHsv(int hue, int saturation, int value, int alpha = 255);
		QScriptValue setNamedColor(const QString &name);
		QScriptValue lighter(int factor = 150);
		QScriptValue darker(int factor = 200);
		int red() const;
		int green() const;
		int blue() const;
		int alpha() const;
		int cyan() const;
		int magenta() const;
		int yellow() const;
		int black() const;
		int hue() const;
		int saturation() const;
		int lightness() const;
		QString name() const;
		
	private:
		QColor mColor;
	};
}

#endif // COLOR_H
