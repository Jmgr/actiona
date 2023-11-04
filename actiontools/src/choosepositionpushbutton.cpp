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

#include "actiontools/choosepositionpushbutton.hpp"

#include <QStylePainter>
#include <QStyleOptionButton>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMainWindow>
#include <QTimer>
#include <QDebug>
#include <QApplication>

#ifdef Q_OS_UNIX
#include "actiontools/x11info.hpp"
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <xcb/xcb.h>
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace ActionTools
{
	ChoosePositionPushButton::ChoosePositionPushButton(QWidget *parent)
	: QPushButton(parent),
    mCrossIcon(new QPixmap(QStringLiteral(":/images/cross.png")))
#ifdef Q_OS_UNIX
    ,mCrossCursor(XCreateFontCursor(X11Info::display(), XC_crosshair))
#endif
#ifdef Q_OS_WIN
	,mPreviousCursor(NULL)
#endif
	{
#ifdef Q_OS_UNIX
        const auto widgets = QApplication::topLevelWidgets();
        for(QWidget *widget: widgets)
		{
			if(auto mainWindow = qobject_cast<QMainWindow*>(widget))
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

#ifdef Q_OS_UNIX
        QCoreApplication::instance()->removeNativeEventFilter(this);
        XFreeCursor(X11Info::display(), mCrossCursor);
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

#ifdef Q_OS_UNIX
        mShownWindows.clear();

        const auto widgets = qApp->topLevelWidgets();
        for(QWidget *widget: widgets)
        {
            if(mMainWindow == widget)
                continue;

            if(widget->isVisible() && !widget->windowTitle().isEmpty())
            {
                mShownWindows.append(widget);

                XUnmapWindow(X11Info::display(), widget->winId());
            }
        }

        if(mMainWindow)
            mMainWindow->hide();
#endif
#ifdef Q_OS_WIN
        for(QWidget *widget: qApp->topLevelWidgets())
			widget->setWindowOpacity(0.0f);
#endif

		emit chooseStarted();

#ifdef Q_OS_WIN
        mPreviousCursor = SetCursor(LoadCursor(0, IDC_CROSS));
#endif
#ifdef Q_OS_UNIX
        QCoreApplication::instance()->installNativeEventFilter(this);

        if(XGrabPointer(X11Info::display(), DefaultRootWindow(X11Info::display()), True, ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
                        None, mCrossCursor, CurrentTime) != GrabSuccess)
        {
            QMessageBox::warning(this, tr("Choose a window"), tr("Unable to grab the pointer."));
            event->ignore();
        }
#endif
    }

#ifdef Q_OS_WIN
	void ChoosePositionPushButton::mouseReleaseEvent(QMouseEvent *event)
	{
		QPushButton::mouseReleaseEvent(event);

        emit positionChosen(event->globalPosition().toPoint());

        stopMouseCapture();
	}
#endif

#ifdef Q_OS_UNIX
    bool ChoosePositionPushButton::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *)
    {
        if(eventType == "xcb_generic_event_t")
        {
            auto* event = static_cast<xcb_generic_event_t *>(message);

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
#ifdef Q_OS_UNIX
        XUngrabPointer(X11Info::display(), CurrentTime);
        XFlush(X11Info::display());

        QCoreApplication::instance()->removeNativeEventFilter(this);

        for(auto shownWindow: qAsConst(mShownWindows))
        {
            XMapWindow(X11Info::display(), shownWindow->winId());
        }

        if(mMainWindow)
            mMainWindow->showNormal();


#endif
	}
}
