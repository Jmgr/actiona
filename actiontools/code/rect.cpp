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

#include "rect.h"
#include "point.h"
#include "size.h"
#include "codetools.h"

namespace Code
{
	QScriptValue Rect::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Rect *rect = nullptr;
		
		switch(context->argumentCount())
		{
		case 0:
			rect = new Rect;
			break;
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Rect *codeRect = qobject_cast<Rect*>(object))
					rect = new Rect(*codeRect);
				else
					throwError(context, engine, QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			}
			break;
		case 4:
			rect = new Rect(QRect(context->argument(0).toInt32(),
								  context->argument(1).toInt32(),
								  context->argument(2).toInt32(),
								  context->argument(3).toInt32()));
			break;
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			break;
		}
		
		if(!rect)
			return engine->undefinedValue();

		return CodeClass::constructor(rect, context, engine);
	}
	
	QScriptValue Rect::constructor(const QRect &rect, QScriptEngine *engine)
	{
		return CodeClass::constructor(new Rect(rect), engine);
	}
	
	QRect Rect::parameter(QScriptContext *context, QScriptEngine *engine)
	{
		switch(context->argumentCount())
		{
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Rect *rect = qobject_cast<Rect*>(object))
					return rect->mRect;
				else
					throwError(context, engine, QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			}
			return {};
		case 4:
			return QRect(context->argument(0).toInt32(),
						 context->argument(1).toInt32(),
						 context->argument(2).toInt32(),
						 context->argument(3).toInt32());
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			return QRect();
		}
	}

	void Rect::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassToScriptEngine<Rect>(scriptEngine);
	}
	
	Rect::Rect()
		: CodeClass()
	{
		
	}

	Rect::Rect(const Rect &other)
		: CodeClass(),
		mRect(other.rect())
	{
		
	}

	Rect::Rect(const QRect &rect)
		: CodeClass(),
		mRect(rect)
	{
		
	}
	
	Rect &Rect::operator=(Rect other)
	{
		swap(other);
		
		return *this;
	}

	Rect &Rect::operator=(QRect rect)
	{
		swap(rect);
		
		return *this;
	}
	
	void Rect::swap(Rect &other)
	{
		std::swap(mRect, other.mRect);
	}

	void Rect::swap(QRect &rect)
	{
		std::swap(mRect, rect);
	}
	
	const QRect &Rect::rect() const
	{
		return mRect;
	}

	int Rect::width() const
	{
		return mRect.width();
	}

	int Rect::height() const
	{
		return mRect.height();
	}

	int Rect::x() const
	{
		return mRect.x();
	}

	int Rect::y() const
	{
		return mRect.y();
	}

	int Rect::left() const
	{
		return mRect.left();
	}

	int Rect::right() const
	{
		return mRect.right();
	}

	int Rect::top() const
	{
		return mRect.top();
	}

	int Rect::bottom() const
	{
		return mRect.bottom();
	}
	
	QScriptValue Rect::clone() const
	{
		return constructor(mRect, engine());
	}

	bool Rect::equals(const QScriptValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(Rect *otherRect = qobject_cast<Rect*>(object))
			return (otherRect == this || otherRect->mRect == mRect);
			
		return false;
	}
	
	QString Rect::toString() const
	{
		return QStringLiteral("Rect {x: %1, y: %2, width: %3, height: %4}").arg(x()).arg(y()).arg(width()).arg(height());
	}
	
	QScriptValue Rect::normalize()
	{
		mRect = mRect.normalized();
		
		return thisObject();
	}
	
	QScriptValue Rect::setTop(int top)
	{
		mRect.setTop(top);
		
		return thisObject();
	}

	QScriptValue Rect::setBottom(int bottom)
	{
		mRect.setBottom(bottom);
		
		return thisObject();
	}

	QScriptValue Rect::setLeft(int left)
	{
		mRect.setLeft(left);
		
		return thisObject();
	}

	QScriptValue Rect::setRight(int right)
	{
		mRect.setRight(right);
		
		return thisObject();
	}

	QScriptValue Rect::setX(int x)
	{
		mRect.setX(x);
		
		return thisObject();
	}

	QScriptValue Rect::setY(int y)
	{
		mRect.setY(y);
		
		return thisObject();
	}

	QScriptValue Rect::setWidth(int width)
	{
		mRect.setWidth(width);
		
		return thisObject();
	}

	QScriptValue Rect::setHeight(int height)
	{
		mRect.setHeight(height);
		
		return thisObject();
	}

	QScriptValue Rect::setSize()
	{
		mRect.setSize(Size::parameter(context(), engine()));
		
		return thisObject();
	}

	QScriptValue Rect::setCoords(int x1, int y1, int x2, int y2)
	{
		mRect.setCoords(x1, y1, x2, y2);
		
		return thisObject();
	}

	QScriptValue Rect::setRect()
	{
		const QRect &rect = parameter(context(), engine());
		mRect.setRect(rect.x(), rect.y(), rect.width(), rect.height());
		
		return thisObject();
	}

	QScriptValue Rect::translate()
	{
		mRect.translate(Point::parameter(context(), engine()));
		
		return thisObject();
	}

	bool Rect::contains(const QScriptValue &point) const
	{
		switch(context()->argumentCount())
		{
		case 1:
			{
				QObject *object = point.toQObject();
				if(auto codePoint = qobject_cast<Point*>(object))
					return mRect.contains(codePoint->point());
				else if(Rect *codeRect = qobject_cast<Rect*>(object))
					return (codeRect == this || mRect.contains(codeRect->mRect));
				else
					throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			}
			return false;
		case 2:
			return mRect.contains(QPoint(context()->argument(0).toInt32(), context()->argument(1).toInt32()));
		case 4:
			return mRect.contains(QRect(context()->argument(0).toInt32(),
										context()->argument(1).toInt32(),
										context()->argument(2).toInt32(),
										context()->argument(3).toInt32()));
		default:
			throwError(QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			return false;
		}
	}
	
	QScriptValue Rect::united() const
	{
		return constructor(mRect.united(parameter(context(), engine())), engine());
	}
	
	QScriptValue Rect::intersected() const
	{
		return constructor(mRect.intersected(parameter(context(), engine())), engine());
	}
	
	bool Rect::intersects() const
	{
		return mRect.intersects(parameter(context(), engine()));
	}
	
	bool Rect::isEmpty() const
	{
		return mRect.isEmpty();
	}
	
	QScriptValue Rect::center() const
	{
		return Point::constructor(mRect.center(), engine());
	}
	
	QScriptValue Rect::size() const
	{
		return Size::constructor(mRect.size(), engine());
	}
}
