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

#include "itemlistview.h"

#include <QKeyEvent>
#include <QApplication>
#include <QPen>
#include <QPainter>

namespace ActionTools
{
	ItemListView::ItemListView(QWidget *parent)
	  : QListView(parent)
	{
		setDropIndicatorShown(false);
	}

	void ItemListView::keyPressEvent(QKeyEvent *event)
	{
		switch(event->key())
		{
		case Qt::Key_Delete:
			emit removeCurrentItem();
			break;
		case Qt::Key_PageUp:
			emit moveCurrentItem(true);
			break;
		case Qt::Key_PageDown:
			emit moveCurrentItem(false);
			break;
		default:
			break;
		}
	}

	void ItemListView::dragMoveEvent(QDragMoveEvent *event)
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

		QListView::dragMoveEvent(event);
	}

	void ItemListView::dragLeaveEvent(QDragLeaveEvent* event)
	{
		mDropIndicator = QRect(0, 0, 0, 0);

		QListView::dragLeaveEvent(event);
	}

	void ItemListView::dropEvent(QDropEvent* event)
	{
		mDropIndicator = QRect(0, 0, 0, 0);

		QListView::dropEvent(event);
	}

	void ItemListView::paintEvent(QPaintEvent* event)
	{
		QListView::paintEvent(event);

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
}
