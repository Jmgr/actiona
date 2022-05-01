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

namespace Backend
{
    class Capabilities;

    class BACKENDSHARED_EXPORT System final
    {
        Q_DISABLE_COPY(System)

    public:
        struct FileOperationParameters
        {
            bool noErrorDialog{};
            bool noConfirmDialog{};
            bool noProgressDialog{};
            bool allowUndo{};
            bool createDestinationDirectory{true};
        };

        System(Capabilities &caps);

        Feature<void(bool force)> logout
        {
            QStringLiteral("logout")
        };
        Feature<void(bool force)> restart
        {
            QStringLiteral("restart")
        };
        Feature<void(bool force)> shutdown
        {
            QStringLiteral("shutdown")
        };
        Feature<void(bool force)> suspend
        {
            QStringLiteral("suspend")
        };
        Feature<void(bool force)> hibernate
        {
            QStringLiteral("hibernate")
        };
        Feature<void()> lockScreen
        {
            QStringLiteral("lockScreen")
        };
        Feature<void()> startScreenSaver
        {
            QStringLiteral("startScreenSaver")
        };
        Feature<QString()> getUsername
        {
            QStringLiteral("getUsername")
        };
        Feature<void(const QString &sourceFilepath, const QString &destinationFilepath, const FileOperationParameters &parameters)> copyFiles
        {
            QStringLiteral("copyFiles")
        };
        Feature<void(const QString &sourceFilepath, const QString &destinationFilepath, const FileOperationParameters &parameters)> moveFiles
        {
            QStringLiteral("moveFiles")
        };
        Feature<void(const QString &sourceFilepath, const QString &destinationFilepath, const FileOperationParameters &parameters)> renameFiles
        {
            QStringLiteral("renameFiles")
        };
        Feature<void(const QString &filepath, const FileOperationParameters &parameters)> removeFiles
        {
            QStringLiteral("removeFiles")
        };
    };
}
