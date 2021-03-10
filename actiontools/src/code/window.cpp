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

#include "actiontools/code/window.hpp"
#include "actiontools/code/rect.hpp"
#include "actiontools/code/size.hpp"
#include "actiontools/code/point.hpp"
#include "actiontools/code/processhandle.hpp"
#include "actiontools/code/codetools.hpp"
#include "backend/backend.hpp"

#include <QScriptValueIterator>

namespace Code
{
	QScriptValue Window::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Window *window = nullptr;
		
		switch(context->argumentCount())
		{
		case 0:
			window = new Window;
			break;
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(auto codeWindow = qobject_cast<Window*>(object))
					window = new Window(*codeWindow);
				else
					throwError(context, engine, QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			}
			break;
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			break;
		}
		
		if(!window)
			return engine->undefinedValue();

		return CodeClass::constructor(window, context, engine);
	}
	
	QScriptValue Window::constructor(const ActionTools::WindowHandle &windowHandle, QScriptEngine *engine)
	{
		return CodeClass::constructor(new Window(windowHandle), engine);
	}
	
	ActionTools::WindowHandle Window::parameter(QScriptContext *context, QScriptEngine *engine)
	{
		switch(context->argumentCount())
		{
		case 1:
			{
				QObject *object = context->argument(0).toQObject();
				if(auto window = qobject_cast<Window*>(object))
					return window->windowHandle();
				else
					throwError(context, engine, QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
			}
			return {};
		default:
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
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
		ProcessHandle *processHandle = nullptr;

		while(it.hasNext())
		{
			it.next();

			if(it.name() == QLatin1String("title"))
				titlePattern = it.value().toString();
			else if(it.name() == QLatin1String("className"))
				classNamePattern = it.value().toString();
			else if(it.name() == QLatin1String("titleMode"))
				titleMode = static_cast<Mode>(it.value().toInt32());
			else if(it.name() == QLatin1String("classNameMode"))
				classNameMode = static_cast<Mode>(it.value().toInt32());
			else if(it.name() == QLatin1String("titleCaseSensitive"))
				titleCaseSensitive = it.value().toBool();
			else if(it.name() == QLatin1String("classNameCaseSensitive"))
				classNameCaseSensitive = it.value().toBool();
			else if(it.name() == QLatin1String("processId"))
				processId = it.value().toInt32();
			else if(it.name() == QLatin1String("process"))
			{
				if(auto processHandleParameter = qobject_cast<ProcessHandle *>(it.value().toQObject()))
					processHandle = processHandleParameter;
				else
					throwError(context, engine, QStringLiteral("ProcessHandleError"), tr("Invalid process handle"));
			}
		}

        QList<ActionTools::WindowHandle> windowList;

        try
        {
            windowList = ActionTools::WindowHandle::windowList();
        }
        catch(const Backend::BackendError &e)
        {
            throwError(context, engine, QStringLiteral("ListWindowsError"), e.what());
            return engine->undefinedValue();
        }

		QRegExp titleRegExp(titlePattern,
							titleCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive,
							static_cast<QRegExp::PatternSyntax>(titleMode));
		QRegExp classNameRegExp(classNamePattern,
							classNameCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive,
							static_cast<QRegExp::PatternSyntax>(classNameMode));

		QList<ActionTools::WindowHandle> foundWindows;

        for(const ActionTools::WindowHandle &windowHandle: qAsConst(windowList))
		{
            try
            {
                if(!titlePattern.isNull() && !titleRegExp.exactMatch(windowHandle.title()))
                    continue;

                if(!classNamePattern.isNull() && !classNameRegExp.exactMatch(windowHandle.classname()))
                    continue;

                if(processId != -1 && windowHandle.processId() != processId)
                    continue;

                if(processHandle && windowHandle.processId() != processHandle->processId())
                    continue;
            }
            catch(const Backend::BackendError &)
            {
                continue;
            }

			foundWindows.append(windowHandle);
		}

		QScriptValue back = engine->newArray(foundWindows.count());

		for(int index = 0; index < foundWindows.count(); ++index)
			back.setProperty(index, constructor(foundWindows.at(index), engine));

		return back;
	}

	QScriptValue Window::all(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)

        QList<ActionTools::WindowHandle> windowList;

        try
        {
            windowList = ActionTools::WindowHandle::windowList();
        }
        catch(const Backend::BackendError &e)
        {
            throwError(context, engine, QStringLiteral("ListWindowsError"), e.what());
            return engine->undefinedValue();
        }

		QScriptValue back = engine->newArray(windowList.count());

		for(int index = 0; index < windowList.count(); ++index)
			back.setProperty(index, constructor(windowList.at(index), engine));

		return back;
	}

	QScriptValue Window::foreground(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)

        ActionTools::WindowHandle window;

        try
        {
            window = ActionTools::WindowHandle::foregroundWindow();
        }
        catch(const Backend::BackendError &e)
        {
            throwError(context, engine, QStringLiteral("GetForegroundWindowError"), e.what());
            return engine->undefinedValue();
        }

        return constructor(window, engine);
	}

	void Window::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassToScriptEngine<Window>(scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<Window>(&all, QStringLiteral("all"), scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<Window>(&find, QStringLiteral("find"), scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<Window>(&foreground, QStringLiteral("foreground"), scriptEngine);
	}
	
	Window::Window()
		: CodeClass()
	{
		
	}

	Window::Window(const Window &other)
		: CodeClass(),
		mWindowHandle(other.windowHandle())
	{
		
	}

	Window::Window(const ActionTools::WindowHandle &windowHandle)
		: CodeClass(),
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
		return constructor(mWindowHandle, engine());
	}

	bool Window::equals(const QScriptValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(auto otherWindow = qobject_cast<Window*>(object))
			return (otherWindow == this || otherWindow->mWindowHandle == mWindowHandle);
			
		return false;
	}

	QString Window::toString() const
	{
		return QStringLiteral("Window  {title: \"%1\", className: \"%2\"}").arg(title()).arg(className());
	}

	bool Window::isValid() const
	{
		return mWindowHandle.isValid();
	}
	
	QString Window::title() const
	{
		if(!checkValidity())
            return {};

        try
        {
            return mWindowHandle.title();
        }
        catch(const Backend::BackendError &)
        {
            return {};
        }
	}
	
	QString Window::className() const
	{
		if(!checkValidity())
            return {};

        try
        {
            return mWindowHandle.classname();
        }
        catch(const Backend::BackendError &)
        {
            return {};
        }
	}

	bool Window::isActive() const
	{
		if(!checkValidity())
			return false;

        try
        {
            return mWindowHandle.isActive();
        }
        catch(const Backend::BackendError &)
        {
            return {};
        }
	}
	
	QScriptValue Window::rect(bool useBorders) const
	{
		if(!checkValidity())
            return {};

        try
        {
            return Rect::constructor(mWindowHandle.rect(useBorders), engine());
        }
        catch(const Backend::BackendError &)
        {
            return {};
        }
	}
	
	QScriptValue Window::process() const
	{
		if(!checkValidity())
			return -1;

        try
        {
            return ProcessHandle::constructor(mWindowHandle.processId(), engine());
        }
        catch(const Backend::BackendError &)
        {
            return {};
        }
	}
	
	QScriptValue Window::close() const
	{
		if(!checkValidity())
			return thisObject();

        try
        {
            mWindowHandle.close();
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("CloseWindowError"), tr("Unable to close the window: %1").arg(e.what()));
        }

		return thisObject();
	}
	
	QScriptValue Window::killCreator() const
	{
		if(!checkValidity())
			return thisObject();

        try
        {
            mWindowHandle.killCreator();
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("KillCreatorError"), tr("Unable to kill the window creator: %1").arg(e.what()));
        }

		return thisObject();
	}
	
	QScriptValue Window::setForeground() const
	{
		if(!checkValidity())
			return thisObject();

        try
        {
            mWindowHandle.setForeground();
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("SetForegroundError"), tr("Unable to set the window foreground: %1").arg(e.what()));
        }

		return thisObject();
	}
	
	QScriptValue Window::minimize() const
	{
		if(!checkValidity())
			return thisObject();

        try
        {
            mWindowHandle.minimize();
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("MinimizeError"), tr("Unable to minimize the window: %1").arg(e.what()));
        }

		return thisObject();
	}
	
	QScriptValue Window::maximize() const
	{
		if(!checkValidity())
			return thisObject();

        try
        {
            mWindowHandle.maximize();
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("MaximizeError"), tr("Unable to maximize the window: %1").arg(e.what()));
        }

		return thisObject();
	}
	
	QScriptValue Window::move() const
	{
		if(!checkValidity())
			return thisObject();

        try
        {
            mWindowHandle.move(Point::parameter(context(), engine()));
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("MoveError"), tr("Unable to move the window: %1").arg(e.what()));
        }

		return thisObject();
	}
	
	QScriptValue Window::resize(bool useBorders) const
	{
		if(!checkValidity())
			return thisObject();

        try
        {
            mWindowHandle.resize(Size::parameter(context(), engine()), useBorders);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("ResizeError"), tr("Unable to resize the window: %1").arg(e.what()));
        }

		return thisObject();
	}

	bool Window::checkValidity() const
	{
		if(!mWindowHandle.isValid())
		{
			throwError(QStringLiteral("InvalidWindowError"), tr("Invalid window"));
			return false;
		}

		return true;
	}
}
