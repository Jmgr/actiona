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

#include "actiontools/code/color.hpp"

namespace Code
{
	Color::Color()
		: CodeClass()
	{
		
	}

    Color::Color(const QString &colorName)
                : CodeClass()
    {
        if(!QColor::isValidColor(colorName))
            throwError(QStringLiteral("ColorNameError"), tr("Invalid color name"));
        else
            mColor = {colorName};
    }

    Color::Color(int red, int green, int blue, int alpha)
                : CodeClass(),
        mColor(red, green, blue, alpha)
    {

    }

	Color::Color(const Color &other)
		: CodeClass(),
		mColor(other.color())
	{
		
	}

	Color::Color(const QColor &color)
		: CodeClass(),
		mColor(color)
	{
		
	}
	
	Color &Color::operator=(Color other)
	{
		swap(other);
		
		return *this;
	}

	Color &Color::operator=(QColor color)
	{
		swap(color);
		
		return *this;
	}
	
	void Color::swap(Color &other)
	{
		std::swap(mColor, other.mColor);
	}

	void Color::swap(QColor &color)
	{
		std::swap(mColor, color);
	}
	
	const QColor &Color::color() const
	{
		return mColor;
	}

	int Color::red() const
	{
		return mColor.red();
	}

	int Color::green() const
	{
		return mColor.green();
	}

	int Color::blue() const
	{
		return mColor.blue();
	}

	int Color::alpha() const
	{
		return mColor.alpha();
	}
	
	QJSValue Color::clone() const
    {
        return CodeClass::clone<Color>();
	}
	
	bool Color::equals(const QJSValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(auto otherColor = qobject_cast<Color*>(object))
			return (otherColor == this || otherColor->mColor == mColor);
			
		return false;
	}

	QString Color::toString() const
	{
		return QStringLiteral("Color {red: %1, green: %2, blue: %3, alpha: %4}").arg(mColor.red()).arg(green()).arg(blue()).arg(alpha());
	}
	
    Color *Color::setRed(int red)
	{
		mColor.setRed(red);
		
        return this;
	}

    Color *Color::setGreen(int green)
	{
        mColor.setGreen(green);

        return this;
    }

    Color *Color::setBlue(int blue)
	{
        mColor.setBlue(blue);

        return this;
    }

    Color *Color::setAlpha(int alpha)
	{
        mColor.setAlpha(alpha);

        return this;
    }

    Color *Color::setCmyk(int cyan, int magenta, int yellow, int black, int alpha)
	{
        mColor.setCmyk(cyan, magenta, yellow, black, alpha);

        return this;
    }

    Color *Color::setHsl(int hue, int saturation, int lightness, int alpha)
	{
        mColor.setHsl(hue, saturation, lightness, alpha);

        return this;
    }

    Color *Color::setHsv(int hue, int saturation, int value, int alpha)
	{
        mColor.setHsv(hue, saturation, value, alpha);

        return this;
    }

    Color *Color::setNamedColor(const QString &name)
	{
		if(!QColor::isValidColor(name))
		{
            throwError(QStringLiteral("ColorNameError"), tr("Invalid color name"));
            return this;
        }
		
        mColor.setNamedColor(name);

        return this;
    }

    Color *Color::lighter(int factor)
	{
        mColor = mColor.lighter(factor);

        return this;
    }
	
    Color *Color::darker(int factor)
	{
        mColor = mColor.darker(factor);

        return this;
    }
	
	int Color::cyan() const
	{
		return mColor.cyan();
	}
	
	int Color::magenta() const
	{
		return mColor.magenta();
	}
	
	int Color::yellow() const
	{
		return mColor.yellow();
	}
	
	int Color::black() const
	{
		return mColor.black();
	}
	
	int Color::hue() const
	{
		return mColor.hue();
	}
	
	int Color::saturation() const
	{
		return mColor.saturation();
	}
	
	int Color::lightness() const
	{
		return mColor.lightness();
	}
	
	QString Color::name() const
	{
		return mColor.name();
	}

    void Color::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Color*>("const Color *");

        CodeClass::registerClass<Color>(QStringLiteral("Color"), scriptEngine);
    }
}
