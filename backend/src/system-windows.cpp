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

#include "backend/system-windows.hpp"
#include "backend/backend.hpp"
#include "backend/backend-windows.hpp"

#include <QDir>

#include <Windows.h>
#include <powrprof.h>
#include <lmcons.h>
#include <Shellapi.h>

#include <array>

namespace Backend
{
    void logoutWindows(bool force)
    {
        if(!ExitWindowsEx(EWX_LOGOFF | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER))
            throw BackendError(lastErrorString());
    }

    void restartWindows(bool force)
    {
        if(!ExitWindowsEx(EWX_REBOOT | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER))
            throw BackendError(lastErrorString());
    }

    void shutdownWindows(bool force)
    {
        if(!ExitWindowsEx(EWX_POWEROFF | (force ? EWX_FORCE : 0), SHTDN_REASON_MAJOR_OTHER | SHTDN_REASON_MINOR_OTHER))
            throw BackendError(lastErrorString());
    }

    void suspendWindows(bool force)
    {
        if(!SetSuspendState(false, force, true))
            throw BackendError(lastErrorString());
    }

    void hibernateWindows(bool force)
    {
        if(!SetSuspendState(true, force, false))
            throw BackendError(lastErrorString());
    }

    void lockScreenWindows()
    {
        if(!LockWorkStation())
            throw BackendError(lastErrorString());
    }

    void startScreenSaverWindows()
    {
        SendMessage(GetDesktopWindow(), WM_SYSCOMMAND, SC_SCREENSAVE, 0);
    }

    QString getUsernameWindows()
    {
        std::array<TCHAR, UNLEN+1> buffer;
        DWORD size = buffer.size();
        if(!GetUserName(buffer.data(), &size))
            throw BackendError(lastErrorString());

        return QString::fromWCharArray(buffer.data());
    }

    QString getErrorString(int error)
    {
        switch(error)
        {
        case ERROR_SUCCESS:
            return QString();
        case ERROR_FILE_NOT_FOUND:
            return QObject::tr("File not found");
        case ERROR_PATH_NOT_FOUND:
            return QObject::tr("Path not found");
        case ERROR_ACCESS_DENIED:
            return QObject::tr("Access denied");
        case ERROR_SHARING_VIOLATION:
            return QObject::tr("This file is used by another process");
        case ERROR_DISK_FULL:
            return QObject::tr("The disk is full");
        case ERROR_FILE_EXISTS:
        case ERROR_ALREADY_EXISTS:
            return QObject::tr("The file already exists");
        case ERROR_INVALID_NAME:
            return QObject::tr("Invalid name");
        case ERROR_CANCELLED:
            return QObject::tr("Canceled");
        default:
            return QObject::tr("Unknown error (%1)").arg(error);
        }
    }

    void copyFilesWindows(const QString &sourceFilepath, const QString &destinationFilepath, const System::FileOperationParameters &parameters)
    {
        QDir sourceDir(sourceFilepath);
        QDir destinationDir(destinationFilepath);

        std::wstring wideSource = QDir::toNativeSeparators(sourceDir.absolutePath()).toStdWString();
        wideSource += L'\0';

        std::wstring wideDestination = QDir::toNativeSeparators(destinationDir.absolutePath()).toStdWString();
        wideDestination += L'\0';

        SHFILEOPSTRUCT shFileOpStruct;
        shFileOpStruct.hwnd = 0;
        shFileOpStruct.wFunc = FO_COPY;
        shFileOpStruct.pFrom = wideSource.c_str();
        shFileOpStruct.pTo = wideDestination.c_str();
        shFileOpStruct.fFlags = 0;
        shFileOpStruct.fAnyOperationsAborted = false;
        shFileOpStruct.lpszProgressTitle = 0;
        shFileOpStruct.hNameMappings = 0;

        if(parameters.noErrorDialog)
            shFileOpStruct.fFlags |= FOF_NOERRORUI;
        if(parameters.noConfirmDialog)
            shFileOpStruct.fFlags |= (FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR);
        if(parameters.noProgressDialog)
            shFileOpStruct.fFlags |= FOF_SILENT;
        if(parameters.allowUndo)
            shFileOpStruct.fFlags |= FOF_ALLOWUNDO;

        int result = SHFileOperation(&shFileOpStruct);
        if(result != 0)
            throw BackendError(QObject::tr("Copy failed: %1").arg(getErrorString(result)));

        if(shFileOpStruct.fAnyOperationsAborted)
            throw BackendError(QObject::tr("Copy failed: aborted"));
    }

