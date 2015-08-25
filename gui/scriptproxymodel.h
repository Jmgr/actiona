/*
    Actiona
    Copyright (C) 2008-2015 Jonathan Mercier-Ganady

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

#ifndef SCRIPTPROXYMODEL_HPP
#define SCRIPTPROXYMODEL_HPP

#include <QSortFilterProxyModel>

namespace ActionTools
{
    class Script;
}

class ScriptProxyModel : public QSortFilterProxyModel
{
public:
    enum class FilteringCriterion
    {
        All,
        Label,
        ActionName,
        Comment
    };

    ScriptProxyModel(ActionTools::Script *script, QObject *parent = 0);

    void setFilterString(const QString &filterString);
    void setFilteringCriterion(FilteringCriterion filteringCriterion);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    ActionTools::Script *mScript;
    QString mFilterString;
    FilteringCriterion mFilteringCriterion;
};

#endif // SCRIPTPROXYMODEL_HPP
