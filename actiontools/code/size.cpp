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

#include "size.h"

namespace Code
{
	QScriptValue Size::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Size *size = 0;
		
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
					context->throwError("Incorrect parameter type");
			}
			break;
		case 2:
			size = new Size(QSize(context->argument(0).toInt32(), context->argument(1).toInt32()));
			break;
		default:
			context->throwError("Incorrect parameter count");
			break;
		}
		
		if(!size)
			return engine->undefinedValue();

		return engine->newQObject(size, QScriptEngine::ScriptOwnership);
	}
	
	QScriptValue Size::constructor(const QSize &size, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Size(size), QScriptEngine::ScriptOwnership);
	}
	
	QSize Size::parameter(QScriptContext *context)
	{
		switch(context->argumentCount())
		{
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Size *size = qobject_cast<Size*>(object))
					return size->size();
				else
					context->throwError("Incorrect parameter type");
			}
			return QSize();
		case 2:
			return QSize(context->argument(0).toInt32(),
						 context->argument(1).toInt32());
		default:
			context->throwError("Incorrect parameter count");
			return QSize();
		}
	}
	
	Size::Size()
		: QObject(),
		QScriptable()
	{
		
	}

	Size::Size(const Size &other)
		: QObject(),
		QScriptable(),
		mSize(other.size())
	{
		
	}

	Size::Size(const QSize &size)
		: QObject(),
		QScriptable(),
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
	
	QScriptValue Size::clone() const
	{
		return constructor(mSize, context(), engine());
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

	QScriptValue Size::setWidth(int width)
	{
		mSize.setWidth(width);
		
		return context()->thisObject();
	}

	QScriptValue Size::setHeight(int height)
	{
		mSize.setHeight(height);
		
		return context()->thisObject();
	}

	int Size::width() const
	{
		return mSize.width();
	}
	
	int Size::height() const
	{
		return mSize.height();
	}
}
