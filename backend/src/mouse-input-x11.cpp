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

#include "backend/mouse-input-x11.hpp"

#include <QCursor>
#include <QX11Info>

#include <X11/Xlib.h>

namespace Backend
{
    bool MouseInputX11::isButtonPressed(Button button) const
    {
        Window unusedWindow;
        int unusedInt;
        unsigned int buttonMask;

        if(!XQueryPointer(QX11Info::display(),
                          XDefaultRootWindow(QX11Info::display()),
                          &unusedWindow,
                          &unusedWindow,
                          &unusedInt,
                          &unusedInt,
                          &unusedInt,
                          &unusedInt,
                          &buttonMask))
            return false;

        switch(button)
        {
        case LeftButton:
            return (buttonMask & Button1Mask);
        case MiddleButton:
            return (buttonMask & Button2Mask);
        case RightButton:
            return (buttonMask & Button3Mask);
        default:
            return false;
        }
    }

    QPoint MouseInputX11::cursorPosition() const
    {
        return QCursor::pos();
    }
}
