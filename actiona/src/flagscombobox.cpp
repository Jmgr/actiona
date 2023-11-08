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

#include "flagscombobox.hpp"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QListView>
#include <QStyledItemDelegate>
#include <QWheelEvent>

inline bool isPowerOfTwo(unsigned int number)
{
    return (!(number & (number - 1)) && number);
}

FlagsComboBox::FlagsComboBox(QWidget *parent):
    QComboBox(parent)
{
    setModel(new QStandardItemModel(this));
    setEditable(false);
    setItemDelegate(new QStyledItemDelegate(this));

    auto item = new QStandardItem;
    item->setFlags(Qt::ItemIsEnabled);
    standardItemModel()->appendRow(item);

    connect(view(), &QListView::pressed, this, [this](const QModelIndex &index)
    {
        auto item = standardItemModel()->itemFromIndex(index);

        if(!(item->flags() & Qt::ItemIsUserCheckable))
            return;

        if(item->checkState() == Qt::Checked)
            item->setCheckState(Qt::Unchecked);
        else
            item->setCheckState(Qt::Checked);
    });

    connect(standardItemModel(), &QStandardItemModel::dataChanged, this, [this]()
    {
        updateText();

        emit flagsChanged(flags());
    });
}

FlagsComboBox::~FlagsComboBox() = default;

void FlagsComboBox::setAllCheckedText(const QString &text)
{
    m_allCheckedText = text;

    updateText();
}

void FlagsComboBox::setNoneCheckedText(const QString &text)
{
    m_noneCheckedText = text;

    updateText();
}

void FlagsComboBox::setFlags(unsigned int flags)
{
    int rowCount = standardItemModel()->rowCount();

    for(int i = 1; i < rowCount; i++)
    {
        auto item = standardItemModel()->item(i);

        item->setCheckState((flags & item->data().toUInt()) ? Qt::Checked : Qt::Unchecked);
    }
}

void FlagsComboBox::addFlag(const QString &label, unsigned int flag, bool checked)
{
    Q_ASSERT(isPowerOfTwo(flag));

    auto* item = new QStandardItem(label);
    item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    item->setData(flag);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

    standardItemModel()->appendRow(item);

    updateText();
}

unsigned int FlagsComboBox::flags() const
{
    unsigned int result{};
    int rowCount = standardItemModel()->rowCount();

    for(int i = 1; i < rowCount; i++)
    {
        auto item = standardItemModel()->item(i);

        if(item->checkState() == Qt::Checked)
            result |= item->data().toUInt();
    }

    return result;
}

bool FlagsComboBox::hasFlag(unsigned int flag) const
{
    Q_ASSERT(isPowerOfTwo(flag));

    return flags() & flag;
}

void FlagsComboBox::wheelEvent(QWheelEvent *event)
{
    event->ignore();
}

Qt::CheckState FlagsComboBox::globalCheckState()
{
    int rowCount = standardItemModel()->rowCount();
    int checkedItemCount = 0;
    int uncheckedItemCount = 0;

    if(rowCount == 1)
        return Qt::Unchecked;

    for(int i = 1; i < rowCount; i++)
    {
        if(standardItemModel()->item(i)->checkState() == Qt::Checked)
            checkedItemCount++;
        else if(standardItemModel()->item(i)->checkState() == Qt::Unchecked)
            uncheckedItemCount++;
    }

    if(checkedItemCount == (rowCount - 1))
        return Qt::Checked;
    else if(uncheckedItemCount == (rowCount - 1))
        return Qt::Unchecked;
    else
        return Qt::PartiallyChecked;
}

void FlagsComboBox::updateText()
{
    QString text;

    switch(globalCheckState())
    {
    case Qt::Checked:
        text = m_allCheckedText;
        break;

    case Qt::Unchecked:
        text = m_noneCheckedText;
        break;

    case Qt::PartiallyChecked:
        for(int i = 1; i < standardItemModel()->rowCount(); i++)
        {
            if(standardItemModel()->item(i)->checkState() == Qt::Checked)
            {
                if(!text.isEmpty())
					text+= QStringLiteral(", ");

                text += standardItemModel()->item(i)->text();
            }
        }
        break;
    }

    setItemText(0, text);
    setCurrentIndex(0);
    setToolTip(text);
}

QStandardItemModel *FlagsComboBox::standardItemModel()
{
    return static_cast<QStandardItemModel *>(model());
}

const QStandardItemModel *FlagsComboBox::standardItemModel() const
{
    return static_cast<const QStandardItemModel *>(model());
}
