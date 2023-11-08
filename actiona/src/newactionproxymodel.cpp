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

#include "newactionproxymodel.hpp"
#include "newactionmodel.hpp"

NewActionProxyModel::NewActionProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void NewActionProxyModel::setFilterString(const QString &filterString)
{
    mFilterString = filterString;

    invalidateFilter();
}

bool NewActionProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(mFilterString.isEmpty())
        return true;

    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    {
        QString actionId = index.data(NewActionModel::ActionIdRole).toString();

        if(actionId.isEmpty())
            return true; // We always accept categories
    }

    QString actionName = index.data(Qt::DisplayRole).toString();

    return actionName.contains(mFilterString, Qt::CaseInsensitive);
}

