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

#include "backend/process-windows.hpp"
#include "backend/backend.hpp"

#include <QObject>
#include <QThread>
#include <QElapsedTimer>

#include <Windows.h>
#include <Tlhelp32.h>

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

    void killProcessWindows(int id, Process::KillMode killMode, int timeout)
    {
        auto handle = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION, FALSE, id);
        if(!handle)
            throw BackendError(lastErrorString());

        if(killMode == Process::KillMode::Forceful)
        {
            CloseHandle(handle);
            if(!TerminateProcess(handle, 0))
                throw BackendError(lastErrorString());
            return;
        }

        // TODO
        /*
        for(const ActionTools::WindowHandle &windowHandle: ActionTools::WindowHandle::windowList())
        {
            if(windowHandle.processId() == id)
                windowHandle.close();
        }
        */

        QElapsedTimer timer;
        timer.start();

        DWORD exitCode;
        do
        {
            if(!GetExitCodeProcess(handle, &exitCode))
            {
                CloseHandle(handle);
                throw BackendError(lastErrorString());
            }

            if(timer.elapsed() >= timeout)
                break;

            QThread::msleep(100);
        }
        while(exitCode == STILL_ACTIVE);

        if(exitCode == STILL_ACTIVE)
        {
            if(killMode == Backend::Process::KillMode::Graceful)
            {
                CloseHandle(handle);
                throw BackendError(QObject::tr("failed to gracefully terminate the process"));
            }

            if(!TerminateProcess(handle, 0))
            {
                CloseHandle(handle);
                throw BackendError(lastErrorString());
            }
        }

        CloseHandle(handle);
    }

    Process::ProcessStatus processStatusWindows(int id)
    {
        auto handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id);
        if(handle)
        {
            CloseHandle(handle);
            return Process::ProcessStatus::Running;
        }

        return Process::ProcessStatus::Stopped;
    }

    QList<int> runningProcessesWindows()
    {
        auto handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if(!handle)
            throw BackendError(lastErrorString());

        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        bool first = Process32First(handle, &entry);
        if(!first)
        {
            CloseHandle(handle);
            if(GetLastError() == ERROR_NO_MORE_FILES)
                return {};

            throw BackendError(lastErrorString());
        }

        QList<int> back{static_cast<int>(entry.th32ProcessID)};
        while(true)
        {
            bool next = Process32Next(handle, &entry);
            if(!next)
            {
                CloseHandle(handle);
                if(GetLastError() == ERROR_NO_MORE_FILES)
                    return {};

                throw BackendError(lastErrorString());
            }

            back.append(entry.th32ProcessID);
        }

        return back;
    }
}

