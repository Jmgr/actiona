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

#include "actiontools/code/size.hpp"

namespace Code
{
	Size::Size()
		: CodeClass()
	{
		
	}

    Size::Size(int width, int height)
        : CodeClass(),
        mSize(width, height)
    {

    }

	Size::Size(const Size &other)
		: CodeClass(),
		mSize(other.size())
	{
		
	}

	Size::Size(const QSize &size)
		: CodeClass(),
		mSize(size)
	{
		
	}
	
	Size &Size::operator=(Size other)
	{
		swap(other);
		
		return *this;
	}

	Size &Size::operator=(QSize size)
	{
		swap(size);
		
		return *this;
	}
	
	void Size::swap(Size &other)
	{
		std::swap(mSize, other.mSize);
	}

	void Size::swap(QSize &size)
	{
		std::swap(mSize, size);
	}
	
	const QSize &Size::size() const
	{
		return mSize;
	}

	int Size::width() const
	{
		return mSize.width();
	}

	int Size::height() const
	{
		return mSize.height();
	}
	
	QJSValue Size::clone() const
    {
        return CodeClass::clone<Size>();
	}

	bool Size::equals(const QJSValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(Size *otherSize = qobject_cast<Size*>(object))
			return (otherSize == this || otherSize->mSize == mSize);
			
		return false;
	}

	QString Size::toString() const
	{
		return QStringLiteral("Size {width: %1, height: %2}").arg(width()).arg(height());
	}

    Size *Size::setWidth(int width)
	{
		mSize.setWidth(width);
		
        return this;
	}

    Size *Size::setHeight(int height)
	{
        mSize.setHeight(height);

        return this;
    }

    void Size::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Size*>("const Size *");

        CodeClass::registerClass<Size>(QStringLiteral("Size"), scriptEngine);
    }
}
