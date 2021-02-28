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

#include "backend/mouse-output-windows.hpp"

#include <QCursor>

#include <Windows.h>

namespace Backend
{
    int toWinButton(Mouse::Button button, bool press)
    {
        switch(button)
        {
        case Mouse::LeftButton:
            return press ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
        case Mouse::MiddleButton:
            return press ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
        case Mouse::RightButton:
            return press ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
        }

        return -1;
    }

    bool pressButton(Mouse::Button button, bool press)
    {
        INPUT input;
        SecureZeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = toWinButton(button, press);

        return SendInput(1, &input, sizeof(INPUT));
    }

    MouseOutputWindows::MouseOutputWindows(QObject *parent):
        MouseOutput(parent)
    {
    }

    void MouseOutputWindows::setCursorPosition(const QPoint &position)
    {
        QCursor::setPos(position);
    }

    bool MouseOutputWindows::buttonClick(Mouse::Button button)
    {
        return pressButton(button) && releaseButton(button);
    }

    bool MouseOutputWindows::pressButton(Mouse::Button button)
    {
        setButtonPressed(button, true);

        return ::Backend::pressButton(button, true);
    }

    bool MouseOutputWindows::releaseButton(Mouse::Button button)
    {
        setButtonPressed(button, false);

        return ::Backend::pressButton(button, false);
    }

    bool MouseOutputWindows::wheel(int intensity)
    {
        INPUT input;
        SecureZeroMemory(&input, sizeof(INPUT));
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
        input.mi.mouseData = intensity * WHEEL_DELTA;

        return SendInput(1, &input, sizeof(INPUT));
    }
}
