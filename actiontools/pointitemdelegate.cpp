/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#include "pointitemdelegate.h"

#include <QSpinBox>

PointItemDelegate::PointItemDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

QWidget *PointItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)
	Q_UNUSED(index)

	QSpinBox *spinBox = new QSpinBox(parent);
	spinBox->setMinimum(0);
	spinBox->setMaximum(INT_MAX);
	return spinBox;
}

void PointItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
	spinBox->setValue(index.data().toInt());
}

void PointItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
	model->setData(index, spinBox->value());
}
