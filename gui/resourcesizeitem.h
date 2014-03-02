/*
    Actionaz
    Copyright (C) 2008-2014 Jonathan Mercier-Ganady

    Actionaz is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Actionaz is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    Contact : jmgr@jmgr.info
*/

#ifndef RESOURCESIZEITEM_H
#define RESOURCESIZEITEM_H

#include <QTableWidgetItem>

class ResourceSizeItem : public QTableWidgetItem
{
public:
    explicit ResourceSizeItem()
        : QTableWidgetItem()
    {
    }

    bool operator<(const QTableWidgetItem &other) const
    {
        return data(Qt::UserRole).toInt() < other.data(Qt::UserRole).toInt();
    }
};

#endif // RESOURCESIZEITEM_H
