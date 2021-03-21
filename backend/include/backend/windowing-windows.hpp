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

#pragma once

#include "backend/backend_global.hpp"
#include "backend/windowing.hpp"

namespace Backend
{
    void BACKENDSHARED_EXPORT setForegroundWindowWindows(WId windowId);
    QString BACKENDSHARED_EXPORT titleWindows(WId windowId);
    QString BACKENDSHARED_EXPORT classnameWindows(WId windowId);
    QRect BACKENDSHARED_EXPORT rectWindows(WId windowId, bool useBorders);
    int BACKENDSHARED_EXPORT processIdWindows(WId windowId);
    void BACKENDSHARED_EXPORT closeWindows(WId windowId);
    void BACKENDSHARED_EXPORT killCreatorWindows(WId windowId);
    void BACKENDSHARED_EXPORT minimizeWindows(WId windowId);
    void BACKENDSHARED_EXPORT maximizeWindows(WId windowId);
    void BACKENDSHARED_EXPORT moveWindows(WId windowId, QPoint position);
    void BACKENDSHARED_EXPORT resizeWindows(WId windowId, QSize size, bool useBorders);
    bool BACKENDSHARED_EXPORT isActiveWindows(WId windowId);
    WId BACKENDSHARED_EXPORT foregroundWindowWindows();
    QList<WId> BACKENDSHARED_EXPORT windowListWindows();
    PositionChooser BACKENDSHARED_EXPORT *createPositionChooserWindows(QObject *parent);
    AreaChooser BACKENDSHARED_EXPORT *createAreaChooserWindows(QObject *parent);
    WindowChooser BACKENDSHARED_EXPORT *createWindowChooserWindows(QObject *parent);
}
