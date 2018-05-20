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

#include "screenshooter.h"
#include "windowhandle.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QScreen>

#include <limits>

namespace ActionTools
{
    QPixmap ScreenShooter::captureScreen(int screenIndex)
    {
        QDesktopWidget *desktop = QApplication::desktop();

        if(screenIndex < 0 || screenIndex >= desktop->screenCount())
            return QPixmap();

        const QRect &screenGeometry = desktop->screenGeometry(screenIndex);

        return QGuiApplication::primaryScreen()->grabWindow(0, screenGeometry.x(), screenGeometry.y(), screenGeometry.width(), screenGeometry.height());
    }

    QList<std::pair<QPixmap, QRect>> ScreenShooter::captureScreens()
    {
        QDesktopWidget *desktop = QApplication::desktop();
        QList<std::pair<QPixmap, QRect>> result;

        for(int screenIndex = 0; screenIndex < desktop->screenCount(); ++screenIndex)
        {
            const QRect &screenGeometry = desktop->screenGeometry(screenIndex);

            result.append(std::make_pair(QGuiApplication::primaryScreen()->grabWindow(0, screenGeometry.x(), screenGeometry.y(), screenGeometry.width(), screenGeometry.height()), screenGeometry));
        }

        return result;
    }

    QList<std::pair<QPixmap, QRect>> ScreenShooter::captureWindows(const QList<WindowHandle> &windows)
    {
        QDesktopWidget *desktop = QApplication::desktop();
        QList<std::pair<QPixmap, QRect>> result;

        for(const WindowHandle &window: windows)
        {
            if(!window.isValid())
                continue;

            const QRect &windowGeometry = window.rect();

            result.append(std::make_pair(QGuiApplication::primaryScreen()->grabWindow(desktop->winId(), windowGeometry.x(), windowGeometry.y(), windowGeometry.width(), windowGeometry.height()), windowGeometry));
        }

        return result;
    }

    QPixmap ScreenShooter::captureWindow(WindowHandle window)
    {
        if(!window.isValid())
            return QPixmap();

        const QRect &windowGeometry = window.rect();

        return QGuiApplication::primaryScreen()->grabWindow(0, windowGeometry.x(), windowGeometry.y(), windowGeometry.width(), windowGeometry.height());
    }

    QPixmap ScreenShooter::captureAllScreens()
    {
        const auto &screens = captureScreens();
        QRect resultRect;
        QPoint minimalTopLeft(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

        for(const auto &screen: screens)
        {
            const QRect &screenRect = screen.second;

            resultRect = resultRect.united(screenRect);

            if(minimalTopLeft.x() > screenRect.x())
                minimalTopLeft.setX(screenRect.x());
            if(minimalTopLeft.y() > screenRect.y())
                minimalTopLeft.setY(screenRect.y());
        }

        QImage result(resultRect.width(), resultRect.height(), QImage::Format_RGB32);
        result.fill(Qt::black);

        {
            QPainter painter(&result);

            for(const auto &screen: screens)
            {
                const QRect &screenRect = screen.second;

                painter.drawPixmap(screenRect.x() - minimalTopLeft.x(), screenRect.y() - minimalTopLeft.y(), screen.first);
            }
        }

        return QPixmap::fromImage(result);
    }

    QPixmap ScreenShooter::captureRect(const QRect &rect)
    {
        return QGuiApplication::primaryScreen()->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
    }
}
