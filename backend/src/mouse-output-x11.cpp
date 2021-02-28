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

#include "backend/mouse-output-x11.hpp"

#include <QCursor>
#include <QX11Info>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

namespace Backend
{
    int toX11Button(Mouse::Button button)
    {
        return button + 1;
    }

    bool pressButton(Mouse::Button button, bool press)
    {
        if(!XTestFakeButtonEvent(QX11Info::display(), toX11Button(button), press, CurrentTime))
            return false;

        XFlush(QX11Info::display());

        return true;
    }

    MouseOutputX11::MouseOutputX11(QObject *parent):
        MouseOutput(parent)
    {
    }

    void MouseOutputX11::setCursorPosition(const QPoint &position)
    {
        QCursor::setPos(position);
    }

    bool MouseOutputX11::buttonClick(Mouse::Button button)
    {
        return pressButton(button) && releaseButton(button);
    }

    bool MouseOutputX11::pressButton(Mouse::Button button)
    {
        return ::Backend::pressButton(button, true);
    }

    bool MouseOutputX11::releaseButton(Mouse::Button button)
    {
        return ::Backend::pressButton(button, false);
    }

    bool MouseOutputX11::wheel(int intensity)
    {
        int button;
        if(intensity < 0)
        {
            button = Button5;
            intensity = -intensity;
        }
        else
            button = Button4;

        bool result = true;

        for(int i = 0; i < intensity; ++i)
        {
            result &= XTestFakeButtonEvent(QX11Info::display(), button, True, CurrentTime);
            result &= XTestFakeButtonEvent(QX11Info::display(), button, False, CurrentTime);

            XFlush(QX11Info::display());
        }

        return result;
    }
}
