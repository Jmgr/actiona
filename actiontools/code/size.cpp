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

#include "size.h"
#include "codetools.h"

namespace Code
{
	QScriptValue Size::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Size *size = nullptr;
		
		switch(context->argumentCount())
		{
		case 0:
			size = new Size;
			break;
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Size *codeSize = qobject_cast<Size*>(object))
					size = new Size(*codeSize);
				else
					throwError(context, engine, QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			}
			break;
		case 2:
			size = new Size(QSize(context->argument(0).toInt32(), context->argument(1).toInt32()));
			break;
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			break;
		}
		
		if(!size)
			return engine->undefinedValue();

		return CodeClass::constructor(size, context, engine);
	}
	
	QScriptValue Size::constructor(const QSize &size, QScriptEngine *engine)
	{
		return CodeClass::constructor(new Size(size), engine);
	}
	
	QSize Size::parameter(QScriptContext *context, QScriptEngine *engine)
	{
		switch(context->argumentCount())
		{
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Size *size = qobject_cast<Size*>(object))
					return size->size();
				else
					throwError(context, engine, QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			}
			return {};
		case 2:
			return QSize(context->argument(0).toInt32(),
						 context->argument(1).toInt32());
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			return QSize();
		}
	}

	void Size::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassToScriptEngine<Size>(scriptEngine);
	}
	
	Size::Size()
		: CodeClass()
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
	
	QScriptValue Size::clone() const
	{
		return constructor(mSize, engine());
	}

	bool Size::equals(const QScriptValue &other) const
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

	QScriptValue Size::setWidth(int width)
	{
		mSize.setWidth(width);
		
		return thisObject();
	}

	QScriptValue Size::setHeight(int height)
	{
		mSize.setHeight(height);
		
		return thisObject();
	}
}
