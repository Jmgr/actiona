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

#include "window.h"
#include "rect.h"
#include "size.h"
#include "point.h"

namespace Code
{
	QScriptValue Window::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Window *window = 0;
		
		switch(context->argumentCount())
		{
		case 0:
			window = new Window;
			break;
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Window *codeWindow = qobject_cast<Window*>(object))
					window = new Window(*codeWindow);
				else
					context->throwError("Incorrect parameter type");
			}
			break;
		default:
			context->throwError("Incorrect parameter count");
			break;
		}
		
		if(!window)
			return engine->undefinedValue();

		return engine->newQObject(window, QScriptEngine::ScriptOwnership);
	}
	
	QScriptValue Window::constructor(const ActionTools::WindowHandle &windowHandle, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)
	
		return engine->newQObject(new Window(windowHandle), QScriptEngine::ScriptOwnership);
	}
	
	ActionTools::WindowHandle Window::parameter(QScriptContext *context)
	{
		switch(context->argumentCount())
		{
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(Window *window = qobject_cast<Window*>(object))
					return window->windowHandle();
				else
					context->throwError("Incorrect parameter type");
			}
			return ActionTools::WindowHandle();
		default:
			context->throwError("Incorrect parameter count");
			return ActionTools::WindowHandle();
		}
	}
	
	QScriptValue Window::find(QScriptContext *context, QScriptEngine *engine)
	{
		return constructor(ActionTools::WindowHandle::findWindow(context->argument(0).toString()), context, engine);//TODO : add token search and regular expression search
	}
	
	Window::Window()
		: QObject(),
		QScriptable()
	{
		
	}

	Window::Window(const Window &other)
		: QObject(),
		QScriptable(),
		mWindowHandle(other.windowHandle())
	{
		
	}

	Window::Window(const ActionTools::WindowHandle &windowHandle)
		: QObject(),
		QScriptable(),
		mWindowHandle(windowHandle)
	{
		
	}
	
	Window &Window::operator=(Window other)
	{
		swap(other);
		
		return *this;
	}

	Window &Window::operator=(ActionTools::WindowHandle windowHandle)
	{
		swap(windowHandle);
		
		return *this;
	}
	
	void Window::swap(Window &other)
	{
		std::swap(mWindowHandle, other.mWindowHandle);
	}

	void Window::swap(ActionTools::WindowHandle &windowHandle)
	{
		std::swap(mWindowHandle, windowHandle);
	}
	
	const ActionTools::WindowHandle &Window::windowHandle() const
	{
		return mWindowHandle;
	}
	
	QScriptValue Window::clone() const
	{
		return constructor(mWindowHandle, context(), engine());
	}

	bool Window::equals(const QScriptValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(Window *otherWindow = qobject_cast<Window*>(object))
			return (otherWindow == this || otherWindow->mWindowHandle == mWindowHandle);
			
		return false;
	}
	
	QString Window::title() const
	{
		return mWindowHandle.title();
	}
	
	QString Window::className() const
	{
		return mWindowHandle.classname();
	}
	
	QScriptValue Window::rect() const
	{
		return Rect::constructor(mWindowHandle.rect(), context(), engine());
	}
	
	int Window::processId() const
	{
		return mWindowHandle.processId();
	}
	
	QScriptValue Window::close() const
	{
		if(!mWindowHandle.close())
			context()->throwError(tr("Unable to close the window"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::killCreator() const
	{
		if(!mWindowHandle.killCreator())
			context()->throwError(tr("Unable to kill the window creator"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::setForeground() const
	{
		if(!mWindowHandle.setForeground())
			context()->throwError(tr("Unable to set the window foreground"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::minimize() const
	{
		if(!mWindowHandle.minimize())
			context()->throwError(tr("Unable to minimize the window"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::maximize() const
	{
		if(!mWindowHandle.maximize())
			context()->throwError(tr("Unable to maximize the window"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::move() const
	{
		if(!mWindowHandle.move(Point::parameter(context())))
			context()->throwError(tr("Unable to move the window"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::resize() const
	{
		if(!mWindowHandle.resize(Size::parameter(context())))
			context()->throwError(tr("Unable to resize the window"));
		
		return context()->thisObject();
	}
}
