/*
    Actiona
    Copyright (C) 2005-2017 Jonathan Mercier-Ganady

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

#include "filterlineedit.h"

#include <QPaintEvent>
#include <QPainter>

FilterLineEdit::FilterLineEdit(QWidget *parent)
    : QLineEdit(parent),
      mIcon(":/icons/look.png")
{
    Q_ASSERT(!mIcon.isNull());

    setPlaceholderText(tr("Filter"));
    setClearButtonEnabled(true);
    setStyleSheet("padding-left: 14px;"
                  "padding-right: 5px;");
}

void FilterLineEdit::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);

    QPainter painter(this);
    painter.drawImage(3, 3, mIcon);
}