    void moveFilesWindows(const QString &sourceFilepath, const QString &destinationFilepath, const System::FileOperationParameters &parameters)
    {
        QDir sourceDir(sourceFilepath);
        QDir destinationDir(destinationFilepath);

        std::wstring wideSource = QDir::toNativeSeparators(sourceDir.absolutePath()).toStdWString();
        wideSource += L'\0';

        std::wstring wideDestination = QDir::toNativeSeparators(destinationDir.absolutePath()).toStdWString();
        wideDestination += L'\0';

        SHFILEOPSTRUCT shFileOpStruct;
        shFileOpStruct.hwnd = 0;
        shFileOpStruct.wFunc = FO_MOVE;
        shFileOpStruct.pFrom = wideSource.c_str();
        shFileOpStruct.pTo = wideDestination.c_str();
        shFileOpStruct.fFlags = 0;
        shFileOpStruct.fAnyOperationsAborted = false;
        shFileOpStruct.lpszProgressTitle = 0;
        shFileOpStruct.hNameMappings = 0;

        if(parameters.noErrorDialog)
            shFileOpStruct.fFlags |= FOF_NOERRORUI;
        if(parameters.noConfirmDialog)
            shFileOpStruct.fFlags |= (FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR);
        if(parameters.noProgressDialog)
            shFileOpStruct.fFlags |= FOF_SILENT;
        if(parameters.allowUndo)
            shFileOpStruct.fFlags |= FOF_ALLOWUNDO;

        int result = SHFileOperation(&shFileOpStruct);
        if(result != 0)
            throw BackendError(QObject::tr("Move failed: %1").arg(getErrorString(result)));

        if(shFileOpStruct.fAnyOperationsAborted)
            throw BackendError(QObject::tr("Move failed: aborted"));
    }

    void renameFilesWindows(const QString &sourceFilepath, const QString &destinationFilepath, const System::FileOperationParameters &parameters)
    {
        QDir sourceDir(sourceFilepath);
        QDir destinationDir(destinationFilepath);

        std::wstring wideSource = QDir::toNativeSeparators(sourceDir.absolutePath()).toStdWString();
        wideSource += L'\0';

        std::wstring wideDestination = QDir::toNativeSeparators(destinationDir.absolutePath()).toStdWString();
        wideDestination += L'\0';

        SHFILEOPSTRUCT shFileOpStruct;
        shFileOpStruct.hwnd = 0;
        shFileOpStruct.wFunc = FO_RENAME;
        shFileOpStruct.pFrom = wideSource.c_str();
        shFileOpStruct.pTo = wideDestination.c_str();
        shFileOpStruct.fFlags = 0;
        shFileOpStruct.fAnyOperationsAborted = false;
        shFileOpStruct.lpszProgressTitle = 0;
        shFileOpStruct.hNameMappings = 0;

        if(parameters.noErrorDialog)
            shFileOpStruct.fFlags |= FOF_NOERRORUI;
        if(parameters.noConfirmDialog)
            shFileOpStruct.fFlags |= (FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR);
        if(parameters.noProgressDialog)
            shFileOpStruct.fFlags |= FOF_SILENT;
        if(parameters.allowUndo)
            shFileOpStruct.fFlags |= FOF_ALLOWUNDO;

        int result = SHFileOperation(&shFileOpStruct);
        if(result != 0)
            throw BackendError(QObject::tr("Rename failed: %1").arg(getErrorString(result)));

        if(shFileOpStruct.fAnyOperationsAborted)
            throw BackendError(QObject::tr("Rename failed: aborted"));
    }

    void removeFilesWindows(const QString &filepath, const System::FileOperationParameters &parameters)
    {
        QDir filenameDir(filepath);

        std::wstring wideFilename = QDir::toNativeSeparators(filenameDir.absolutePath()).toStdWString();
        wideFilename += L'\0';

        SHFILEOPSTRUCT shFileOpStruct;
        shFileOpStruct.hwnd = 0;
        shFileOpStruct.wFunc = FO_DELETE;
        shFileOpStruct.pFrom = wideFilename.c_str();
        shFileOpStruct.pTo = 0;
        shFileOpStruct.fFlags = 0;
        shFileOpStruct.fAnyOperationsAborted = false;
        shFileOpStruct.lpszProgressTitle = 0;
        shFileOpStruct.hNameMappings = 0;

        if(parameters.noErrorDialog)
            shFileOpStruct.fFlags |= FOF_NOERRORUI;
        if(parameters.noConfirmDialog)
            shFileOpStruct.fFlags |= (FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR);
        if(parameters.noProgressDialog)
            shFileOpStruct.fFlags |= FOF_SILENT;
        if(parameters.allowUndo)
            shFileOpStruct.fFlags |= FOF_ALLOWUNDO;

        int result = SHFileOperation(&shFileOpStruct);
        if(result != 0)
            throw BackendError(QObject::tr("Remove failed: %1").arg(getErrorString(result)));

        if(shFileOpStruct.fAnyOperationsAborted)
            throw BackendError(QObject::tr("Remove failed: aborted"));
    }
}

