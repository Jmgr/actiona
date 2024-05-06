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

#include "actiontools/targetwindow.hpp"

#include <QMouseEvent>
#include <QCursor>
#include <QPainter>
#include <QMessageBox>
#include <QApplication>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace ActionTools
{
    TargetWindow::TargetWindow()
        : QWidget(nullptr, Qt::Widget
                  | Qt::Window
                  | Qt::FramelessWindowHint
                  | Qt::WindowStaysOnTopHint
                  | Qt::X11BypassWindowManagerHint
                  | Qt::Tool
                  | Qt::NoDropShadowWindowHint
                  | Qt::BypassWindowManagerHint
                  )
    {
        setWindowModality(Qt::ApplicationModal);
        setAttribute(Qt::WA_TranslucentBackground);
        setMinimumSize(1, 1);
        setCursor(Qt::CrossCursor);

        connect(&mUpdateTimer, &QTimer::timeout, this, &TargetWindow::update);
    }

    TargetWindow::~TargetWindow()
    {
    }

    void TargetWindow::keyPressEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_Escape)
            close();

        event->ignore();
    }

    void TargetWindow::mousePressEvent(QMouseEvent *event)
    {
        mMouseClickPosition = event->globalPosition().toPoint();
        mMousePressed = true;
    }

    void TargetWindow::mouseReleaseEvent(QMouseEvent *event)
    {
        Q_UNUSED(event)

        mMousePressed = false;

        mouseButtonReleased();
        close();
    }

    void TargetWindow::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event)

        QPainter painter(this);

#ifdef Q_OS_UNIX
        if(mMousePressed)
            painter.fillRect(0, 0, width(), height(), QBrush(Qt::black));
#endif

#ifdef Q_OS_WIN
        if(!mMousePressed)
        {
            painter.fillRect(0, 0, width() - 1, height() - 1, QBrush(QColor(0, 0, 0, 1)));

            return;
        }

        painter.fillRect(0, 0, width() - 1, height() - 1, QBrush(QColor(0, 0, 255, 32)));

        painter.setPen(QColor(Qt::gray));
        painter.drawRect(0, 0, width() - 1, height() - 1);

        painter.setBrush(Qt::SolidPattern);
        painter.setPen(QColor(Qt::black));

        QFont font = painter.font();
        font.setPointSize(18);
        painter.setFont(font);

        painter.drawText(rect(), Qt::AlignCenter, QString::number(width()) + QStringLiteral(" x ") + QString::number(height()));
#endif
    }

    void TargetWindow::showEvent(QShowEvent *event)
    {
        Q_UNUSED(event)

#ifdef Q_OS_WIN
        resize(100, 100);
#endif
#ifdef Q_OS_UNIX
        resize(1, 1);
#endif

        mUpdateTimer.start(1);

        mMousePressed = false;
        mResult = QRect();
    }

    void TargetWindow::hideEvent(QHideEvent *event)
    {
        Q_UNUSED(event)

        mUpdateTimer.stop();

        emit rectangleSelected(mResult);
    }

    void TargetWindow::update()
    {
#ifdef Q_OS_UNIX
        if(mMousePressed)
            setMask(QRegion(rect()).subtracted(QRegion(QRect(2, 2, width() - 4, height() - 4))));
#endif

#ifdef Q_OS_WIN
        if(GetKeyState(VK_ESCAPE) < 0)
            close();
#endif

        if(mMousePressed)
        {
            QPoint cursorPosition = QCursor::pos();
            QPoint topLeft(qMin(mMouseClickPosition.x(), cursorPosition.x()),
                           qMin(mMouseClickPosition.y(), cursorPosition.y()));
            QPoint bottomRight(qMax(mMouseClickPosition.x(), cursorPosition.x()),
                           qMax(mMouseClickPosition.y(), cursorPosition.y()));
            QSize size(bottomRight.x() - topLeft.x(),
                       bottomRight.y() - topLeft.y());

            setGeometry(QRect(topLeft, size));
        }
        else
            move(QCursor::pos() - QPoint(width() / 2, height() / 2));
    }

    void TargetWindow::mouseButtonReleased()
    {
        if(width() < 1 || height() < 1)
            return;

        mResult = QRect(pos(), size());

#ifdef Q_OS_UNIX
        mResult = mResult.marginsRemoved(QMargins(2, 2, 2, 2));
#endif

        if(mResult.isEmpty())
            mResult = {};
    }
}
