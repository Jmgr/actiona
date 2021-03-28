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
            QStringLiteral("logout"),
            [](bool){}
        };
        Feature<void(bool force)> restart
        {
            QStringLiteral("restart"),
            [](bool){}
        };
        Feature<void(bool force)> shutdown
        {
            QStringLiteral("shutdown"),
            [](bool){}
        };
        Feature<void(bool force)> suspend
        {
            QStringLiteral("suspend"),
            [](bool){}
        };
        Feature<void(bool force)> hibernate
        {
            QStringLiteral("hibernate"),
            [](bool){}
        };
        Feature<void()> lockScreen
        {
            QStringLiteral("lockScreen"),
            []{}
        };
        Feature<void()> startScreenSaver
        {
            QStringLiteral("startScreenSaver"),
            []{}
        };
        Feature<QString()> getUsername
        {
            QStringLiteral("getUsername"),
            []{ return QString{}; }
        };
        Feature<void(const QString &sourceFilepath, const QString &destinationFilepath, const FileOperationParameters &parameters)> copyFiles
        {
            QStringLiteral("copyFiles"),
            [](const QString &, const QString &, const FileOperationParameters &){}
        };
        Feature<void(const QString &sourceFilepath, const QString &destinationFilepath, const FileOperationParameters &parameters)> moveFiles
        {
            QStringLiteral("moveFiles"),
            [](const QString &, const QString &, const FileOperationParameters &){}
        };
        Feature<void(const QString &sourceFilepath, const QString &destinationFilepath, const FileOperationParameters &parameters)> renameFiles
        {
            QStringLiteral("renameFiles"),
            [](const QString &, const QString &, const FileOperationParameters &){}
        };
        Feature<void(const QString &filepath, const FileOperationParameters &parameters)> removeFiles
        {
            QStringLiteral("removeFiles"),
            [](const QString &, const FileOperationParameters &){}
        };
    };
}
