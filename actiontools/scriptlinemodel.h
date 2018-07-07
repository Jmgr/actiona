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

#include <QAbstractTableModel>

namespace ActionTools
{
    class Script;

    class ScriptLineModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        static const int HeaderRole = Qt::UserRole;

        ScriptLineModel(Script &script, QObject *parent = nullptr);
        ~ScriptLineModel() override;

        void update();

    protected:
        int rowCount(const QModelIndex &parent = {}) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;

    private:
        Script &mScript;
        QString mValue;

        Q_DISABLE_COPY(ScriptLineModel)
    };
}
