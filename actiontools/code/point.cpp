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

#include "point.h"
#include "codetools.h"

namespace Code
{
	QScriptValue Point::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Point *point = nullptr;
		
		switch(context->argumentCount())
		{
		case 0:
			point = new Point;
			break;
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
                if(auto codePoint = qobject_cast<Point*>(object))
					point = new Point(*codePoint);
				else
					throwError(context, engine, QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			}
			break;
		case 2:
			point = new Point(QPoint(context->argument(0).toInt32(), context->argument(1).toInt32()));
			break;
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			break;
		}
		
		if(!point)
			return engine->undefinedValue();

		return CodeClass::constructor(point, context, engine);
	}
	
	QScriptValue Point::constructor(const QPoint &point, QScriptEngine *engine)
	{
		return CodeClass::constructor(new Point(point), engine);
	}
	
	QPoint Point::parameter(QScriptContext *context, QScriptEngine *engine)
	{
		switch(context->argumentCount())
		{
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
                if(auto point = qobject_cast<Point*>(object))
					return point->point();
				else
					throwError(context, engine, QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			}
			return {};
		case 2:
			return QPoint(context->argument(0).toInt32(),
						 context->argument(1).toInt32());
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			return QPoint();
		}
	}

	void Point::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassToScriptEngine<Point>(scriptEngine);
	}
	
	Point::Point()
		: CodeClass()
	{
		
	}

	Point::Point(const Point &other)
		: CodeClass(),
		mPoint(other.point())
	{
		
	}

	Point::Point(const QPoint &point)
		: CodeClass(),
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

	int Point::x() const
	{
		return mPoint.x();
	}

	int Point::y() const
	{
		return mPoint.y();
	}
	
	QScriptValue Point::clone() const
	{
		return constructor(mPoint, engine());
	}

	bool Point::equals(const QScriptValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
        if(auto otherPoint = qobject_cast<Point*>(object))
			return (otherPoint == this || otherPoint->mPoint == mPoint);
			
		return false;
	}

	QString Point::toString() const
	{
		return QStringLiteral("Point {x: %1, y: %2}").arg(x()).arg(y());
	}

	QScriptValue Point::setX(int x)
	{
		mPoint.setX(x);
		
		return thisObject();
	}

	QScriptValue Point::setY(int y)
	{
		mPoint.setY(y);
		
		return thisObject();
	}
}
