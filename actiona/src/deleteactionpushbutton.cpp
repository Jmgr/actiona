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

	Contact: jmgr@jmgr.info
*/

#include "deleteactionpushbutton.hpp"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPersistentModelIndex>
#include <QMimeData>

DeleteActionPushButton::DeleteActionPushButton(QWidget *parent)
	: QPushButton(parent)
{
	setAcceptDrops(true);
}

void DeleteActionPushButton::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasFormat(QStringLiteral("application/actiona.action")))
		event->acceptProposedAction();
}

void DeleteActionPushButton::dropEvent(QDropEvent *event)
{
	QByteArray encodedData = event->mimeData()->data(QStringLiteral("application/actiona.action"));
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	QList<int> rowIdList;
	while(!stream.atEnd())
	{
		QString text;
		stream >> text;
		rowIdList << text.toInt();
	}

	emit actionsDropped(rowIdList);

	event->setDropAction(Qt::IgnoreAction);
	event->acceptProposedAction();
}
