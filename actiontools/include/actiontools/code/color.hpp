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

#include "actiontools/actiontools_global.hpp"
#include "codeclass.hpp"

#include <QColor>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Color : public CodeClass
	{
		Q_OBJECT
        Q_PROPERTY(int red READ red WRITE setRed)
        Q_PROPERTY(int green READ green WRITE setGreen)
        Q_PROPERTY(int blue READ blue WRITE setBlue)
        Q_PROPERTY(int alpha READ alpha WRITE setAlpha)
		
	public:
        Q_INVOKABLE Color();
        Q_INVOKABLE explicit Color(const QString &colorName);
        Q_INVOKABLE explicit Color(int red, int green, int blue, int alpha = 255);
        Q_INVOKABLE Color(const Color &other);
		Color(const QColor &color);
		
		Color &operator=(Color other);
		Color &operator=(QColor color);
		
		void swap(Color &other);
		void swap(QColor &color);
		
		const QColor &color() const;

		int red() const;
		int green() const;
		int blue() const;
		int alpha() const;

        Q_INVOKABLE QJSValue clone() const;
        Q_INVOKABLE bool equals(const QJSValue &other) const;
        Q_INVOKABLE QString toString() const override;
        Q_INVOKABLE Color *setRed(int red);
        Q_INVOKABLE Color *setGreen(int green);
        Q_INVOKABLE Color *setBlue(int blue);
        Q_INVOKABLE Color *setAlpha(int alpha);
        Q_INVOKABLE Color *setCmyk(int cyan, int magenta, int yellow, int black, int alpha = 255);
        Q_INVOKABLE Color *setHsl(int hue, int saturation, int lightness, int alpha = 255);
        Q_INVOKABLE Color *setHsv(int hue, int saturation, int value, int alpha = 255);
        Q_INVOKABLE Color *setNamedColor(const QString &name);
        Q_INVOKABLE Color *lighter(int factor = 150);
        Q_INVOKABLE Color *darker(int factor = 200);
        Q_INVOKABLE int cyan() const;
        Q_INVOKABLE int magenta() const;
        Q_INVOKABLE int yellow() const;
        Q_INVOKABLE int black() const;
        Q_INVOKABLE int hue() const;
        Q_INVOKABLE int saturation() const;
        Q_INVOKABLE int lightness() const;
        Q_INVOKABLE QString name() const;

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
		
	private:
		QColor mColor;
	};
}

