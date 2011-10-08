/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#include "basewindow.h"
#include "code/point.h"
#include "code/size.h"
#include "code/image.h"

#include <QScriptValueIterator>
#include <QWidget>
#include <QIcon>

namespace Code
{
	BaseWindow::BaseWindow()
		: CodeClass(),
		mWindow(0)
	{
	}

	void BaseWindow::setWidget(QWidget *widget)
	{
		mWindow = widget;
	}

	void BaseWindow::setupConstructorParameters(QScriptContext *context, QScriptEngine *engine, const QScriptValue &parameters)
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
			{
				QObject *object = it.value().toQObject();
				if(Point *codePoint = qobject_cast<Point*>(object))
					mWindow->move(codePoint->point());
				else
					throwError(context, engine, "ParameterTypeError", tr("Incorrect parameter type"));
			}
			else if(it.name() == "opacity")
				mWindow->setWindowOpacity(it.value().toNumber());
			else if(it.name() == "enabled")
				mWindow->setEnabled(it.value().toBool());
			else if(it.name() == "visible")
				mWindow->setVisible(it.value().toBool());
			else if(it.name() == "windowIcon")
			{
				if(Image *icon = qobject_cast<Image*>(it.value().toQObject()))
					mWindow->setWindowIcon(QIcon(QPixmap::fromImage(icon->image())));
				else
					throwError(context, engine, "ParameterTypeError", tr("Incorrect parameter type"));
			}
		}
	}
	
	QScriptValue BaseWindow::setTitle(const QString &title)
	{
		Q_ASSERT(mWindow);
		
		mWindow->setWindowTitle(title);

		return context()->thisObject();
	}

	QScriptValue BaseWindow::setPosition(const QScriptValue &)
	{
		Q_ASSERT(mWindow);

		mWindow->move(Point::parameter(context(), engine()));

		return context()->thisObject();
	}

	QScriptValue BaseWindow::setOpacity(float opacity)
	{
		Q_ASSERT(mWindow);

		mWindow->setWindowOpacity(opacity);

		return context()->thisObject();
	}

	QScriptValue BaseWindow::setEnabled(bool enabled)
	{
		Q_ASSERT(mWindow);
		
		mWindow->setEnabled(enabled);
		
		return context()->thisObject();
	}

	QScriptValue BaseWindow::setVisible(bool visible)
	{
		Q_ASSERT(mWindow);
		
		mWindow->setVisible(visible);
		
		return context()->thisObject();
	}

	QScriptValue BaseWindow::setWindowIcon(const QScriptValue &windowIcon)
	{
		Q_ASSERT(mWindow);

		if(windowIcon.isUndefined() || windowIcon.isNull())
		{
			mWindow->setWindowIcon(QIcon());

			return context()->thisObject();
		}

		if(Image *icon = qobject_cast<Image*>(windowIcon.toQObject()))
			mWindow->setWindowIcon(QIcon(QPixmap::fromImage(icon->image())));
		else
		{
			throwError("SetWindowIcon", tr("Invalid image"));
			return context()->thisObject();
		}

		return context()->thisObject();
	}
	
	QScriptValue BaseWindow::close()
	{
		Q_ASSERT(mWindow);
		
		mWindow->close();

		return context()->thisObject();
	}	

	QString BaseWindow::title() const
	{
		Q_ASSERT(mWindow);

		return mWindow->windowTitle();
	}

	QScriptValue BaseWindow::position() const
	{
		Q_ASSERT(mWindow);
		
		return Point::constructor(mWindow->pos(), context(), engine());
	}
	
	float BaseWindow::opacity() const
	{
		Q_ASSERT(mWindow);
		
		return mWindow->windowOpacity();
	}

	bool BaseWindow::enabled() const
	{
		Q_ASSERT(mWindow);
		
		return mWindow->isEnabled();
	}
	
	bool BaseWindow::visible() const
	{
		Q_ASSERT(mWindow);
		
		return mWindow->isVisible();
	}
}
