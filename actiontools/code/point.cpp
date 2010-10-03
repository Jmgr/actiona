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

#include "point.h"

namespace Code
{
	QScriptValue Point::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Point *point = 0;
		
		switch(context->argumentCount())
		{
		case 0:
			point = new Point;
			break;
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Point *codePoint = qobject_cast<Point*>(object))
					point = new Point(*codePoint);
				else
					context->throwError("Incorrect parameter type");
			}
			break;
		case 2:
			point = new Point(QPoint(context->argument(0).toInt32(), context->argument(1).toInt32()));
			break;
		default:
			context->throwError("Incorrect parameter count");
			break;
		}
		
		if(!point)
			return engine->undefinedValue();

		return engine->newQObject(point, QScriptEngine::ScriptOwnership);
	}
	
	QScriptValue Point::constructor(const QPoint &point, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Point(point), QScriptEngine::ScriptOwnership);
	}
	
	QPoint Point::parameter(QScriptContext *context)
	{
		switch(context->argumentCount())
		{
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Point *point = qobject_cast<Point*>(object))
					return point->point();
				else
					context->throwError("Incorrect parameter type");
			}
			return QPoint();
		case 2:
			return QPoint(context->argument(0).toInt32(),
						 context->argument(1).toInt32());
		default:
			context->throwError("Incorrect parameter count");
			return QPoint();
		}
	}
	
	Point::Point()
		: QObject(),
		QScriptable()
	{
		
	}

	Point::Point(const Point &other)
		: QObject(),
		QScriptable(),
		mPoint(other.point())
	{
		
	}

	Point::Point(const QPoint &point)
		: QObject(),
		QScriptable(),
		mPoint(point)
	{
		
	}
	
	Point &Point::operator=(Point other)
	{
		swap(other);
		
		return *this;
	}

	Point &Point::operator=(QPoint point)
	{
		swap(point);
		
		return *this;
	}
	
	void Point::swap(Point &other)
	{
		std::swap(mPoint, other.mPoint);
	}

	void Point::swap(QPoint &point)
	{
		std::swap(mPoint, point);
	}
	
	const QPoint &Point::point() const
	{
		return mPoint;
	}
	
	QScriptValue Point::clone() const
	{
		return constructor(mPoint, context(), engine());
	}

	bool Point::equals(const QScriptValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(Point *otherPoint = qobject_cast<Point*>(object))
			return (otherPoint == this || otherPoint->mPoint == mPoint);
			
		return false;
	}

	QScriptValue Point::setX(int x)
	{
		mPoint.setX(x);
		
		return context()->thisObject();
	}

	QScriptValue Point::setY(int y)
	{
		mPoint.setY(y);
		
		return context()->thisObject();
	}

	int Point::x() const
	{
		return mPoint.x();
	}
	
	int Point::y() const
	{
		return mPoint.y();
	}
}
