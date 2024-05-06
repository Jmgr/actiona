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

#include "actiontools/screenshooter.hpp"
#include "actiontools/windowhandle.hpp"

#include <QApplication>
#include <QImage>
#include <QPainter>
#include <QScreen>

#include <limits>

namespace ActionTools
{
    QPixmap ScreenShooter::captureScreen(int screenIndex)
    {
        auto screens = QGuiApplication::screens();

        if(screenIndex < 0 || screenIndex >= screens.size())
            return {};

        auto screen = screens[screenIndex];

        return screen->grabWindow(0);
    }

    QList<std::pair<QPixmap, QRect>> ScreenShooter::captureScreens()
    {
       auto screens = QGuiApplication::screens();
        QList<std::pair<QPixmap, QRect>> result;

        for(int screenIndex = 0; screenIndex < screens.size(); ++screenIndex)
            result.append(std::make_pair(screens[screenIndex]->grabWindow(0), screens[screenIndex]->geometry()));

        return result;
    }

    QList<std::pair<QPixmap, QRect>> ScreenShooter::captureWindows(const QList<WindowHandle> &windows)
    {
        QList<std::pair<QPixmap, QRect>> result;

        for(const WindowHandle &window: windows)
        {
            if(!window.isValid())
                continue;

            const QRect &windowGeometry = window.rect();

            auto screen = QGuiApplication::screenAt(windowGeometry.center());
            if(!screen)
                continue;

            auto geometry = screen->geometry();
            auto localX = windowGeometry.x() - geometry.x();
            auto localY = windowGeometry.y() - geometry.y();

            result.append(std::make_pair(screen->grabWindow(0, localX, localY, windowGeometry.width(), windowGeometry.height()), windowGeometry));
        }

        return result;
    }

    QPixmap ScreenShooter::captureWindow(WindowHandle window)
    {
        if(!window.isValid())
            return {};

        const QRect &windowGeometry = window.rect();

        auto screen = QGuiApplication::screenAt(windowGeometry.center());
        if(!screen)
            return {};

        auto geometry = screen->geometry();
        auto localX = windowGeometry.x() - geometry.x();
        auto localY = windowGeometry.y() - geometry.y();

        return screen->grabWindow(0, localX, localY, windowGeometry.width(), windowGeometry.height());
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
        auto screen = QGuiApplication::screenAt(rect.center());
        if(!screen)
            return {};

        auto geometry = screen->geometry();
        auto localX = rect.x() - geometry.x();
        auto localY = rect.y() - geometry.y();

        return screen->grabWindow(0, localX, localY, rect.width(), rect.height());
    }
}
