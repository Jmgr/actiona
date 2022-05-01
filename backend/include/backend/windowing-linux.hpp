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

#include "backend/windowing.hpp"

namespace Backend
{
    class BACKENDSHARED_EXPORT FullscreenScreenshooterLinux final : public Screenshooter
    {
        Q_OBJECT
        Q_DISABLE_COPY(FullscreenScreenshooterLinux)

    public:
        explicit FullscreenScreenshooterLinux(QObject *parent = nullptr): Screenshooter(parent) {}
        ~FullscreenScreenshooterLinux() override = default;

        void capture(bool includeCursor, bool includeFrame, bool flash) override;
    };

    class BACKENDSHARED_EXPORT AreaScreenshooterLinux final : public Screenshooter
    {
        Q_OBJECT
        Q_DISABLE_COPY(AreaScreenshooterLinux)

    public:
        explicit AreaScreenshooterLinux(QObject *parent = nullptr): Screenshooter(parent) {}
        ~AreaScreenshooterLinux() override = default;

        void capture(bool includeCursor, bool includeFrame, bool flash) override;
    };

    class BACKENDSHARED_EXPORT WindowScreenshooterLinux final : public Screenshooter
    {
        Q_OBJECT
        Q_DISABLE_COPY(WindowScreenshooterLinux)

    public:
        explicit WindowScreenshooterLinux(QObject *parent = nullptr): Screenshooter(parent) {}
        ~WindowScreenshooterLinux() override = default;

        void capture(bool includeCursor, bool includeFrame, bool flash) override;
    };

    bool checkFullscreenScreenshooterGnome();
    bool checkAreaScreenshooterGnome();
    bool checkWindowScreenshooterGnome();

    Screenshooter* createFullscreenScreenshooterLinux(QObject *parent);
    Screenshooter* createAreaScreenshooterLinux(QObject *parent);
    Screenshooter* createWindowScreenshooterLinux(QObject *parent);
}
