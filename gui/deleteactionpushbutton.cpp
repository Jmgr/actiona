/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#include "deleteactionpushbutton.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QPersistentModelIndex>

DeleteActionPushButton::DeleteActionPushButton(QWidget *parent)
	: QPushButton(parent)
{
	setAcceptDrops(true);
}

void DeleteActionPushButton::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasFormat("application/actionaz.action"))
		event->acceptProposedAction();
}

void DeleteActionPushButton::dropEvent(QDropEvent *event)
{
	QByteArray encodedData = event->mimeData()->data("application/actionaz.action");
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
