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
#include "actiontools/scriptengine.hpp"

#include <QJSValueIterator>
#include <QDebug>

namespace Code
{
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
	
	QJSValue Window::clone() const
	{
        return CodeClass::clone<Window>();
	}

	bool Window::equals(const QJSValue &other) const
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

		return mWindowHandle.title();
	}
	
	QString Window::className() const
	{
        if(!checkValidity())
            return {};

		return mWindowHandle.classname();
	}

	bool Window::isActive() const
	{
		if(!checkValidity())
			return false;

		return mWindowHandle.isActive();
	}
	
	QJSValue Window::rect(bool useBorders) const
	{
        if(!checkValidity())
            return {};

        return CodeClass::construct<Rect>(mWindowHandle.rect(useBorders));
    }
	
	QJSValue Window::process() const
	{
		if(!checkValidity())
            return {};

        return CodeClass::construct<ProcessHandle>(mWindowHandle.processId());
	}
	
    Window *Window::close()
	{
		if(!checkValidity())
            return this;

		if(!mWindowHandle.close())
			throwError(QStringLiteral("CloseWindowError"), tr("Unable to close the window"));
		
        return this;
	}
	
    Window *Window::killCreator()
	{
		if(!checkValidity())
            return this;

		if(!mWindowHandle.killCreator())
			throwError(QStringLiteral("KillCreatorError"), tr("Unable to kill the window creator"));
		
        return this;
	}
	
    Window *Window::setForeground()
	{
		if(!checkValidity())
            return this;

		if(!mWindowHandle.setForeground())
			throwError(QStringLiteral("SetForegroundError"), tr("Unable to set the window foreground"));
		
        return this;
	}
	
    Window *Window::minimize()
	{
		if(!checkValidity())
            return this;

		if(!mWindowHandle.minimize())
			throwError(QStringLiteral("MinimizeError"), tr("Unable to minimize the window"));
		
        return this;
	}
	
    Window *Window::maximize()
	{
		if(!checkValidity())
            return this;

		if(!mWindowHandle.maximize())
			throwError(QStringLiteral("MaximizeError"), tr("Unable to maximize the window"));
		
        return this;
	}
	
    Window *Window::move(const Point *point)
	{
		if(!checkValidity())
            return this;

        if(!mWindowHandle.move(point->point()))
			throwError(QStringLiteral("MoveError"), tr("Unable to move the window"));
		
        return this;
	}
	
    Window *Window::resize(const Size *size, bool useBorders)
	{
		if(!checkValidity())
            return this;

        if(!mWindowHandle.resize(size->size(), useBorders))
			throwError(QStringLiteral("ResizeError"), tr("Unable to resize the window"));
		
        return this;
	}

    void Window::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Window*>("const Window *");

        CodeClass::registerClassWithStaticFunctions<Window, StaticWindow>(
            QStringLiteral("Window"),
            {QStringLiteral("all"), QStringLiteral("find"), QStringLiteral("foreground")},
            scriptEngine
        );
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

    QRegularExpression windowModeToRegularExpression(Window::Mode mode, const QString &pattern, bool caseSensitive)
    {
        QString localPattern = pattern;

        QRegularExpression result;
        switch(mode)
        {
        case Window::RegExp:
            result = QRegularExpression(localPattern, caseSensitive? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
            break;
        case Window::Wildcard:
            result = QRegularExpression::fromWildcard(localPattern, caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
            result.setPatternOptions(QRegularExpression::DotMatchesEverythingOption);
            break;
        case Window::WildcardUnix:
            result = QRegularExpression::fromWildcard(localPattern, caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
            break;
        case Window::FixedString:
            localPattern = QRegularExpression::escape(localPattern);
            result.setPattern(localPattern);
            result.setPatternOptions(caseSensitive ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
            break;
        }
        return result;
    }

    QJSValue StaticWindow::find(const QJSValue &parameters)
    {
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return {};
        }

        auto titlePattern = parameters.property(QStringLiteral("title"));
        auto classNamePattern = parameters.property(QStringLiteral("className"));
        QString titlePatternString = titlePattern.toString();
        QString classNamePatternString = classNamePattern.toString();
        Window::Mode titleMode = Window::WildcardUnix;
        if(parameters.hasProperty(QStringLiteral("titleMode")))
            titleMode = static_cast<Window::Mode>(parameters.property(QStringLiteral("titleMode")).toInt());
        Window::Mode classNameMode = Window::WildcardUnix;
        if(parameters.hasProperty(QStringLiteral("classNameMode")))
            classNameMode = static_cast<Window::Mode>(parameters.property(QStringLiteral("classNameMode")).toInt());
        bool titleCaseSensitive = true;
        if(parameters.hasProperty(QStringLiteral("titleCaseSensitive")))
            titleCaseSensitive = parameters.property(QStringLiteral("titleCaseSensitive")).toBool();
        bool classNameCaseSensitive = true;
        if(parameters.hasProperty(QStringLiteral("classNameCaseSensitive")))
            classNameCaseSensitive = parameters.property(QStringLiteral("classNameCaseSensitive")).toBool();
        int processId = -1;
        if(parameters.hasProperty(QStringLiteral("processId")))
            processId = parameters.property(QStringLiteral("processId")).toInt();
        ProcessHandle *processHandle = nullptr;
        if(parameters.hasProperty(QStringLiteral("process")))
        {
            if(auto processHandleParameter = qobject_cast<ProcessHandle *>(parameters.property(QStringLiteral("process")).toQObject()))
                processHandle = processHandleParameter;
            else
                throwError(QStringLiteral("ProcessHandleError"), tr("Invalid process handle"));
        }

        QList<ActionTools::WindowHandle> windowList = ActionTools::WindowHandle::windowList();

        QRegularExpression titleRegExp = windowModeToRegularExpression(titleMode, titlePatternString, titleCaseSensitive);
        QRegularExpression classNameRegExp = windowModeToRegularExpression(classNameMode, classNamePatternString, classNameCaseSensitive);
        QList<ActionTools::WindowHandle> foundWindows;

        for(const ActionTools::WindowHandle &windowHandle: qAsConst(windowList))
        {
            if(!titlePattern.isUndefined() && !titleRegExp.match(windowHandle.title()).hasMatch())
                    continue;

            if(!classNamePattern.isUndefined() && !classNameRegExp.match(windowHandle.classname()).hasMatch())
                    continue;

            if(processId != -1 && windowHandle.processId() != processId)
                    continue;

            if(processHandle && windowHandle.processId() != processHandle->processId())
                    continue;

            foundWindows.append(windowHandle);
        }

        QJSValue back = ActionTools::ScriptEngine::current()->newArray(foundWindows.count());

        for(int index = 0; index < foundWindows.count(); ++index)
            back.setProperty(index, CodeClass::construct<Window>(foundWindows.at(index)));

        return back;
    }

    QJSValue StaticWindow::all()
    {
        QList<ActionTools::WindowHandle> windowList = ActionTools::WindowHandle::windowList();

        QJSValue back = ActionTools::ScriptEngine::current()->newArray(windowList.count());

        for(int index = 0; index < windowList.count(); ++index)
            back.setProperty(index, CodeClass::construct<Window>(windowList.at(index)));

        return back;
    }

    QJSValue StaticWindow::foreground()
    {
        return CodeClass::construct<Window>(ActionTools::WindowHandle::foregroundWindow());
    }
}
