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

#include "backend/system.hpp"
#include "backend/capabilities.hpp"

#if defined(Q_OS_LINUX)
#include "backend/system-unix.hpp"
#endif

namespace Backend
{
    System::System(Capabilities &caps)
    {
#if defined(Q_OS_LINUX)
        logout.addImplementation(unixImpl, logoutUnix);
        restart.addImplementation(unixImpl, restartUnix);
        shutdown.addImplementation(unixImpl, shutdownUnix);
        suspend.addImplementation(unixImpl, suspendUnix);
        hibernate.addImplementation(unixImpl, hibernateUnix);
        lockScreen.addImplementation(unixImpl, lockScreenUnix);
        startScreenSaver.addImplementation(unixImpl, startScreenSaverUnix);
        getUsername.addImplementation(unixImpl, getUsernameUnix);
        copyFiles.addImplementation(unixImpl, copyFilesUnix);
        moveFiles.addImplementation(unixImpl, moveFilesUnix);
        renameFiles.addImplementation(unixImpl, renameFilesUnix);
        removeFiles.addImplementation(unixImpl, removeFilesUnix);
#endif

        logout.choose();
        restart.choose();
        shutdown.choose();
        suspend.choose();
        hibernate.choose();
        lockScreen.choose();
        startScreenSaver.choose();
        getUsername.choose();
        copyFiles.choose();
        moveFiles.choose();
        renameFiles.choose();
        removeFiles.choose();
    }
}
