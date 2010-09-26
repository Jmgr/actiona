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

		QScriptValueIterator it(parameters);

		while(it.hasNext())
		{
			it.next();

			if(it.name() == "position")
				mWindow->move(it.value().property("x").toInt32(), it.value().property("y").toInt32());
			else if(it.name() == "opacity")
				mWindow->setWindowOpacity(it.value().toNumber());
			else if(it.name() == "size")
				mWindow->resize(it.value().property("width").toInt32(), it.value().property("height").toInt32());
			else if(it.name() == "fixedSize")
				mWindow->setFixedSize(it.value().property("width").toInt32(), it.value().property("height").toInt32());
		}
	}

	QScriptValue Window::setPosition(int x, int y)
	{
		Q_ASSERT(mWindow);

		mWindow->move(x, y);

		return context()->thisObject();
	}

	QScriptValue Window::setOpacity(float opacity)
	{
		Q_ASSERT(mWindow);

		mWindow->setWindowOpacity(opacity);

		return context()->thisObject();
	}

	QScriptValue Window::setSize(int width, int height)
	{
		Q_ASSERT(mWindow);

		mWindow->resize(width, height);

		return context()->thisObject();
	}

	QScriptValue Window::setFixedSize(int width, int height)
	{
		Q_ASSERT(mWindow);

		mWindow->setFixedSize(width, height);

		return context()->thisObject();
	}
}
