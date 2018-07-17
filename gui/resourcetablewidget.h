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

#pragma once

#include <QTableWidget>

class ResourceTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit ResourceTableWidget(QWidget *parent = nullptr);

    QString checkResourceName(const QString &originalName) const;
    bool containsResource(const QString &name) const;

protected:
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    bool dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    
signals:
    void filesDropped(const QStringList &filenames);
    void removeSelection();
};

