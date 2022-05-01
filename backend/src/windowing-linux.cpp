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

#include "backend/windowing-linux.hpp"

#include <QDBusInterface>

namespace Backend
{
    void FullscreenScreenshooterLinux::capture(bool includeCursor, bool includeFrame, bool flash)
    {
    }

    void AreaScreenshooterLinux::capture(bool includeCursor, bool includeFrame, bool flash)
    {

    }

    void WindowScreenshooterLinux::capture(bool includeCursor, bool includeFrame, bool flash)
    {

    }

    Screenshooter* createFullscreenScreenshooterLinux(QObject *parent)
    {

    }

    Screenshooter* createAreaScreenshooterLinux(QObject *parent)
    {

    }

    Screenshooter* createWindowScreenshooterLinux(QObject *parent)
    {

    }

    bool checkFullscreenScreenshooterGnome()
    {
        QDBusInterface interface(QStringLiteral("org.gnome.Shell"), QStringLiteral("/org/gnome/Shell/Screenshot"), QStringLiteral("org.gnome.Shell.Screenshot"));
        if(!interface.isValid())
            return false;
        interface.call(QStringLiteral("FlashArea"), 0, 0, 0, 0); // TODO
        return true;
    }

    bool checkAreaScreenshooterGnome()
    {

    }

    bool checkWindowScreenshooterGnome()
    {

    }
}
