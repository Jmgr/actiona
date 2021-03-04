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

#include <QPoint>

#include <functional>
#include <memory>

namespace Backend
{
    class BACKENDSHARED_EXPORT Mouse final
    {
        Q_DISABLE_COPY_MOVE(Mouse)

    private:
        Mouse() = default;

    public:
        enum class Button
        {
            Left,
            Middle,
            Right,

            Count
        };

        // Input
        std::function<bool(Button button)> isButtonPressed;
        std::function<QPoint()> cursorPosition;

        // Output
        std::function<void(const QPoint &position)> setCursorPosition;
        std::function<void(Button button, bool press)> pressButton;
        std::function<void(int intensity)> wheel;

        friend std::unique_ptr<Mouse> std::make_unique<Mouse>();
    };

    // Dummy implementations
    static bool isButtonPressedDummy(Mouse::Button) { return false; }
    static QPoint cursorPositionDummy() { return {}; }
    static void setCursorPositionDummy(const QPoint &) {}
    static void pressButtonDummy(Mouse::Button, bool) {}
    static void wheelDummy(int) {}
}
