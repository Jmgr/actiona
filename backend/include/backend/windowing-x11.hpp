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
    void BACKENDSHARED_EXPORT setForegroundWindowX11(WId windowId);
    QString BACKENDSHARED_EXPORT titleX11(WId windowId);
    QString BACKENDSHARED_EXPORT classnameX11(WId windowId);
    QRect BACKENDSHARED_EXPORT rectX11(WId windowId, bool useBorders);
    int BACKENDSHARED_EXPORT processIdX11(WId windowId);
    void BACKENDSHARED_EXPORT closeX11(WId windowId);
    void BACKENDSHARED_EXPORT killCreatorX11(WId windowId);
    void BACKENDSHARED_EXPORT minimizeX11(WId windowId);
    void BACKENDSHARED_EXPORT maximizeX11(WId windowId);
    void BACKENDSHARED_EXPORT moveX11(WId windowId, QPoint position);
    void BACKENDSHARED_EXPORT resizeX11(WId windowId, QSize size, bool useBorders);
    bool BACKENDSHARED_EXPORT isActiveX11(WId windowId);
    WId BACKENDSHARED_EXPORT foregroundWindowX11();
    QList<WId> BACKENDSHARED_EXPORT windowListX11();
    PositionChooser BACKENDSHARED_EXPORT *createPositionChooserX11(QObject *parent);
    AreaChooser BACKENDSHARED_EXPORT *createAreaChooserX11(QObject *parent);
    WindowChooser BACKENDSHARED_EXPORT *createWindowChooserX11(QObject *parent);
}
