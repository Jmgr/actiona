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

#include "scriptproxymodel.h"
#include "scriptmodel.h"
#include "script.h"
#include "actioninstance.h"
#include "actiondefinition.h"
#include "numberformat.h"

#include <QDebug>

ScriptProxyModel::ScriptProxyModel(ActionTools::Script *script, QObject *parent)
    : QSortFilterProxyModel(parent),
      mScript(script),
      mFilteringFlags(ActionFilteringFlag::AllFlags)
{
}

void ScriptProxyModel::setFilterString(const QString &filterString)
{
    mFilterString = filterString;

    invalidateFilter();
}

void ScriptProxyModel::setFilteringFlags(ActionFilteringFlags filteringFlags)
{
    mFilteringFlags = filteringFlags;

    invalidateFilter();
}

bool ScriptProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(mFilterString.isEmpty())
        return true;

    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    if(!index.isValid())
        return false;

    if(index.row() >= mScript->actionCount())
        return false;

    ActionTools::ActionInstance *actionInstance = mScript->actionAt(index.row());

    if(!actionInstance)
        return false;

    const ActionTools::ActionDefinition *actionDefinition = actionInstance->definition();

    if((mFilteringFlags == 0 || mFilteringFlags.testFlag(ActionFilteringFlag::ActionName)) && actionDefinition->name().contains(mFilterString, Qt::CaseInsensitive))
        return true;
    if((mFilteringFlags == 0 || mFilteringFlags.testFlag(ActionFilteringFlag::Label)) && ((actionInstance->label().contains(mFilterString, Qt::CaseInsensitive)) || ActionTools::NumberFormat::labelIndexString(index.row()).contains(mFilterString, Qt::CaseInsensitive)))
        return true;
    if(((mFilteringFlags == 0 || mFilteringFlags.testFlag(ActionFilteringFlag::Comment)) && actionInstance->comment().contains(mFilterString, Qt::CaseInsensitive)))
        return true;
    if(mFilteringFlags == 0 || mFilteringFlags.testFlag(ActionFilteringFlag::CodeParameters))
    {
        for(const auto &parameter: actionInstance->parametersData())
        {
            for(const auto &subParameter: parameter.subParameters())
            {
                if(subParameter.isCode() && subParameter.value().contains(mFilterString, Qt::CaseInsensitive))
                    return true;
            }
        }
    }
    if(mFilteringFlags == 0 || mFilteringFlags.testFlag(ActionFilteringFlag::TextParameters))
    {
        for(const auto &parameter: actionInstance->parametersData())
        {
            for(const auto &subParameter: parameter.subParameters())
            {
                if(!subParameter.isCode() && subParameter.value().contains(mFilterString, Qt::CaseInsensitive))
                    return true;
            }
        }
    }

    return false;
}

