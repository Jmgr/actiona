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

#include "scripttreeview.h"
#include "scriptmodel.h"

#include <QMimeData>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QPushButton>
#include <QApplication>
#include <QDrag>

ScriptTreeView::ScriptTreeView(QWidget *parent)
    : QTreeView(parent)
{
	setDropIndicatorShown(false);
}

void ScriptTreeView::startDrag(Qt::DropActions supportedActions)
{
	QModelIndexList indexes = selectedIndexes();
	for(int i = indexes.count() - 1 ; i >= 0; --i)
	{
		const QModelIndex &index = indexes.at(i);
		if(!(index.flags() & Qt::ItemIsDragEnabled))
			indexes.removeAt(i);
	}

	if(indexes.count() == 0)
		return;

	QMimeData *data = model()->mimeData(indexes);
	if(!data)
		return;

	QRect rect;
	auto drag = new QDrag(this);
	drag->setMimeData(data);

	Qt::DropAction finalDefaultDropAction = Qt::IgnoreAction;
	if(defaultDropAction() != Qt::IgnoreAction && (supportedActions & defaultDropAction()))
		finalDefaultDropAction = defaultDropAction();
	else if(supportedActions & Qt::CopyAction && dragDropMode() != QAbstractItemView::InternalMove)
		finalDefaultDropAction = Qt::CopyAction;

	drag->exec(supportedActions, finalDefaultDropAction);
}

void ScriptTreeView::dragMoveEvent(QDragMoveEvent *event)
{
	QModelIndex index = indexAt(event->pos());
	bool afterLastItem = false;

	if(index.isValid())
	{
		mDropIndicator = visualRect(index);

		if(event->pos().y() > mDropIndicator.top() + mDropIndicator.height() / 2)
		{
			index = model()->index(index.row() + 1, 0, QModelIndex());
			if(index.isValid())
				mDropIndicator = visualRect(index);
			else
				afterLastItem = true;
		}
	}
	else
		afterLastItem = true;

	if(afterLastItem)
	{
		index = model()->index(model()->rowCount() - 1, 0, QModelIndex());
		mDropIndicator = visualRect(index);
		mDropIndicator = mDropIndicator.translated(0, mDropIndicator.height());
	}

    QTreeView::dragMoveEvent(event);
}

void ScriptTreeView::dragLeaveEvent(QDragLeaveEvent* event)
{
	mDropIndicator = QRect(0, 0, 0, 0);

    QTreeView::dragLeaveEvent(event);
}

void ScriptTreeView::dropEvent(QDropEvent* event)
{
	mDropIndicator = QRect(0, 0, 0, 0);

    QTreeView::dropEvent(event);
}

void ScriptTreeView::paintEvent(QPaintEvent* event)
{
    QTreeView::paintEvent(event);

	if(!mDropIndicator.size().isEmpty())
	{
		QPalette p = QApplication::palette();
		QPen pen(p.color(QPalette::Highlight), 3, Qt::DotLine);
		QPainter painter(viewport());
		painter.setPen(pen);

		mDropIndicator.translate(0, -1);
		mDropIndicator.setLeft(0);
		mDropIndicator.setRight(viewport()->rect().size().width());

		painter.drawLine(mDropIndicator.topLeft(), mDropIndicator.topRight());
	}
}

