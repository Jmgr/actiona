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
#include "backend/backend-windows.hpp"
#include "backend/windowing.hpp"

#include <QObject>
#include <QThread>
#include <QElapsedTimer>

#include <Windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>

#include <array>

namespace Backend
{
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

        auto windows = Instance::windowing().windowList();
        for(const auto &windowHandle: windows)
        {
            if(Instance::windowing().processId(windowHandle) == id)
                Instance::windowing().close(windowHandle);
        }

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
            if(killMode == Process::KillMode::Graceful)
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

    int parentProcessWindows(int id)
    {
        auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if(snapshot == INVALID_HANDLE_VALUE)
            throw BackendError(lastErrorString());

        PROCESSENTRY32 processEntry;
        ZeroMemory(&processEntry, sizeof(processEntry));
        processEntry.dwSize = sizeof(processEntry);

        if(!Process32First(snapshot, &processEntry))
        {
            if(GetLastError() != ERROR_NO_MORE_FILES)
            {
                CloseHandle(snapshot);
                throw BackendError(lastErrorString());
            }
        }

        while(true)
        {
            if(!Process32Next(snapshot, &processEntry))
            {
                if(GetLastError() != ERROR_NO_MORE_FILES)
                {
                    CloseHandle(snapshot);
                    throw BackendError(lastErrorString());
                }
            }

            if(processEntry.th32ProcessID == id)
            {
                CloseHandle(snapshot);
                return processEntry.th32ParentProcessID;
            }
        }

        CloseHandle(snapshot);

        return 0;
    }

    QString processCommandWindows(int id)
    {
        auto process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, id);
        if(!process)
            throw BackendError(lastErrorString());

        std::array<TCHAR, 256> buffer;
        if(!GetModuleFileNameEx(process, nullptr, buffer.data(), buffer.size()))
        {
            CloseHandle(process);
            throw BackendError(lastErrorString());
        }

        CloseHandle(process);

        return QString::fromWCharArray(buffer.data());
    }

    Process::Priority processPriorityWindows(int id)
    {
        auto process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id);
        if(!process)
            throw BackendError(lastErrorString());

        int priority = GetPriorityClass(process);
        CloseHandle(process);
        if(!priority)
            throw BackendError(lastErrorString());

        switch(priority)
        {
        case ABOVE_NORMAL_PRIORITY_CLASS:
            return Process::Priority::AboveNormal;
        case BELOW_NORMAL_PRIORITY_CLASS:
            return Process::Priority::BelowNormal;
        case HIGH_PRIORITY_CLASS:
            return Process::Priority::High;
        case IDLE_PRIORITY_CLASS:
            return Process::Priority::Idle;
        case NORMAL_PRIORITY_CLASS:
        default:
            return Process::Priority::Normal;
        case REALTIME_PRIORITY_CLASS:
            return Process::Priority::Realtime;
        }
    }
}

