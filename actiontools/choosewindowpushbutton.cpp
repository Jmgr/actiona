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

	Contact : jmgr@jmgr.info
*/
//Using parts of the Window Finder : http://www.codeproject.com/KB/dialog/windowfinder.aspx
//By Lim Bio Liong

#include "choosewindowpushbutton.h"

#include <QStylePainter>
#include <QStyleOptionButton>
#include <QDebug>
#include <QMessageBox>
#include <QMainWindow>
#include <QApplication>

#ifdef Q_OS_UNIX
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <xcb/xcb.h>
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#ifdef Q_OS_WIN
//Some functions to replace MFC stuff
inline bool PtInRect(const POINT &point, const RECT &rect)
{
	return (point.x >= rect.left && point.x <= rect.right
		&& point.y >= rect.bottom && point.y <= rect.top);
}
inline POINT RectTopLeft(const RECT &rect)
{
	POINT back;
	back.x = rect.left;
	back.y = rect.top;
	return back;
}
inline POINT RectBottomRight(const RECT &rect)
{
	POINT back;
	back.x = rect.right;
	back.y = rect.bottom;
	return back;
}
#endif

namespace ActionTools
{
	ChooseWindowPushButton::ChooseWindowPushButton(QWidget *parent)
		: QPushButton(parent),
        mCrossIcon(new QPixmap(QStringLiteral(":/images/cross.png")))
#ifdef Q_OS_UNIX
        ,mCrossCursor(XCreateFontCursor(QX11Info::display(), XC_crosshair))
#endif
#ifdef Q_OS_WIN
		,mPreviousCursor(NULL)
		,mRectanglePen(CreatePen(PS_SOLID, 3, RGB(255, 0, 0)))
#endif
	{
#ifdef Q_OS_UNIX
        for(QWidget *widget: QApplication::topLevelWidgets())
		{
			if(auto mainWindow = qobject_cast<QMainWindow*>(widget))
			{
				mMainWindow = mainWindow;
				break;
			}
		}
#endif

		setToolTip(tr("Target a window by clicking this button, moving the cursor to the wanted window and releasing the mouse button."));
	}

	ChooseWindowPushButton::~ChooseWindowPushButton()
	{
		if(mSearching)
			stopMouseCapture();

        QCoreApplication::instance()->removeNativeEventFilter(this);

#ifdef Q_OS_WIN
		DeleteObject(mRectanglePen);
#endif
#ifdef Q_OS_UNIX
        XFreeCursor(QX11Info::display(), mCrossCursor);
#endif

		delete mCrossIcon;
	}

	void ChooseWindowPushButton::paintEvent(QPaintEvent *event)
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

	void ChooseWindowPushButton::mousePressEvent(QMouseEvent *event)
	{
		QPushButton::mousePressEvent(event);

#ifdef Q_OS_UNIX
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

        QCoreApplication::instance()->installNativeEventFilter(this);

		startMouseCapture();
	}

#ifdef Q_OS_WIN
	void ChooseWindowPushButton::mouseReleaseEvent(QMouseEvent *event)
	{
		QPushButton::mouseReleaseEvent(event);

		stopMouseCapture();
	}

	void ChooseWindowPushButton::foundWindow(const WindowHandle &handle)
	{
		if(!isWindowValid(handle))
			return;

		if(mLastFoundWindow)
			refreshWindow(mLastFoundWindow);

		highlightWindow(handle);

		mLastFoundWindow = handle;

		emit foundValidWindow(handle);
	}
#endif

