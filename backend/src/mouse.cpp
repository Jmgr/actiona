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

#include "backend/mouse.hpp"
#include "backend/capabilities.hpp"
#include "backend/mouse-qt.hpp"

#if defined(Q_OS_LINUX)
#include "backend/mouse-xtest.hpp"
#endif

namespace Backend
{
    Mouse::Mouse(Capabilities &caps)
    {
        cursorPosition.addImplementation(qtImpl, cursorPositionQt);
        setCursorPosition.addImplementation(qtImpl, setCursorPositionQt);
#if defined(Q_OS_LINUX)
        if(caps.hasXTest())
        {
            isButtonPressed.addImplementation(x11Impl, isButtonPressedXTest);
            pressButton.addImplementation(x11Impl, pressButtonXTest);
            rotateWheel.addImplementation(x11Impl, rotateWheelXTest);
        }
#endif

        cursorPosition.choose();
        setCursorPosition.choose();
        isButtonPressed.choose();
        pressButton.choose();
        rotateWheel.choose();
    }
}
