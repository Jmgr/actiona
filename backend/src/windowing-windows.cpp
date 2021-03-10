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

#include "backend/windowing-windows.hpp"
#include "backend/backend.hpp"
#include "backend/backend-windows.hpp"
#include "backend/process.hpp"

#include <QWidget>

#include <Windows.h>
#include <Tlhelp32.h>

#include <vector>
#include <array>

namespace Backend
{
    void setForegroundWindowWindows(WId windowId)
    {
        auto hwnd = reinterpret_cast<HWND>(windowId);

        if(IsIconic(hwnd))
            ShowWindow(hwnd, SW_RESTORE);
        else
        {
            if(!SetForegroundWindow(hwnd))
                throw BackendError(QObject::tr("failed to set window %1 foreground").arg(windowId));

            if(!SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
                throw BackendError(lastErrorString());
        }
    }

    QString titleWindows(WId windowId)
    {
        auto hwnd = reinterpret_cast<HWND>(windowId);
        QString title;

        SetLastError(0);
        int titleLength = GetWindowTextLength(hwnd);
        if(titleLength == 0)
        {
            if(GetLastError() != ERROR_SUCCESS)
                throw BackendError(lastErrorString());
        }

        if(titleLength >= 0)
        {
            std::vector<wchar_t> titleName(titleLength + 1);

            if(!GetWindowText(hwnd, titleName.data(), titleLength + 1))
            {
                if(GetLastError() != ERROR_SUCCESS)
                    throw BackendError(lastErrorString());
            }
            title = QString::fromWCharArray(titleName.data());
        }

        return title;
    }

    QString classnameWindows(WId windowId)
    {
        auto hwnd = reinterpret_cast<HWND>(windowId);
        std::array<wchar_t, 255> className;

        if(!GetClassName(hwnd, className.data(), className.size()))
        {
            if(GetLastError() != ERROR_SUCCESS)
                throw BackendError(lastErrorString());
        }

        return QString::fromWCharArray(className.data());
    }

    QRect rectWindows(WId windowId, bool useBorders)
    {
        auto hwnd = reinterpret_cast<HWND>(windowId);
        RECT rect;

        if(useBorders)
        {
            if(!GetWindowRect(hwnd, &rect))
                throw BackendError(lastErrorString());
        }
        else
        {
            if(!GetClientRect(hwnd, &rect))
                throw BackendError(lastErrorString());
        }

        return {QPoint(rect.left, rect.top), QPoint(rect.right, rect.bottom)};
    }

    int processIdWindows(WId windowId)
    {
        auto hwnd = reinterpret_cast<HWND>(windowId);
        DWORD procID;

        GetWindowThreadProcessId(hwnd, &procID);

        return procID;
    }

    void closeWindows(WId windowId)
    {
        auto hwnd = reinterpret_cast<HWND>(windowId);

        if(!SendNotifyMessage(hwnd, WM_CLOSE, 0, 0))
            throw BackendError(lastErrorString());
    }

    void killCreatorWindows(WId windowId)
    {
        int id = Instance::windowing().processId(windowId);

        Instance::process().killProcess(id, Backend::Process::KillMode::GracefulThenForceful, 3000);
    }

    void minimizeWindows(WId windowId)
    {
        auto hwnd = reinterpret_cast<HWND>(windowId);

        if(SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0))
            throw BackendError(QObject::tr("SendMessage failed"));
    }

    void maximizeWindows(WId windowId)
    {
        auto hwnd = reinterpret_cast<HWND>(windowId);

        if(SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0))
            throw BackendError(QObject::tr("SendMessage failed"));
    }

    void moveWindows(WId windowId, QPoint position)
    {
        auto hwnd = reinterpret_cast<HWND>(windowId);

        if(!SetWindowPos(hwnd, 0, position.x(), position.y(), 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE))
            throw BackendError(lastErrorString());
    }

    void resizeWindows(WId windowId, QSize size, bool useBorders)
    {
        auto hwnd = reinterpret_cast<HWND>(windowId);
        auto rect = Instance::windowing().rect;

        if(!useBorders)
        {
            const QRect &sizeWithBorders = rect(windowId, true);
            const QRect &sizeWithoutBorders = rect(windowId, false);

            int borderWidth = sizeWithBorders.width() - sizeWithoutBorders.width();
            int borderHeight = sizeWithBorders.height() - sizeWithoutBorders.height();

            if(borderWidth < 0)
                borderWidth = 0;
            if(borderHeight < 0)
                borderHeight = 0;

            size.rwidth() += borderWidth;
            size.rheight() += borderHeight;
        }

        if(!SetWindowPos(hwnd, 0, 0, 0, size.width(), size.height(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE))
            throw BackendError(lastErrorString());
    }

    bool isActiveWindows(WId windowId)
    {
        return (Instance::windowing().foregroundWindow() == windowId);
    }

    WId foregroundWindowWindows()
    {
        return reinterpret_cast<WId>(GetForegroundWindow());
    }

    QList<WId> windowListWindows()
    {
        auto hdesk = OpenInputDesktop(0, false, DESKTOP_READOBJECTS);

        QList<WId> result;

        auto res = EnumDesktopWindows(hdesk, [](HWND hwnd, LPARAM lParam) -> BOOL {
            auto &result = *reinterpret_cast<QList<WId> *>(lParam);

            if(IsWindowVisible(hwnd))
                result.append(reinterpret_cast<WId>(hwnd));

            return true;
        }, reinterpret_cast<LPARAM>(&result));
        CloseDesktop(hdesk);

        if(!res)
            throw BackendError(lastErrorString());

        return result;
    }
}

