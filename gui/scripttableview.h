/*
	Actiona
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef SCRIPTTABLEVIEW_H
#define SCRIPTTABLEVIEW_H

#include <QTableView>

class ScriptTableView : public QTableView
{
	Q_OBJECT

public:
	explicit ScriptTableView(QWidget *parent = 0);

protected:
	void startDrag(Qt::DropActions supportedActions);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragLeaveEvent(QDragLeaveEvent* event);
	void dropEvent(QDropEvent* event);
	void paintEvent(QPaintEvent* event);

private:
	QRect mDropIndicator;
};

#endif // SCRIPTTABLEVIEW_H
