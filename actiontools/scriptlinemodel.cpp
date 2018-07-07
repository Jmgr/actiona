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

#include "scriptlinemodel.h"
#include "script.h"
#include "numberformat.h"

namespace ActionTools
{
    ScriptLineModel::ScriptLineModel(Script &script, QObject *parent):
        QAbstractListModel{parent},
        mScript{script}
    {
    }

    ScriptLineModel::~ScriptLineModel()
    {
    }

    void ScriptLineModel::update()
    {
        emit dataChanged(index(0), index(mScript.actionCount() - 1));
    }

    int ScriptLineModel::rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent)

        int labelCount = mScript.labels().size();
        if(labelCount > 0)
            ++labelCount;

        int lineCount = mScript.actionCount();
        if(lineCount > 0)
            ++lineCount;

        return labelCount + lineCount;
    }

    QVariant ScriptLineModel::data(const QModelIndex &index, int role) const
    {
        if(!index.isValid() || index.row() >= rowCount())
            return {};

        const auto &labels = mScript.labels();
        int labelCount = labels.empty() ? 0 : labels.size() + 1;

        switch(role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            {
                if(labels.size() > 0 && index.row() < labelCount)
                {
                    if(index.row() == 0)
                        return tr("Labels");
                    else
                        return labels[index.row() - 1];
                }
                else if(mScript.actionCount() > 0 && index.row() < labelCount + mScript.actionCount() + 1)
                {
                    if(index.row() == labelCount)
                        return tr("Lines");
                    else
                        return NumberFormat::labelIndexString(index.row() - labelCount - 1);
                }
            }
            break;
        case HeaderRole:
            return (labels.size() > 0 && index.row() == 0) || (mScript.actionCount() > 0 && index.row() == labelCount);
        }

        return {};
    }

    QVariant ScriptLineModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        Q_UNUSED(section)
        Q_UNUSED(orientation)
        Q_UNUSED(role)

        return {};
    }

    Qt::ItemFlags ScriptLineModel::flags(const QModelIndex &index) const
    {
        Qt::ItemFlags flags = Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemIsSelectable;

        if (!index.isValid())
            return flags;

        auto userData = index.data(HeaderRole);
        if(userData.isValid() && userData.toBool())
        {
            flags &= ~Qt::ItemIsSelectable;
            flags &= ~Qt::ItemIsEnabled;
        }

        return flags;
    }
}
