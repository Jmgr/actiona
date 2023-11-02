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

#include "resourcenamedelegate.hpp"
#include "actiontools/resource.hpp"
#include "resourcetablewidget.hpp"
#include "actiontools/actioninstance.hpp"

#include <QLineEdit>
#include <QRegularExpressionValidator>

ResourceNameDelegate::ResourceNameDelegate(ResourceTableWidget *resourceTableWidget, QObject *parent)
    : QStyledItemDelegate(parent),
      mResourceTableWidget(resourceTableWidget)
{
}

QWidget* ResourceNameDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    auto lineEdit = new QLineEdit(parent);

    lineEdit->setValidator(new QRegularExpressionValidator(ActionTools::ActionInstance::NameRegExp, lineEdit));

    return lineEdit;
}

void ResourceNameDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(auto lineEdit = qobject_cast<QLineEdit *>(editor))
        lineEdit->setText(index.data().toString());
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void ResourceNameDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(auto lineEdit = qobject_cast<QLineEdit *>(editor))
    {
        QString name = lineEdit->text();

        if(!name.isEmpty() && !mResourceTableWidget->containsResource(name) && ActionTools::ActionInstance::NameRegExp.match(name).hasMatch())
            model->setData(index, lineEdit->text(), Qt::DisplayRole);
    }
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}
