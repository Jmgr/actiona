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

#ifndef DELETEACTIONLABEL_H
#define DELETEACTIONLABEL_H

#include <QPushButton>

class DeleteActionPushButton : public QPushButton
{
	Q_OBJECT

public:
	explicit DeleteActionPushButton(QWidget *parent = 0);

signals:
	void actionsDropped(QList<int> rows);

private:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

	Q_DISABLE_COPY(DeleteActionPushButton)
};

#endif // DELETEACTIONLABEL_H
