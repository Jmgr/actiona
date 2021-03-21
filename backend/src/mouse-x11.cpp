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

#include "backend/mouse-x11.hpp"

#include <QX11Info>
#include <QCursor>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

namespace Backend
{
    int toX11Button(Mouse::Button button)
    {
        return static_cast<int>(button) + 1;
    }

    bool isButtonPressedX11(Mouse::Button button)
    {
        Window unusedWindow;
        int unusedInt;
        unsigned int buttonMask;

        if(!XQueryPointer(QX11Info::display(),
                          QX11Info::appRootWindow(),
                          &unusedWindow,
                          &unusedWindow,
                          &unusedInt,
                          &unusedInt,
                          &unusedInt,
                          &unusedInt,
                          &buttonMask))
            throw BackendError();

        switch(button)
        {
        case Mouse::Button::Left:
            return (buttonMask & Button1Mask);
        case Mouse::Button::Middle:
            return (buttonMask & Button2Mask);
        case Mouse::Button::Right:
            return (buttonMask & Button3Mask);
        default:
            return false;
        }
    }

    QPoint cursorPositionX11()
    {
        return QCursor::pos();
    }

    void setCursorPositionX11(const QPoint &position)
    {
        QCursor::setPos(position);
    }

    void pressButtonX11(Mouse::Button button, bool press)
    {
        if(!XTestFakeButtonEvent(QX11Info::display(), toX11Button(button), press, CurrentTime))
            throw BackendError(); // TODO: get error message

        if(!XFlush(QX11Info::display()))
            throw BackendError();
    }

    void rotateWheelX11(int intensity)
    {
        int button;
        if(intensity < 0)
        {
            button = Button5;
            intensity = -intensity;
        }
        else
            button = Button4;

        for(int i = 0; i < intensity; ++i)
        {
            if(!XTestFakeButtonEvent(QX11Info::display(), button, True, CurrentTime))
                throw BackendError();

            if(!XTestFakeButtonEvent(QX11Info::display(), button, False, CurrentTime))
                throw BackendError();

            if(!XFlush(QX11Info::display()))
                throw BackendError();
        }
    }
}

