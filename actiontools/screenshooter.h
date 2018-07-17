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

    Contact : jmgr@jmgr.info
*/

#pragma once

#include "actiontools_global.h"

#include <QList>
#include <QPixmap>

#include <utility>

namespace ActionTools
{
    class WindowHandle;

    class ACTIONTOOLSSHARED_EXPORT ScreenShooter
    {
    public:
        static QPixmap captureScreen(int screenIndex);
        static QList<std::pair<QPixmap, QRect>> captureScreens();
        static QList<std::pair<QPixmap, QRect>> captureWindows(const QList<WindowHandle> &windows);
        static QPixmap captureWindow(WindowHandle window);
        static QPixmap captureAllScreens();
        static QPixmap captureRect(const QRect &rect);

        ScreenShooter() = delete;
    };
}

