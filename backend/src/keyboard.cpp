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

#include "backend/keyboard.hpp"
#include "backend/capabilities.hpp"

#if defined(Q_OS_LINUX)
#include "backend/keyboard-xtest.hpp"
#endif

namespace Backend
{
    Keyboard::Keyboard(Capabilities &caps)
    {
#if defined(Q_OS_LINUX)
        if(caps.hasXTest())
        {
            pressKey.addImplementation(x11Impl, pressKeyXTest);
            writeText.addImplementation(x11Impl, writeTextXTest);
        }
#endif

        pressKey.choose();
        writeText.choose();
    }
}
