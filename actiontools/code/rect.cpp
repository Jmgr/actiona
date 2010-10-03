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

#include "rect.h"
#include "point.h"
#include "size.h"

namespace Code
{
	QScriptValue Rect::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Rect *rect = 0;
		
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
					context->throwError("Incorrect parameter type");
			}
			break;
		case 4:
			rect = new Rect(QRect(context->argument(0).toInt32(),
								  context->argument(1).toInt32(),
								  context->argument(2).toInt32(),
								  context->argument(3).toInt32()));
			break;
		default:
			context->throwError("Incorrect parameter count");
			break;
		}
		
		if(!rect)
			return engine->undefinedValue();

		return engine->newQObject(rect, QScriptEngine::ScriptOwnership);
	}
	
	QScriptValue Rect::constructor(const QRect &rect, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Rect(rect), QScriptEngine::ScriptOwnership);
	}
	
	QRect Rect::parameter(QScriptContext *context)
	{
		switch(context->argumentCount())
		{
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Rect *rect = qobject_cast<Rect*>(object))
					return rect->mRect;
				else
					context->throwError("Incorrect parameter type");
			}
			return QRect();
		case 4:
			return QRect(context->argument(0).toInt32(),
						 context->argument(1).toInt32(),
						 context->argument(2).toInt32(),
						 context->argument(3).toInt32());
		default:
			context->throwError("Incorrect parameter count");
			return QRect();
		}
	}
	
	Rect::Rect()
		: QObject(),
		QScriptable()
	{
		
	}

	Rect::Rect(const Rect &other)
		: QObject(),
		QScriptable(),
		mRect(other.rect())
	{
		
	}

	Rect::Rect(const QRect &rect)
		: QObject(),
		QScriptable(),
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
	
	QScriptValue Rect::clone() const
	{
		return constructor(mRect, context(), engine());
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
	
	QScriptValue Rect::normalize()
	{
		mRect = mRect.normalized();
		
		return context()->thisObject();
	}
	
	QScriptValue Rect::setTop(int top)
	{
		mRect.setTop(top);
		
		return context()->thisObject();
	}

	QScriptValue Rect::setBottom(int bottom)
	{
		mRect.setBottom(bottom);
		
		return context()->thisObject();
	}

	QScriptValue Rect::setLeft(int left)
	{
		mRect.setLeft(left);
		
		return context()->thisObject();
	}

	QScriptValue Rect::setRight(int right)
	{
		mRect.setRight(right);
		
		return context()->thisObject();
	}

	QScriptValue Rect::setX(int x)
	{
		mRect.setX(x);
		
		return context()->thisObject();
	}

	QScriptValue Rect::setY(int y)
	{
		mRect.setY(y);
		
		return context()->thisObject();
	}

	QScriptValue Rect::setWidth(int width)
	{
		mRect.setWidth(width);
		
		return context()->thisObject();
	}

	QScriptValue Rect::setHeight(int height)
	{
		mRect.setHeight(height);
		
		return context()->thisObject();
	}

	QScriptValue Rect::setSize()
	{
		mRect.setSize(Size::parameter(context()));
		
		return context()->thisObject();
	}

	QScriptValue Rect::setCoords(int x1, int y1, int x2, int y2)
	{
		mRect.setCoords(x1, y1, x2, y2);
		
		return context()->thisObject();
	}

	QScriptValue Rect::setRect()
	{
		const QRect &rect = parameter(context());
		mRect.setRect(rect.x(), rect.y(), rect.width(), rect.height());
		
		return context()->thisObject();
	}

	QScriptValue Rect::translate()
	{
		mRect.translate(Point::parameter(context()));
		
		return context()->thisObject();
	}

	QScriptValue Rect::moveTop(int y)
	{
		mRect.moveTop(y);
		
		return context()->thisObject();
	}

	QScriptValue Rect::moveBottom(int y)
	{
		mRect.moveBottom(y);
		
		return context()->thisObject();
	}

	QScriptValue Rect::moveLeft(int x)
	{
		mRect.moveLeft(x);
		
		return context()->thisObject();
	}

	QScriptValue Rect::moveRight(int x)
	{
		mRect.moveRight(x);
		
		return context()->thisObject();
	}

	QScriptValue Rect::moveCenter()
	{
		mRect.moveCenter(Point::parameter(context()));
		
		return context()->thisObject();
	}

	QScriptValue Rect::moveTo()
	{
		mRect.moveTo(Point::parameter(context()));
		
		return context()->thisObject();
	}

	bool Rect::contains(const QScriptValue &point) const
	{
		switch(context()->argumentCount())
		{
		case 1:
			{
				QObject *object = point.toQObject();
				if(Point *codePoint = qobject_cast<Point*>(object))
					return mRect.contains(codePoint->point());
				else if(Rect *codeRect = qobject_cast<Rect*>(object))
					return (codeRect == this || mRect.contains(codeRect->mRect));
				else
					context()->throwError("Incorrect parameter type");
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
			context()->throwError("Incorrect parameter count");
			return false;
		}
	}
	
	QScriptValue Rect::united() const
	{
		return constructor(mRect.united(parameter(context())), context(), engine());
	}
	
	QScriptValue Rect::intersected() const
	{
		return constructor(mRect.intersected(parameter(context())), context(), engine());
	}
	
	bool Rect::intersects() const
	{
		return mRect.intersects(parameter(context()));
	}
	
	bool Rect::isEmpty() const
	{
		return mRect.isEmpty();
	}
	
	QScriptValue Rect::center() const
	{
		return Point::constructor(mRect.center(), context(), engine());
	}
	
	QScriptValue Rect::size() const
	{
		return Size::constructor(mRect.size(), context(), engine());
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
}
