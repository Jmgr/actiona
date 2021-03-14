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

#include <functional>
#include <memory>

namespace Backend
{
    class BACKENDSHARED_EXPORT System final
    {
        Q_DISABLE_COPY(System)

    private:
        System() = default;

    public:
        struct FileOperationParameters
        {
            bool noErrorDialog{};
            bool noConfirmDialog{};
            bool noProgressDialog{};
            bool allowUndo{};
            bool createDestinationDirectory{true};
        };

        std::function<void(bool force)> logout;
        std::function<void(bool force)> restart;
        std::function<void(bool force)> shutdown;
        std::function<void(bool force)> suspend;
        std::function<void(bool force)> hibernate;
        std::function<void()> lockScreen;
        std::function<void()> startScreenSaver;
        std::function<QString()> getUsername;
        std::function<void(const QString &sourceFilepath, const QString &destinationFilepath, const FileOperationParameters &parameters)> copyFiles;
        std::function<void(const QString &sourceFilepath, const QString &destinationFilepath, const FileOperationParameters &parameters)> moveFiles;
        std::function<void(const QString &sourceFilepath, const QString &destinationFilepath, const FileOperationParameters &parameters)> renameFiles;
        std::function<void(const QString &filepath, const FileOperationParameters &parameters)> removeFiles;

        friend std::unique_ptr<System> std::make_unique<System>();
    };

    // Dummy implementations
    static void logoutDummy(bool) {}
    static void restartDummy(bool) {}
    static void shutdownDummy(bool) {}
    static void suspendDummy(bool) {}
    static void hibernateDummy(bool) {}
    static void lockScreenDummy() {}
    static void startScreenSaverDummy() {}
    static QString getUsernameDummy() { return {}; }
    static void copyFilesDummy(const QString &, const QString &, const System::FileOperationParameters &) {}
    static void moveFilesDummy(const QString &, const QString &, const System::FileOperationParameters &) {}
    static void renameFilesDummy(const QString &, const QString &, const System::FileOperationParameters &) {}
    static void removeFilesDummy(const QString &, const System::FileOperationParameters &) {}
}
