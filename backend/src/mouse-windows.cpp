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

#include "backend/mouse-windows.hpp"

#include <QCursor>

#include <Windows.h>

namespace Backend
{
    int toWinButton(Mouse::Button button, bool press)
    {
        switch(button)
        {
        case Mouse::Button::Left:
            return press ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
        case Mouse::Button::Middle:
            return press ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
        case Mouse::Button::Right:
            return press ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
        case Mouse::Button::Count:
        default:
            throw BackendError(QStringLiteral("unknown mouse button: %1").arg(static_cast<int>(button)));
        }
    }

    void pressButton(Mouse::Button button, bool press)
    {
        INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = toWinButton(button, press);

        auto res = SendInput(1, &input, sizeof(INPUT));
        if(res == 0)
            throw BackendError(QStringLiteral("failed to press button, error code: %1").arg(GetLastError()));
        if(res != 1)
            throw BackendError(QStringLiteral("failed to press button"));
    }

    bool isButtonPressedWindows(Mouse::Button button)
    {
        switch(button)
        {
        case Mouse::Button::Left:
            return (GetAsyncKeyState(GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON) & 0x8000);
        case Mouse::Button::Middle:
            return (GetAsyncKeyState(VK_MBUTTON) & 0x8000);
        case Mouse::Button::Right:
            return (GetAsyncKeyState(GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON) & 0x8000);
        case Mouse::Button::Count:
        default:
            throw BackendError(QStringLiteral("unknown mouse button: %1").arg(static_cast<int>(button)));
        }
    }

    QPoint cursorPositionWindows()
    {
        return QCursor::pos();
    }

    void setCursorPositionWindows(const QPoint &position)
    {
        QCursor::setPos(position);
    }

    void pressButtonWindows(Mouse::Button button, bool press)
    {
        pressButton(button, press);
    }

    void rotateWheelWindows(int intensity)
    {
        INPUT input;
        ZeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        input.mi.mouseData = intensity * WHEEL_DELTA;

        auto res = SendInput(1, &input, sizeof(INPUT));
        if(res == 0)
            throw BackendError(QStringLiteral("failed to rotate wheel, error code: %1").arg(GetLastError()));
        if(res != 1)
            throw BackendError(QStringLiteral("failed to rotate wheel"));
    }
}
