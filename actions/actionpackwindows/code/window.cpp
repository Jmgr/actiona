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
#include "code/point.h"
#include "code/size.h"

#include <QScriptValueIterator>
#include <QWidget>

namespace Code
{
	Window::Window()
		: mWindow(0)
	{
	}

	void Window::setWidget(QWidget *widget)
	{
		mWindow = widget;
	}

	void Window::setupConstructorParameters(const QScriptValue &parameters)
	{
		Q_ASSERT(mWindow);
		
		mWindow->setWindowTitle(tr("Window"));

		QScriptValueIterator it(parameters);

		while(it.hasNext())
		{
			it.next();

			if(it.name() == "title")
				mWindow->setWindowTitle(it.value().toString());
			else if(it.name() == "position")
				mWindow->move(it.value().property("x").toInt32(), it.value().property("y").toInt32());
			else if(it.name() == "opacity")
				mWindow->setWindowOpacity(it.value().toNumber());
			else if(it.name() == "size")
				mWindow->resize(it.value().property("width").toInt32(), it.value().property("height").toInt32());
			else if(it.name() == "fixedSize")
				mWindow->setFixedSize(it.value().property("width").toInt32(), it.value().property("height").toInt32());
			else if(it.name() == "enabled")
				mWindow->setEnabled(it.value().toBool());
			else if(it.name() == "visible")
				mWindow->setVisible(it.value().toBool());
		}
	}
	
	QScriptValue Window::setTitle(const QString &title)
	{
		Q_ASSERT(mWindow);
		
		mWindow->setWindowTitle(title);

		return context()->thisObject();
	}

	QScriptValue Window::setPosition()
	{
		Q_ASSERT(mWindow);

		mWindow->move(Point::parameter(context()));

		return context()->thisObject();
	}

	QScriptValue Window::setOpacity(float opacity)
	{
		Q_ASSERT(mWindow);

		mWindow->setWindowOpacity(opacity);

		return context()->thisObject();
	}

	QScriptValue Window::setSize()
	{
		Q_ASSERT(mWindow);

		mWindow->resize(Size::parameter(context()));

		return context()->thisObject();
	}

	QScriptValue Window::setFixedSize()
	{
		Q_ASSERT(mWindow);

		mWindow->setFixedSize(Size::parameter(context()));

		return context()->thisObject();
	}
	
	QScriptValue Window::setEnabled(bool enabled)
	{
		Q_ASSERT(mWindow);
		
		mWindow->setEnabled(enabled);
		
		return context()->thisObject();
	}

	QScriptValue Window::setVisible(bool visible)
	{
		Q_ASSERT(mWindow);
		
		mWindow->setVisible(visible);
		
		return context()->thisObject();
	}
	
	QScriptValue Window::close()
	{
		Q_ASSERT(mWindow);
		
		mWindow->close();

		return context()->thisObject();
	}	
	
	QScriptValue Window::position() const
	{
		Q_ASSERT(mWindow);
		
		return Point::constructor(mWindow->pos(), context(), engine());
	}
	
	float Window::opacity() const
	{
		Q_ASSERT(mWindow);
		
		return mWindow->windowOpacity();
	}
	
	QScriptValue Window::size() const
	{
		Q_ASSERT(mWindow);
		
		return Size::constructor(mWindow->size(), context(), engine());
	}
	
	bool Window::enabled() const
	{
		Q_ASSERT(mWindow);
		
		return mWindow->isEnabled();
	}
	
	bool Window::visible() const
	{
		Q_ASSERT(mWindow);
		
		return mWindow->isVisible();
	}
}