	bool ChooseWindowPushButton::isWindowValid(const WindowHandle &handle) const
	{
		if(!handle.isValid())
			return false;

		if(handle == mLastFoundWindow)
			return false;

#ifdef Q_OS_WIN
        if(!IsWindow(reinterpret_cast<HWND>(handle.value())))
			return false;
#endif

        for(QWidget *widget: QApplication::allWidgets())
		{
			if(widget->winId() == handle.value())
				return false;
		}

		return true;
	}

#ifdef Q_OS_WIN
	void ChooseWindowPushButton::refreshWindow(const WindowHandle &handle)
	{
        InvalidateRect(reinterpret_cast<HWND>(handle.value()), NULL, TRUE);
        UpdateWindow(reinterpret_cast<HWND>(handle.value()));
        RedrawWindow(reinterpret_cast<HWND>(handle.value()), NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW | RDW_UPDATENOW | RDW_ALLCHILDREN);

        HWND hParent = GetParent(reinterpret_cast<HWND>(handle.value()));
		if(hParent)
		{
			RECT rc;
            GetWindowRect(reinterpret_cast<HWND>(handle.value()), &rc);
			POINT ptTopLeft		= RectTopLeft(rc);
			POINT ptBottomRight	= RectBottomRight(rc);
			ScreenToClient(hParent, &ptTopLeft);
			ScreenToClient(hParent, &ptBottomRight);
			rc.top				= ptTopLeft.y;
			rc.left				= ptTopLeft.x;
			rc.bottom			= ptBottomRight.y;
			rc.right			= ptBottomRight.x;

			InvalidateRect(hParent, &rc, TRUE);
			UpdateWindow(hParent);
			RedrawWindow(hParent, &rc, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
		}
	}

	void ChooseWindowPushButton::highlightWindow(const WindowHandle &handle)
	{
		HDC		hWindowDC = NULL;  // The DC of the found window.
		HGDIOBJ	hPrevPen = NULL;   // Handle of the existing pen in the DC of the found window.
		HGDIOBJ	hPrevBrush = NULL; // Handle of the existing brush in the DC of the found window.
		RECT	rect;              // Rectangle area of the found window.

        GetWindowRect(reinterpret_cast<HWND>(handle.value()), &rect);
        hWindowDC = GetWindowDC(reinterpret_cast<HWND>(handle.value()));

		if(hWindowDC)
		{
			hPrevPen = SelectObject(hWindowDC, mRectanglePen);
			hPrevBrush = SelectObject(hWindowDC, GetStockObject(HOLLOW_BRUSH));
			// Draw a rectangle in the DC covering the entire window area of the found window.
			Rectangle(hWindowDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top);

			SelectObject(hWindowDC, hPrevPen);
			SelectObject(hWindowDC, hPrevBrush);

            ReleaseDC(reinterpret_cast<HWND>(handle.value()), hWindowDC);
		}
	}
#endif

	void ChooseWindowPushButton::startMouseCapture()
	{
		mSearching = true;
		update();
		mLastFoundWindow.setInvalid();

		QCursor newCursor(*mCrossIcon);

#ifdef Q_OS_WIN
        mPreviousCursor = SetCursor(LoadCursor(0, IDC_CROSS));
#endif
#ifdef Q_OS_UNIX
		if(XGrabPointer(QX11Info::display(), DefaultRootWindow(QX11Info::display()), True, ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
            None, mCrossCursor, CurrentTime) != GrabSuccess)
		{
			QMessageBox::warning(this, tr("Choose a window"), tr("Unable to grab the pointer."));
			mSearching = false;
		}
#endif
	}

	void ChooseWindowPushButton::stopMouseCapture()
	{
		if(!mSearching)
			return;

		mSearching = false;
		update();

	#ifdef Q_OS_WIN
		if(mPreviousCursor)
			SetCursor(mPreviousCursor);

		if(mLastFoundWindow)
			refreshWindow(mLastFoundWindow);

        for(QWidget *widget: qApp->topLevelWidgets())
			widget->setWindowOpacity(1.0f);
	#endif
    #ifdef Q_OS_UNIX
		XUngrabPointer(QX11Info::display(), CurrentTime);
        XFlush(QX11Info::display());

        for(auto shownWindow: mShownWindows)
        {
            XMapWindow(QX11Info::display(), shownWindow->winId());
        }

		if(mMainWindow)
			mMainWindow->showNormal();
	#endif

        QCoreApplication::instance()->removeNativeEventFilter(this);

        emit searchEnded(mLastFoundWindow);
    }

#ifdef Q_OS_UNIX
	WId ChooseWindowPushButton::windowAtPointer() const
	{
		Window window = DefaultRootWindow(QX11Info::display());
		Window back = None;

		while(window)
		{
			Window root, child;
			int x, y, rx, ry;
			unsigned int mask;

			back = window;

			XQueryPointer(QX11Info::display(), window, &root, &child, &rx, &ry, &x, &y, &mask);

			window = child;
		}

		return back;
	}

    bool ChooseWindowPushButton::nativeEventFilter(const QByteArray &eventType, void *message, long *)
    {
        if(eventType == "xcb_generic_event_t")
        {
            auto* event = static_cast<xcb_generic_event_t *>(message);

            switch(event->response_type)
            {
            case XCB_BUTTON_RELEASE:
            {
                Window window = windowAtPointer();
                if(window == None)
                    return true;

                if(isWindowValid(window))
                    mLastFoundWindow = window;

                stopMouseCapture();

                return false;
            }
            }

            return false;
        }

        return false;
    }
#endif

#ifdef Q_OS_WIN
    bool ChooseWindowPushButton::nativeEventFilter(const QByteArray &, void *message, long *)
    {
        MSG *msg = static_cast<MSG*>(message);

        if(!msg || !mSearching)
            return false;

        switch(msg->message)
        {
        case WM_LBUTTONUP:
            stopMouseCapture();
            break;

        case WM_MOUSEMOVE:
            {
                POINT screenpoint;
                GetCursorPos(&screenpoint);

                HWND window = NULL;
                HWND firstWindow = WindowFromPoint(screenpoint);
                if(firstWindow && IsWindow(firstWindow))
                {
                    // try to go to child window
                    if(!GetParent(firstWindow))
                    {
                        POINT pt;
                        pt.x = screenpoint.x;
                        pt.y = screenpoint.y;
                        ScreenToClient(firstWindow, &pt);
                        HWND childWindow = ChildWindowFromPoint(firstWindow, pt);
                        if(firstWindow != childWindow)
                        {
                            RECT rc;
                            GetWindowRect(childWindow, &rc);
                            if(PtInRect(screenpoint, rc))
                                // it may seem strange to check this condition after above lines
                                // but try to comment it and work with MSDN main window,
                                // you will hardly "find" it
                                firstWindow	= childWindow;
                        }
                    }

                    // find the best child
                    if(GetParent(firstWindow))
                    {
                        RECT rcFirst;
                        GetWindowRect(firstWindow, &rcFirst);

                        // find next/prev windows in the Z-order
                        bool bBestFound = false;
                        HWND hOther = firstWindow;
                        do
                        {
                            hOther	= GetNextWindow(hOther, GW_HWNDPREV);
                            if(!hOther)
                                break;
                            RECT rcOther;
                            GetWindowRect(hOther, &rcOther);
                            if(PtInRect(screenpoint, rcOther) &&
                                PtInRect(RectTopLeft(rcOther), rcFirst) &&
                                PtInRect(RectBottomRight(rcOther), rcFirst))
                            {
                                firstWindow = hOther;
                                bBestFound = true;
                            }
                        }
                        while(!bBestFound);

                        if(!bBestFound)
                        {
                            hOther = firstWindow;
                            do
                            {
                                hOther = GetNextWindow(hOther, GW_HWNDNEXT);
                                if (!hOther) break;
                                RECT rcOther;
                                GetWindowRect(hOther, &rcOther);
                                if(PtInRect(screenpoint, rcOther) &&
                                    PtInRect(RectTopLeft(rcOther), rcFirst) &&
                                    PtInRect(RectBottomRight(rcOther), rcFirst))
                                {
                                    firstWindow	= hOther;
                                    bBestFound = true;
                                }
                            }
                            while(!bBestFound);
                        }
                    }

                    window = firstWindow;
                }
                else
                    break;

                foundWindow(WindowHandle(reinterpret_cast<WId>(window)));
            }
            break;
        }

        return false;
    }
#endif
}
