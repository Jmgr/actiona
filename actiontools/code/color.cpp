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

#include "color.h"
#include "codetools.h"

namespace Code
{
	QScriptValue Color::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Color *color = nullptr;
		
		switch(context->argumentCount())
		{
		case 0:
			color = new Color;
			break;
		case 1:
			{
				if(context->argument(0).isString())
				{
					if(!QColor::isValidColor(context->argument(0).toString()))
					{
						throwError(context, engine, QStringLiteral("ColorNameError"), tr("Invalid color name"));
						color = new Color;
					}
					else
						color = new Color(QColor(context->argument(0).toString()));
				}
				else
				{
					QObject *object = context->argument(0).toQObject();
					if(auto codeColor = qobject_cast<Color*>(object))
						color = new Color(*codeColor);
					else
						throwError(context, engine, QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
				}
			}
			break;
		case 3:
			color = new Color(QColor(context->argument(0).toInt32(), context->argument(1).toInt32(), context->argument(2).toInt32()));
			break;
		case 4:
			color = new Color(QColor(context->argument(0).toInt32(), context->argument(1).toInt32(), context->argument(2).toInt32(), context->argument(3).toInt32()));
			break;
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			break;
		}
		
		if(!color)
			return engine->undefinedValue();
	
		return CodeClass::constructor(color, context, engine);
	}
	
	QScriptValue Color::constructor(const QColor &color, QScriptEngine *engine)
	{
		return CodeClass::constructor(new Color(color), engine);
	}

	void Color::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassToScriptEngine<Color>(scriptEngine);
	}
	
	Color::Color()
		: CodeClass()
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
	
	QScriptValue Color::clone() const
	{
		return constructor(mColor, engine());
	}
	
	bool Color::equals(const QScriptValue &other) const
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
	
	QScriptValue Color::setRed(int red)
	{
		mColor.setRed(red);
		
		return thisObject();
	}

	QScriptValue Color::setGreen(int green)
	{
		mColor.setGreen(green);
		
		return thisObject();
	}

	QScriptValue Color::setBlue(int blue)
	{
		mColor.setBlue(blue);
		
		return thisObject();
	}

	QScriptValue Color::setAlpha(int alpha)
	{
		mColor.setAlpha(alpha);
		
		return thisObject();
	}

	QScriptValue Color::setCmyk(int cyan, int magenta, int yellow, int black, int alpha)
	{
		mColor.setCmyk(cyan, magenta, yellow, black, alpha);
		
		return thisObject();
	}

	QScriptValue Color::setHsl(int hue, int saturation, int lightness, int alpha)
	{
		mColor.setHsl(hue, saturation, lightness, alpha);
		
		return thisObject();
	}

	QScriptValue Color::setHsv(int hue, int saturation, int value, int alpha)
	{
		mColor.setHsv(hue, saturation, value, alpha);
		
		return thisObject();
	}

	QScriptValue Color::setNamedColor(const QString &name)
	{
		if(!QColor::isValidColor(name))
		{
			throwError(QStringLiteral("ColorNameError"), tr("Invalid color name"));
			return thisObject();
		}
		
		mColor.setNamedColor(name);
		
		return thisObject();
	}

	QScriptValue Color::lighter(int factor)
	{
		mColor = mColor.lighter(factor);
		
		return thisObject();
	}
	
	QScriptValue Color::darker(int factor)
	{
		mColor = mColor.darker(factor);
		
		return thisObject();
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
}
