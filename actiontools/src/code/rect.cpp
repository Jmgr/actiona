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

#include "actiontools/code/rect.hpp"
#include "actiontools/code/point.hpp"
#include "actiontools/code/size.hpp"

namespace Code
{
	Rect::Rect()
		: CodeClass()
	{
		
	}

    Rect::Rect(int left, int top, int width, int height)
        : CodeClass(),
        mRect(left, top, width, height)
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
	
	QJSValue Rect::clone() const
    {
        return CodeClass::clone<Rect>();
	}

	bool Rect::equals(const QJSValue &other) const
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
	
    Rect *Rect::normalize()
	{
		mRect = mRect.normalized();
		
        return this;
	}
	
    Rect *Rect::setTop(int top)
	{
		mRect.setTop(top);
		
        return this;
	}

    Rect *Rect::setBottom(int bottom)
	{
		mRect.setBottom(bottom);
		
        return this;
	}

    Rect *Rect::setLeft(int left)
	{
		mRect.setLeft(left);
		
        return this;
	}

    Rect *Rect::setRight(int right)
	{
		mRect.setRight(right);
		
        return this;
	}

    Rect *Rect::setX(int x)
	{
		mRect.setX(x);
		
        return this;
	}

    Rect *Rect::setY(int y)
	{
		mRect.setY(y);
		
        return this;
	}

    Rect *Rect::setWidth(int width)
	{
		mRect.setWidth(width);
		
        return this;
	}

    Rect *Rect::setHeight(int height)
	{
		mRect.setHeight(height);
		
        return this;
	}

    Rect *Rect::setSize(const Size *size)
    {
        mRect.setSize(size->size());
		
        return this;
	}

    Rect *Rect::setCoords(int x1, int y1, int x2, int y2)
	{
		mRect.setCoords(x1, y1, x2, y2);
		
        return this;
	}

    Rect *Rect::setRect(int x, int y, int width, int height)
	{
        mRect.setRect(x, y, width, height);
		
        return this;
	}

    Rect *Rect::translate(const Point *point)
	{
        mRect.translate(point->point());
		
        return this;
	}

    bool Rect::contains(const Point *point) const
    {
        return mRect.contains(point->point());
    }

    bool Rect::contains(const Rect *rect) const
    {
        return mRect.contains(rect->mRect);
    }

    bool Rect::contains(int x, int y) const
    {
        return mRect.contains(x, y);
    }

    bool Rect::contains(int left, int top, int width, int height) const
    {
        return mRect.contains(QRect(left, top, width, height));
    }

    QJSValue Rect::united(const Rect *rect) const
	{
        return CodeClass::construct<Rect>(mRect.united(rect->mRect));
    }
	
    QJSValue Rect::intersected(const Rect *rect) const
	{
        return CodeClass::construct<Rect>(mRect.intersected(rect->mRect));
	}
	
    bool Rect::intersects(const Rect *rect) const
	{
        return mRect.intersects(rect->mRect);
	}
	
	bool Rect::isEmpty() const
	{
		return mRect.isEmpty();
	}
	
	QJSValue Rect::center() const
	{
        return CodeClass::construct<Point>(mRect.center());
	}
	
	QJSValue Rect::size() const
	{
        return CodeClass::construct<Size>(mRect.size());
	}

    void Rect::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Rect*>("const Rect *");

        CodeClass::registerClass<Rect>(QStringLiteral("Rect"), scriptEngine);
    }
}
