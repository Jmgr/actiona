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

#include <QGuiApplication>

#include "actiontools/x11info.hpp"

namespace ActionTools
{
    Display *X11Info::display()
    {
        auto x11App = qGuiApp->nativeInterface<QNativeInterface::QX11Application>();
        return x11App ? x11App->display() : nullptr;
    }

    Window X11Info::appRootWindow(int screen)
    {
        auto display = X11Info::display();
        return display ? RootWindow(display, screen == -1 ? DefaultScreen(display) : screen) : 0;
    }
}
