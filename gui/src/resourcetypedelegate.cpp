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

#include "resourcetypedelegate.hpp"
#include "actiontools/resource.hpp"

#include <QComboBox>

ResourceTypeDelegate::ResourceTypeDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget* ResourceTypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    auto comboBox = new QComboBox(parent);
    comboBox->setEditable(false);

    for(int i = 0; i < ActionTools::Resource::TypeCount; ++i)
        comboBox->addItem(ActionTools::Resource::typeNames[i]);

    comboBox->setCurrentIndex(index.data(Qt::UserRole).toInt());

    return comboBox;
}


void ResourceTypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(auto comboBox = qobject_cast<QComboBox *>(editor))
        comboBox->setCurrentIndex(index.data(Qt::UserRole).toInt());
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void ResourceTypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(auto comboBox = qobject_cast<QComboBox *>(editor))
    {
        model->setData(index, comboBox->currentIndex(), Qt::UserRole);
        model->setData(index, ActionTools::Resource::typeNames[comboBox->currentIndex()], Qt::DisplayRole);

        switch(static_cast<ActionTools::Resource::Type>(comboBox->currentIndex()))
        {
        case ActionTools::Resource::BinaryType:
        case ActionTools::Resource::TypeCount:
			model->setData(index, QIcon(QStringLiteral(":/images/binary.png")), Qt::DecorationRole);
            break;
        case ActionTools::Resource::TextType:
			model->setData(index, QIcon(QStringLiteral(":/images/text.png")), Qt::DecorationRole);
            break;
        case ActionTools::Resource::ImageType:
			model->setData(index, QIcon(QStringLiteral(":/images/image.png")), Qt::DecorationRole);
            break;
        }
    }
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}
