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
#include "process.h"

#include <QScriptValueIterator>

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
		QScriptValueIterator it(context->argument(0));
		QString titlePattern;
		QString classNamePattern;
		Mode titleMode = WildcardUnix;
		Mode classNameMode = WildcardUnix;
		bool titleCaseSensitive = true;
		bool classNameCaseSensitive = true;
		int processId = -1;

		while(it.hasNext())
		{
			it.next();

			if(it.name() == "title")
				titlePattern = it.value().toString();
			else if(it.name() == "className")
				classNamePattern = it.value().toString();
			else if(it.name() == "titleMode")
				titleMode = static_cast<Mode>(it.value().toInt32());
			else if(it.name() == "classNameMode")
				classNameMode = static_cast<Mode>(it.value().toInt32());
			else if(it.name() == "titleCaseSensitive")
				titleCaseSensitive = it.value().toBool();
			else if(it.name() == "classNameCaseSensitive")
				classNameCaseSensitive = it.value().toBool();
			else if(it.name() == "processId")
				processId = it.value().toInt32();
		}

		QList<ActionTools::WindowHandle> windowList = ActionTools::WindowHandle::windowList();

		QRegExp titleRegExp(titlePattern,
							titleCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive,
							static_cast<QRegExp::PatternSyntax>(titleMode));
		QRegExp classNameRegExp(classNamePattern,
							classNameCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive,
							static_cast<QRegExp::PatternSyntax>(classNameMode));

		QList<ActionTools::WindowHandle> foundWindows;

		foreach(const ActionTools::WindowHandle &windowHandle, windowList)
		{
			if(!titlePattern.isNull() && !titleRegExp.exactMatch(windowHandle.title()))
				continue;

			if(!classNamePattern.isNull() && !classNameRegExp.exactMatch(windowHandle.classname()))
				continue;

			if(processId != -1 && windowHandle.processId() != processId)
				continue;

			foundWindows.append(windowHandle);
		}

		QScriptValue back = engine->newArray(foundWindows.count());

		for(int index = 0; index < foundWindows.count(); ++index)
			back.setProperty(index, constructor(foundWindows.at(index), context, engine));

		return back;
	}

	QScriptValue Window::all(QScriptContext *context, QScriptEngine *engine)
	{
		QList<ActionTools::WindowHandle> windowList = ActionTools::WindowHandle::windowList();

		QScriptValue back = engine->newArray(windowList.count());

		for(int index = 0; index < windowList.count(); ++index)
			back.setProperty(index, constructor(windowList.at(index), context, engine));

		return back;
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

	QString Window::toString() const
	{
		return QString("Window [title: %1][className: %2]").arg(title()).arg(className());
	}

	bool Window::isValid() const
	{
		return mWindowHandle.isValid();
	}
	
	QString Window::title() const
	{
		if(!checkValidity())
			return QString();

		return mWindowHandle.title();
	}
	
	QString Window::className() const
	{
		if(!checkValidity())
			return QString();

		return mWindowHandle.classname();
	}
	
	QScriptValue Window::rect() const
	{
		if(!checkValidity())
			return QScriptValue();

		return Rect::constructor(mWindowHandle.rect(), context(), engine());
	}
	
	QScriptValue Window::processId() const
	{
		if(!checkValidity())
			return -1;

		return Process::constructor(mWindowHandle.processId(), context(), engine());
	}
	
	QScriptValue Window::close() const
	{
		if(!checkValidity())
			return context()->thisObject();

		if(!mWindowHandle.close())
			context()->throwError(tr("Unable to close the window"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::killCreator() const
	{
		if(!checkValidity())
			return context()->thisObject();

		if(!mWindowHandle.killCreator())
			context()->throwError(tr("Unable to kill the window creator"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::setForeground() const
	{
		if(!checkValidity())
			return context()->thisObject();

		if(!mWindowHandle.setForeground())
			context()->throwError(tr("Unable to set the window foreground"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::minimize() const
	{
		if(!checkValidity())
			return context()->thisObject();

		if(!mWindowHandle.minimize())
			context()->throwError(tr("Unable to minimize the window"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::maximize() const
	{
		if(!checkValidity())
			return context()->thisObject();

		if(!mWindowHandle.maximize())
			context()->throwError(tr("Unable to maximize the window"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::move() const
	{
		if(!checkValidity())
			return context()->thisObject();

		if(!mWindowHandle.move(Point::parameter(context())))
			context()->throwError(tr("Unable to move the window"));
		
		return context()->thisObject();
	}
	
	QScriptValue Window::resize() const
	{
		if(!checkValidity())
			return context()->thisObject();

		if(!mWindowHandle.resize(Size::parameter(context())))
			context()->throwError(tr("Unable to resize the window"));
		
		return context()->thisObject();
	}

	bool Window::checkValidity() const
	{
		if(!mWindowHandle.isValid())
		{
			context()->throwError(tr("Invalid window"));
			return false;
		}

		return true;
	}
}
