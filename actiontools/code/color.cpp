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

#include "color.h"

namespace Code
{
	QScriptValue Color::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Color *color = 0;
		
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
						context->throwError(tr("Invalid color name"));
						color = new Color;
					}
					else
						color = new Color(QColor(context->argument(0).toString()));
				}
				else
				{
					QObject *object = context->argument(0).toQObject();
					if(Color *codeColor = qobject_cast<Color*>(object))
						color = new Color(*codeColor);
					else
						context->throwError("Incorrect parameter type");
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
			context->throwError("Incorrect parameter count");
			break;
		}
		
		if(!color)
			return engine->undefinedValue();
	
		return engine->newQObject(color, QScriptEngine::ScriptOwnership);
	}
	
	QScriptValue Color::constructor(const QColor &color, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Color(color), QScriptEngine::ScriptOwnership);
	}
	
	Color::Color()
		: QObject(),
		QScriptable()
	{
		
	}

	Color::Color(const Color &other)
		: QObject(),
		QScriptable(),
		mColor(other.color())
	{
		
	}

	Color::Color(const QColor &color)
		: QObject(),
		QScriptable(),
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
	
	QScriptValue Color::clone() const
	{
		return constructor(mColor, context(), engine());
	}
	
	bool Color::equals(const QScriptValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(Color *otherColor = qobject_cast<Color*>(object))
			return (otherColor == this || otherColor->mColor == mColor);
			
		return false;
	}
	
	QScriptValue Color::setRed(int red)
	{
		mColor.setRed(red);
		
		return context()->thisObject();
	}

	QScriptValue Color::setGreen(int green)
	{
		mColor.setGreen(green);
		
		return context()->thisObject();
	}

	QScriptValue Color::setBlue(int blue)
	{
		mColor.setBlue(blue);
		
		return context()->thisObject();
	}

	QScriptValue Color::setAlpha(int alpha)
	{
		mColor.setAlpha(alpha);
		
		return context()->thisObject();
	}

	QScriptValue Color::setCmyk(int cyan, int magenta, int yellow, int black, int alpha)
	{
		mColor.setCmyk(cyan, magenta, yellow, black, alpha);
		
		return context()->thisObject();
	}

	QScriptValue Color::setHsl(int hue, int saturation, int lightness, int alpha)
	{
		mColor.setHsl(hue, saturation, lightness, alpha);
		
		return context()->thisObject();
	}

	QScriptValue Color::setHsv(int hue, int saturation, int value, int alpha)
	{
		mColor.setHsv(hue, saturation, value, alpha);
		
		return context()->thisObject();
	}

	QScriptValue Color::setNamedColor(const QString &name)
	{
		if(!QColor::isValidColor(name))
		{
			context()->throwError(tr("Invalid color name"));
			return context()->thisObject();
		}
		
		mColor.setNamedColor(name);
		
		return context()->thisObject();
	}

	QScriptValue Color::lighter(int factor)
	{
		mColor = mColor.lighter(factor);
		
		return context()->thisObject();
	}
	
	QScriptValue Color::darker(int factor)
	{
		mColor = mColor.darker(factor);
		
		return context()->thisObject();
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