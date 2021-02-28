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

#include "backend/mouse-input-windows.hpp"

#include <QCursor>

#include <Windows.h>

namespace Backend
{
    bool MouseInputWindows::isButtonPressed(Button button) const
    {
        switch(button)
        {
        case LeftButton:
            return (GetAsyncKeyState(GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON) & 0x8000);
        case MiddleButton:
            return (GetAsyncKeyState(VK_MBUTTON) & 0x8000);
        case RightButton:
            return (GetAsyncKeyState(GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON) & 0x8000);
        default:
            return false;
        }
    }

    QPoint MouseInputWindows::cursorPosition() const
    {
        return QCursor::pos();
    }
}
