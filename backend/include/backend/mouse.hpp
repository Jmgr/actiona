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

    Contact: jmgr@jmgr.info
*/

#pragma once

#include "backend/backend_global.hpp"
#include "backend/backend.hpp"
#include "backend/feature.hpp"

#include <QPoint>

namespace Backend
{
    class Capabilities;

    class BACKENDSHARED_EXPORT Mouse final
    {
        Q_DISABLE_COPY(Mouse)

    public:
        enum class Button
        {
            Left,
            Middle,
            Right,

            Count
        };

        Mouse(Capabilities &caps);

        Feature<bool(Button button)> isButtonPressed
        {
            QStringLiteral("isButtonPressed")
        };
        Feature<QPoint()> cursorPosition
        {
            QStringLiteral("cursorPosition")
        };
        Feature<void(const QPoint &position)> setCursorPosition
        {
            QStringLiteral("setCursorPosition")
        };
        Feature<void(Button button, bool press)> pressButton
        {
            QStringLiteral("pressButton")
        };
        Feature<void(int intensity)> rotateWheel
        {
            QStringLiteral("rotateWheel")
        };
    };
}
