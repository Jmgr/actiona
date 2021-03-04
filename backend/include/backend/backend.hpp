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

#include <QString>

#include <stdexcept>
#include <memory>
#include <set>

namespace Backend
{
    class Mouse;
    class Keyboard;

    class BACKENDSHARED_EXPORT BackendError
    {
    public:
        BackendError(const QString &message): mMessage(message) {}
        BackendError(int errorCode): mMessage(QStringLiteral("call failed with error code %1").arg(errorCode)) {}
        BackendError(): mMessage(QStringLiteral("call failed")) {}

        const QString &what() const noexcept { return mMessage; }

    private:
        QString mMessage;
    };

    class BACKENDSHARED_EXPORT Instance final
    {
        Q_DISABLE_COPY(Instance)

    private:
        Instance();

    public:
        ~Instance();

        static void initialize() { get(); }
        // Release all buttons and keys.
        static void releaseAll() { get().instReleaseAll(); }
        static const Mouse &mouse() { return *get().mMouse.get(); }
        static const Keyboard &keyboard() { return *get().mKeyboard.get(); }

    private:
        static Instance &get();

        void useDummy();
        void instReleaseAll();

        std::unique_ptr<Mouse> mMouse;
        std::unique_ptr<Keyboard> mKeyboard;
        std::set<int> mPressedButtons;
        std::set<std::pair<QString, bool>> mPressedKeys;
    };
}
