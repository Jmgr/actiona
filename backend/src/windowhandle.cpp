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

#include "backend/windowhandle.hpp"
#include "backend/windowing.hpp"

#include <memory>

namespace Backend
{
    const Windowing &windowing()
    {
        return Instance::windowing();
    }

	QString WindowHandle::title() const
	{
        return windowing().title(mValue);
	}

    // TODO: check that all callsites catch exceptions
	QString WindowHandle::classname() const
	{
        return windowing().classname(mValue);
	}

	QRect WindowHandle::rect(bool useBorders) const
	{
        return windowing().rect(mValue, useBorders);
	}

	int WindowHandle::processId() const
	{
        return windowing().processId(mValue);
	}

    void WindowHandle::close() const
	{
        windowing().close(mValue);
	}

    void WindowHandle::killCreator() const
	{
        windowing().killCreator(mValue);
	}

    void WindowHandle::setForeground() const
	{
        windowing().setForegroundWindow(mValue);
	}

    void WindowHandle::minimize() const
	{
        windowing().minimize(mValue);
	}

    void WindowHandle::maximize() const
	{
        windowing().maximize(mValue);
	}

    void WindowHandle::move(QPoint position) const
	{
        windowing().move(mValue, position);
	}

    void WindowHandle::resize(QSize size, bool useBorders) const
	{
        windowing().resize(mValue, size, useBorders);
	}

	bool WindowHandle::isActive() const
	{
        return windowing().isActive(mValue);
	}

	WindowHandle WindowHandle::foregroundWindow()
	{
        return windowing().foregroundWindow();
	}

	QList<WindowHandle> WindowHandle::windowList()
	{
        const auto windows = windowing().windowList();
        QList<WindowHandle> res;

        res.reserve(windows.size());
        for(const auto &window: windows)
            res.append(window);

        return res;
    }

    QStringList WindowHandle::windowTitles()
    {
        QStringList windowTitles;

        const auto windowHandles = windowList();
        for(const WindowHandle &windowHandle: windowHandles)
        {
            QString title = windowHandle.title();
            if(title.isEmpty())
                continue;

            windowTitles << title;
        }

        windowTitles.sort();

        return windowTitles;
    }

	WindowHandle WindowHandle::findWindow(const QString &title)
	{
        const auto windowHandles = windowList();
        for(const WindowHandle &windowHandle: windowHandles)
		{
			if(windowHandle.title() == title)
				return windowHandle;
		}

		return {};
	}

	WindowHandle WindowHandle::findWindow(const QRegExp &regExp)
	{
		if(!regExp.isValid())
			return WindowHandle();

        const auto windowHandles = windowList();
        for(const WindowHandle &windowHandle: windowHandles)
		{
			if(regExp.exactMatch(windowHandle.title()))
				return windowHandle;
		}

		return {};
	}

	QList<WindowHandle> WindowHandle::findWindows(const QString &title)
	{
		QList<WindowHandle> back;

        const auto windowHandles = windowList();
        for(const WindowHandle &windowHandle: windowHandles)
		{
			if(windowHandle.title() == title)
				back.append(windowHandle);
		}

		return back;
	}

	QList<WindowHandle> WindowHandle::findWindows(const QRegExp &regExp)
	{
		QList<WindowHandle> back;

        const auto windowHandles = windowList();
        for(const WindowHandle &windowHandle: windowHandles)
		{
			if(regExp.exactMatch(windowHandle.title()))
				back.append(windowHandle);
		}

		return back;
	}
}
