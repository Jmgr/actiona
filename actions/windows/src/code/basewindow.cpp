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

#include "basewindow.hpp"
#include "actiontools/code/point.hpp"
#include "actiontools/code/size.hpp"
#include "actiontools/code/image.hpp"

#include <QJSValueIterator>
#include <QWidget>
#include <QIcon>

namespace Code
{
	BaseWindow::BaseWindow()
		: CodeClass()
		
	{
	}

	QString BaseWindow::title() const
	{
		Q_ASSERT(mWindow);

		return mWindow->windowTitle();
	}

    const Point *BaseWindow::position() const
	{
		Q_ASSERT(mWindow);

        auto point = Code::CodeClass::construct<Code::Point>(mWindow->pos());

        return qobject_cast<Point *>(point.toQObject());
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

	void BaseWindow::setWidget(QWidget *widget)
	{
		mWindow = widget;
	}

    void BaseWindow::setupConstructorParameters(const QJSValue &parameters)
    {
        Q_ASSERT(mWindow);

        mWindow->setWindowTitle(tr("Window"));

        QJSValueIterator it(parameters);

        while(it.hasNext())
        {
            it.next();

            if(it.name() == QLatin1String("title"))
                mWindow->setWindowTitle(it.value().toString());
            else if(it.name() == QLatin1String("position"))
            {
                QObject *object = it.value().toQObject();
                if(auto codePoint = qobject_cast<Point*>(object))
                    mWindow->move(codePoint->point());
                else
                    throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
            }
            else if(it.name() == QLatin1String("opacity"))
                mWindow->setWindowOpacity(it.value().toNumber());
            else if(it.name() == QLatin1String("enabled"))
                mWindow->setEnabled(it.value().toBool());
            else if(it.name() == QLatin1String("visible"))
                mWindow->setVisible(it.value().toBool());
            else if(it.name() == QLatin1String("windowIcon"))
            {
                if(auto icon = qobject_cast<Image*>(it.value().toQObject()))
                    mWindow->setWindowIcon(QIcon(QPixmap::fromImage(icon->image())));
                else
                    throwError(QStringLiteral("ParameterTypeError"), tr("Incorrect parameter type"));
            }
        }
    }

    BaseWindow *BaseWindow::setTitle(const QString &title)
	{
		Q_ASSERT(mWindow);
		
		mWindow->setWindowTitle(title);

        return this;
	}

    BaseWindow *BaseWindow::setPosition(const Code::Point *position)
	{
		Q_ASSERT(mWindow);

        mWindow->move(position->point());

        return this;
	}

    BaseWindow *BaseWindow::setOpacity(float opacity)
	{
		Q_ASSERT(mWindow);

		mWindow->setWindowOpacity(opacity);

        return this;
	}

    BaseWindow *BaseWindow::setEnabled(bool enabled)
	{
		Q_ASSERT(mWindow);
		
		mWindow->setEnabled(enabled);
		
        return this;
	}

    BaseWindow *BaseWindow::setVisible(bool visible)
	{
		Q_ASSERT(mWindow);
		
		mWindow->setVisible(visible);
		
        return this;
	}

    BaseWindow *BaseWindow::setWindowIcon(const QJSValue &windowIcon)
	{
		Q_ASSERT(mWindow);

		if(windowIcon.isUndefined() || windowIcon.isNull())
		{
			mWindow->setWindowIcon(QIcon());

            return this;
		}

		if(auto icon = qobject_cast<Image*>(windowIcon.toQObject()))
			mWindow->setWindowIcon(QIcon(QPixmap::fromImage(icon->image())));
		else
		{
			throwError(QStringLiteral("SetWindowIcon"), tr("Invalid image"));
            return this;
		}

        return this;
	}
	
    BaseWindow *BaseWindow::close()
	{
		Q_ASSERT(mWindow);
		
		mWindow->close();

        return this;
    }

    bool BaseWindow::equals(const QJSValue &other) const
    {
        if(other.isUndefined() || other.isNull())
            return false;

        QObject *object = other.toQObject();
        if(auto otherBaseWindow = qobject_cast<BaseWindow*>(object))
            return (otherBaseWindow == this || otherBaseWindow->mWindow == mWindow);

        return false;
    }
}
