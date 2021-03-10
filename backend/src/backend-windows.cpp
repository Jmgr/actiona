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

#include "backend/backend-windows.hpp"

#include <QString>
#include <QObject>

#include <Windows.h>

namespace Backend
{
    QString lastErrorString()
    {
        auto lastError = GetLastError();
        LPTSTR message;

        if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                      0, lastError, 0, reinterpret_cast<LPTSTR>(&message), 0, 0))
            return QObject::tr("Error: failed to get last error string for error %1. GetLastError returned %2.").arg(lastError).arg(GetLastError());

        auto result = QString::fromWCharArray(message).trimmed();

        LocalFree(message);

        return result;
    }
}
