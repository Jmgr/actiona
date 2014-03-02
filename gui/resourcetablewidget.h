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

#ifndef RESOURCETABLEWIDGET_H
#define RESOURCETABLEWIDGET_H

#include <QTableWidget>

class ResourceTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit ResourceTableWidget(QWidget *parent = 0);

    QString checkResourceName(const QString &originalName) const;
    bool containsResource(const QString &name) const;

protected:
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual bool dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action);
    virtual void keyReleaseEvent(QKeyEvent *event);
    
signals:
    void filesDropped(const QStringList &filenames);
    void removeSelection();
};

#endif // RESOURCETABLEWIDGET_H
