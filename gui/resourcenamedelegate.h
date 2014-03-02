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

#ifndef RESOURCENAMEDELEGATE_H
#define RESOURCENAMEDELEGATE_H

#include <QStyledItemDelegate>

class ResourceTableWidget;

class ResourceNameDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit ResourceNameDelegate(ResourceTableWidget *resourceTableWidget, QObject *parent = 0);
    
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    ResourceTableWidget *mResourceTableWidget;
};

#endif // RESOURCENAMEDELEGATE_H
