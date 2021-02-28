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

#include <QObject>

namespace Backend
{
    Q_NAMESPACE

    namespace Mouse
    {
        Q_NAMESPACE

        enum Button
        {
            LeftButton,
            MiddleButton,
            RightButton,

            ButtonCount
        };
        Q_ENUM_NS(Button)
    }

    class MouseInput;
    class MouseOutput;
    class KeyboardInput;
    class KeyboardOutput;

    class BACKENDSHARED_EXPORT Backend final : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(Backend)

    public:
        explicit Backend(QObject *parent = nullptr);
        ~Backend();

        void autoselect();

        MouseInput *mouseInput();
        MouseOutput *mouseOutput();
        KeyboardInput *keyboardInput();
        KeyboardOutput *keyboardOutput();

    private:
        MouseInput *mMouseInput;
        MouseOutput *mMouseOutput;
        KeyboardInput *mKeyboardInput;
        KeyboardOutput *mKeyboardOutput;
    };
}
