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

#include "actiontools/imagelabel.hpp"

#include <QPainter>
#include <QPaintEvent>

namespace ActionTools
{
    ImageLabel::ImageLabel(QWidget *parent)
      : QWidget(parent)
    {
    }

    void ImageLabel::setPixmap(const QPixmap &pixmap)
    {
        mPixmap = pixmap;

        update();
    }

    void ImageLabel::paintEvent(QPaintEvent *event)
    {
        QWidget::paintEvent(event);

        if(mPixmap.isNull())
            return;

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QSize pixmapSize = mPixmap.size();
        pixmapSize.scale(event->rect().size(), Qt::KeepAspectRatio);

        QPixmap scaledPixmap = mPixmap.scaled(pixmapSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QPoint drawPosition(event->rect().width() / 2 - scaledPixmap.width() / 2,
                            event->rect().height() / 2 - scaledPixmap.height() / 2);

        painter.drawPixmap(drawPosition, scaledPixmap);
    }
}
