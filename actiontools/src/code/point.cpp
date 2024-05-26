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

#include "actiontools/code/point.hpp"

namespace Code
{
	Point::Point()
		: CodeClass()
	{
		
	}

    Point::Point(int x, int y)
        : CodeClass(),
        mPoint(x, y)
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
	
	QJSValue Point::clone() const
    {
        return CodeClass::clone<Point>();
	}

	bool Point::equals(const QJSValue &other) const
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

    Point *Point::setX(int x)
	{
		mPoint.setX(x);
		
        return this;
	}

    Point *Point::setY(int y)
	{
		mPoint.setY(y);
		
        return this;
	}

    void Point::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Point*>("const Point *");

        CodeClass::registerClass<Point>(QStringLiteral("Point"), scriptEngine);
    }
}
