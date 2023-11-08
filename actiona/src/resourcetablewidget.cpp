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

#include "resourcetablewidget.hpp"

#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QUrl>
#include <QKeyEvent>
#include <QMimeData>

ResourceTableWidget::ResourceTableWidget(QWidget *parent)
   : QTableWidget(parent)
{
}

QString ResourceTableWidget::checkResourceName(const QString &originalName) const
{
    QString resourceName = originalName;

    if(containsResource(resourceName))
    {
        for(int itemNumber = 1; ; ++itemNumber)
        {
			resourceName = originalName + QStringLiteral("_%1").arg(itemNumber);

            if(!containsResource(resourceName))
                break;
        }
    }

    return resourceName;
}

bool ResourceTableWidget::containsResource(const QString &name) const
{
    for(int row = 0; row < rowCount(); ++row)
    {
        QTableWidgetItem *currentItem = item(row, 0);
        if(currentItem && currentItem->text() == name)
            return  true;
    }

    return false;
}

void ResourceTableWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void ResourceTableWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

bool ResourceTableWidget::dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action)
{
    Q_UNUSED(row)
    Q_UNUSED(column)

    if(action == Qt::IgnoreAction)
        return true;

    if(!data->hasUrls())
        return false;

    QStringList droppedFiles;
    const auto urls = data->urls();
    for(const QUrl &url: urls)
    {
        if(!url.isLocalFile())
            continue;

        droppedFiles << url.toLocalFile();
    }

    emit filesDropped(droppedFiles);

    return true;
}

void ResourceTableWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
        emit removeSelection();
}


