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

#include "backend/windowing.hpp"
#include "backend/capabilities.hpp"

#if defined(Q_OS_LINUX)
#include "backend/windowing-x11.hpp"
#endif

namespace Backend
{
    Windowing::Windowing(Capabilities &caps)
    {
#if defined(Q_OS_LINUX)
        if(caps.hasX11())
        {
            setForegroundWindow.addImplementation(x11Impl, setForegroundWindowX11);
            title.addImplementation(x11Impl, titleX11);
            classname.addImplementation(x11Impl, classnameX11);
            rect.addImplementation(x11Impl, rectX11);
            processId.addImplementation(x11Impl, processIdX11);
            close.addImplementation(x11Impl, closeX11);
            killCreator.addImplementation(x11Impl, killCreatorX11);
            minimize.addImplementation(x11Impl, minimizeX11);
            maximize.addImplementation(x11Impl, maximizeX11);
            move.addImplementation(x11Impl, moveX11);
            resize.addImplementation(x11Impl, resizeX11);
            isActive.addImplementation(x11Impl, isActiveX11);
            foregroundWindow.addImplementation(x11Impl, foregroundWindowX11);
            windowList.addImplementation(x11Impl, windowListX11);
            createWindowsHidingTool.addImplementation(x11Impl, createWindowsHidingToolX11);
            createPositionChooser.addImplementation(x11Impl, createPositionChooserX11);
            createAreaChooser.addImplementation(x11Impl, createAreaChooserX11);
            createWindowChooser.addImplementation(x11Impl, createWindowChooserX11);
            /*
             * // TODO
            createFullscreenScreenshooter.addImplementation(x11Impl, createFullscreenScreenshooterX11);
            createAreaScreenshooter.addImplementation(x11Impl, createAreaScreenshooterX11);
            createWindowScreenshooter.addImplementation(x11Impl, createWindowScreenshooterX11);
            */
        }
#endif

        setForegroundWindow.choose();
        title.choose();
        classname.choose();
        rect.choose();
        processId.choose();
        close.choose();
        killCreator.choose();
        minimize.choose();
        maximize.choose();
        move.choose();
        resize.choose();
        isActive.choose();
        foregroundWindow.choose();
        windowList.choose();
        createWindowsHidingTool.choose();
        createPositionChooser.choose();
        createAreaChooser.choose();
        createWindowChooser.choose();
        createFullscreenScreenshooter.choose();
        createAreaScreenshooter.choose();
        createWindowScreenshooter.choose();
    }
}
