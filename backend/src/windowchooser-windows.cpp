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

#include "backend/windowchooser-windows.hpp"

#include <QApplication>
#include <QWidget>
#include <QDebug>

#include <Windows.h>

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

namespace Backend
{
    WindowChooserWindows::WindowChooserWindows(QObject *parent):
        WindowChooser(parent),
        mRectanglePen(CreatePen(PS_SOLID, 3, RGB(255, 0, 0)))
	{
	}

    WindowChooserWindows::~WindowChooserWindows()
	{
        stopMouseCapture();

        DeleteObject(mRectanglePen);
	}

    void WindowChooserWindows::choose()
    {
        QCoreApplication::instance()->installNativeEventFilter(this);

        mSearching = true;
        mPreviousCursor = SetCursor(LoadCursor(0, IDC_CROSS));
    }

    bool isWindowValid(WId handle, WId lastFoundWindow)
    {
        if(!handle)
            return false;

        if(handle == lastFoundWindow)
            return false;

        if(!IsWindow(reinterpret_cast<HWND>(handle)))
            return false;

        const auto widgets = QApplication::allWidgets();
        for(QWidget *widget: widgets)
        {
            if(widget->winId() == handle)
                return false;
        }

        return true;
    }

    void refreshWindow(WId wid)
    {
        auto hwnd = reinterpret_cast<HWND>(wid);

        InvalidateRect(hwnd, nullptr, true);
        UpdateWindow(hwnd);
        RedrawWindow(hwnd, nullptr, nullptr, RDW_FRAME | RDW_INVALIDATE | RDW_ERASENOW | RDW_UPDATENOW | RDW_ALLCHILDREN);

        auto parent = GetParent(hwnd);
        if(!parent)
            return;

        RECT rc;
        GetWindowRect(hwnd, &rc);
        POINT ptTopLeft = RectTopLeft(rc);
        POINT ptBottomRight	= RectBottomRight(rc);
        ScreenToClient(parent, &ptTopLeft);
        ScreenToClient(parent, &ptBottomRight);
        rc.top = ptTopLeft.y;
        rc.left	= ptTopLeft.x;
        rc.bottom = ptBottomRight.y;
        rc.right = ptBottomRight.x;

        InvalidateRect(parent, &rc, true);
        UpdateWindow(parent);
        RedrawWindow(parent, &rc, nullptr, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
    }

    void highlightWindow(WId wid, HPEN rectanglePen)
    {
        auto hwnd = reinterpret_cast<HWND>(wid);

        RECT rect;
        GetWindowRect(hwnd, &rect);
        auto windowDC = GetWindowDC(hwnd);
        if(!windowDC)
            return;

        auto prevPen = SelectObject(windowDC, rectanglePen);
        auto prevBrush = SelectObject(windowDC, GetStockObject(HOLLOW_BRUSH));

        Rectangle(windowDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top);

        SelectObject(windowDC, prevPen);
        SelectObject(windowDC, prevBrush);

        ReleaseDC(hwnd, windowDC);
    }

    void WindowChooserWindows::foundWindow(WId wid)
    {
        if(!isWindowValid(wid, mLastFoundWindow))
            return;

        if(mLastFoundWindow)
            refreshWindow(mLastFoundWindow);

        highlightWindow(wid, mRectanglePen);

        mLastFoundWindow = wid;
    }

    void WindowChooserWindows::stopMouseCapture()
	{
        if(!mSearching)
            return;

        mSearching = false;

        if(mPreviousCursor)
            SetCursor(mPreviousCursor);

        if(mLastFoundWindow)
            refreshWindow(mLastFoundWindow);

        QCoreApplication::instance()->removeNativeEventFilter(this);
    }

    bool WindowChooserWindows::nativeEventFilter(const QByteArray &eventType, void *message, long *)
    {
        if(eventType != "windows_generic_MSG")
            return false;

        MSG *msg = static_cast<MSG*>(message);

        if(!msg || !mSearching)
            return false;

        switch(msg->message)
        {
        case WM_KEYDOWN:
            switch(msg->wParam)
            {
            case VK_ESCAPE:
                stopMouseCapture();

                emit canceled();
                break;
            }
            break;
        case WM_LBUTTONUP:
            stopMouseCapture();

            emit done(mLastFoundWindow);
            break;
        case WM_MOUSEMOVE:
            {
                POINT screenpoint;
                GetCursorPos(&screenpoint);

                HWND window = nullptr;
                HWND firstWindow = WindowFromPoint(screenpoint);
                if(firstWindow && IsWindow(firstWindow))
                {
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
                                firstWindow	= childWindow;
                        }
                    }

                    if(GetParent(firstWindow))
                    {
                        RECT rcFirst;
                        GetWindowRect(firstWindow, &rcFirst);

                        bool bestFound = false;
                        auto other = firstWindow;
                        do
                        {
                            other	= GetNextWindow(other, GW_HWNDPREV);
                            if(!other)
                                break;
                            RECT otherRect;
                            GetWindowRect(other, &otherRect);
                            if(PtInRect(screenpoint, otherRect) &&
                                PtInRect(RectTopLeft(otherRect), rcFirst) &&
                                PtInRect(RectBottomRight(otherRect), rcFirst))
                            {
                                firstWindow = other;
                                bestFound = true;
                            }
                        }
                        while(!bestFound);

                        if(!bestFound)
                        {
                            other = firstWindow;
                            do
                            {
                                other = GetNextWindow(other, GW_HWNDNEXT);
                                if(!other)
                                    break;
                                RECT otherRect;
                                GetWindowRect(other, &otherRect);
                                if(PtInRect(screenpoint, otherRect) &&
                                    PtInRect(RectTopLeft(otherRect), rcFirst) &&
                                    PtInRect(RectBottomRight(otherRect), rcFirst))
                                {
                                    firstWindow	= other;
                                    bestFound = true;
                                }
                            }
                            while(!bestFound);
                        }
                    }

                    window = firstWindow;
                }
                else
                    break;

                foundWindow(reinterpret_cast<WId>(window));
            }
            break;
        }

        return false;
    }
}
