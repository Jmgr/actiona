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

#include <QSortFilterProxyModel>

#include "actionfilteringflags.h"

namespace ActionTools
{
    class Script;
}

class ScriptProxyModel : public QSortFilterProxyModel
{
public:
    ScriptProxyModel(ActionTools::Script *script, QObject *parent = nullptr);

    void setFilterString(const QString &filterString);
    void setFilteringFlags(ActionFilteringFlags filteringFlags);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    ActionTools::Script *mScript;
    QString mFilterString;
    ActionFilteringFlags mFilteringFlags;
};

