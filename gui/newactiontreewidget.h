/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef NEWACTIONTREEWIDGET_H
#define NEWACTIONTREEWIDGET_H

#include <QTreeWidget>

class NewActionTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	enum NewActionRole
	{
		ActionIdRole = Qt::UserRole
	};

	explicit NewActionTreeWidget(QWidget *parent = 0);

protected:
	Qt::DropActions supportedDropActions() const;
	QMimeData *mimeData(const QList<QTreeWidgetItem *> items) const;

private:
	Q_DISABLE_COPY(NewActionTreeWidget)
};

#endif // NEWACTIONTREEWIDGET_H
