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

#include <QObject>

namespace Backend
{
    class BACKENDSHARED_EXPORT KeyboardInput : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(KeyboardInput)

    public:
        explicit KeyboardInput(QObject *parent = nullptr): QObject(parent) {}
        virtual ~KeyboardInput() = default;
    };

    class BACKENDSHARED_EXPORT KeyboardInputDummy final : public KeyboardInput
    {
        Q_OBJECT
        Q_DISABLE_COPY(KeyboardInputDummy)

    public:
        explicit KeyboardInputDummy(QObject *parent = nullptr): KeyboardInput(parent) {}
        ~KeyboardInputDummy() = default;
    };
}
