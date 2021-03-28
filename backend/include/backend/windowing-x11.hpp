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

#include "backend/windowing.hpp"

namespace Backend
{
    void setForegroundWindowX11(WId windowId);
    QString titleX11(WId windowId);
    QString classnameX11(WId windowId);
    QRect rectX11(WId windowId, bool useBorders);
    int processIdX11(WId windowId);
    void closeX11(WId windowId);
    void killCreatorX11(WId windowId);
    void minimizeX11(WId windowId);
    void maximizeX11(WId windowId);
    void moveX11(WId windowId, const QPoint &position);
    void resizeX11(WId windowId, const QSize &size, bool useBorders);
    bool isActiveX11(WId windowId);
    WId foregroundWindowX11();
    QList<WId> windowListX11();
    PositionChooser *createPositionChooserX11(QObject *parent);
    AreaChooser *createAreaChooserX11(QObject *parent);
    WindowChooser *createWindowChooserX11(QObject *parent);
}
