/*
	Actiona
	Copyright (C) 2005-2016 Jonathan Mercier-Ganady

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

	Contact : jmgr@jmgr.info
*/

#include "choosepositionpushbutton.h"

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include "nativeeventfilteringapplication.h"
#endif

#include <QStylePainter>
#include <QStyleOptionButton>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QApplication>

#ifdef Q_OS_LINUX
#include <QX11Info>
#include <X11/Xlib.h>
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#ifdef Q_OS_LINUX
#include <X11/cursorfont.h>
#include <xcb/xcb.h>
#endif
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace ActionTools
{
	ChoosePositionPushButton::ChoosePositionPushButton(QWidget *parent)
	: QPushButton(parent),
	mCrossIcon(new QPixmap(":/images/cross.png")),
	mSearching(false),
	mMainWindow(0)
#ifdef Q_OS_LINUX
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    ,mCrossCursor(XCreateFontCursor(QX11Info::display(), XC_crosshair))
#endif
#endif
#ifdef Q_OS_WIN
	,mPreviousCursor(NULL)
#endif
	{
#ifdef Q_OS_LINUX
        for(QWidget *widget: QApplication::topLevelWidgets())
		{
			if(QMainWindow *mainWindow = qobject_cast<QMainWindow*>(widget))
			{
				mMainWindow = mainWindow;
				break;
			}
		}
#endif

		setToolTip(tr("Target a position by clicking this button, moving the cursor to the desired position and releasing the mouse button."));
	}

	ChoosePositionPushButton::~ChoosePositionPushButton()
	{
		if(mSearching)
			stopMouseCapture();

#ifdef Q_OS_LINUX
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QCoreApplication::instance()->removeNativeEventFilter(this);
#else
        nativeEventFilteringApp->removeNativeEventFilter(this);
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        XFreeCursor(QX11Info::display(), mCrossCursor);
#endif
#endif

        delete mCrossIcon;
    }

	void ChoosePositionPushButton::paintEvent(QPaintEvent *event)
	{
		if(mSearching)
		{
			QPushButton::paintEvent(event);
			return;
		}

		QStylePainter painter(this);
		QStyleOptionButton option;

		initStyleOption(&option);

		painter.drawControl(QStyle::CE_PushButton, option);
		painter.drawItemPixmap(rect(), Qt::AlignCenter, *mCrossIcon);
	}

	void ChoosePositionPushButton::mousePressEvent(QMouseEvent *event)
	{
		QPushButton::mousePressEvent(event);

		mSearching = true;
		update();

#ifdef Q_OS_LINUX
        mShownWindows.clear();

        for(QWidget *widget: qApp->topLevelWidgets())
        {
            if(mMainWindow == widget)
                continue;

            if(widget->isVisible() && !widget->windowTitle().isEmpty())
            {
                mShownWindows.append(widget);

                XUnmapWindow(QX11Info::display(), widget->winId());
            }
        }

        if(mMainWindow)
            mMainWindow->hide();
#endif
#ifdef Q_OS_WIN
        for(QWidget *widget: qApp->topLevelWidgets())
			widget->setWindowOpacity(0.0f);
#endif

		QCursor newCursor(*mCrossIcon);

		emit chooseStarted();

#ifdef Q_OS_WIN
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        mPreviousCursor = SetCursor(LoadCursor(0, IDC_CROSS));
#else
        mPreviousCursor = SetCursor(newCursor.handle());
#endif
#endif
#ifdef Q_OS_LINUX
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QCoreApplication::instance()->installNativeEventFilter(this);
#else
		nativeEventFilteringApp->installNativeEventFilter(this);
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        if(XGrabPointer(QX11Info::display(), DefaultRootWindow(QX11Info::display()), True, ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
                        None, mCrossCursor, CurrentTime) != GrabSuccess)
        {
            QMessageBox::warning(this, tr("Choose a window"), tr("Unable to grab the pointer."));
            event->ignore();
        }
#else
        if(XGrabPointer(QX11Info::display(), DefaultRootWindow(QX11Info::display()), True, ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
                        None, newCursor.handle(), CurrentTime) != GrabSuccess)
        {
            QMessageBox::warning(this, tr("Choose a window"), tr("Unable to grab the pointer."));
            event->ignore();
        }
#endif
#endif
    }

#ifdef Q_OS_WIN
	void ChoosePositionPushButton::mouseReleaseEvent(QMouseEvent *event)
	{
		QPushButton::mouseReleaseEvent(event);

        emit positionChosen(event->globalPos());

        stopMouseCapture();
	}
#endif

#ifdef Q_OS_LINUX
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    bool ChoosePositionPushButton::nativeEventFilter(const QByteArray &eventType, void *message, long *)
    {
        if(eventType == "xcb_generic_event_t")
        {
            xcb_generic_event_t* event = static_cast<xcb_generic_event_t *>(message);

            switch(event->response_type)
            {
            case XCB_BUTTON_RELEASE:
                emit positionChosen(QCursor::pos());

                stopMouseCapture();

                return false;
            }

            return false;
        }

        return false;
    }
#else
	bool ChoosePositionPushButton::x11EventFilter(XEvent *event)
	{
		if(event->type == ButtonRelease)
		{
			emit positionChosen(QCursor::pos());

			stopMouseCapture();

			return true;
		}

		return false;
	}
#endif
#endif

	void ChoosePositionPushButton::stopMouseCapture()
	{
		mSearching = false;
        update();

#ifdef Q_OS_WIN
		if(mPreviousCursor)
			SetCursor(mPreviousCursor);

        for(QWidget *widget: qApp->topLevelWidgets())
			widget->setWindowOpacity(1.0f);
#endif
#ifdef Q_OS_LINUX
        XUngrabPointer(QX11Info::display(), CurrentTime);
        XFlush(QX11Info::display());

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QCoreApplication::instance()->removeNativeEventFilter(this);
#else
		nativeEventFilteringApp->removeNativeEventFilter(this);
#endif

        for(int windowIndex = 0; windowIndex < mShownWindows.size(); ++windowIndex)
        {
            QWidget *window = mShownWindows[windowIndex];

            XMapWindow(QX11Info::display(), window->winId());
        }

        if(mMainWindow)
            mMainWindow->showNormal();


#endif
	}
}
