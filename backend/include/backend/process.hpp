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

#include <QList>

#include <functional>
#include <memory>

namespace Backend
{
    class BACKENDSHARED_EXPORT Process final
    {
        Q_DISABLE_COPY(Process)

    private:
        Process() = default;

    public:
        enum class KillMode
        {
            Graceful,
            Forceful,
            GracefulThenForceful
        };
        enum class ProcessStatus
        {
            Running,
            Stopped
        };
        enum class Priority
        {
            AboveNormal,
            BelowNormal,
            High,
            Idle,
            Normal,
            Realtime
        };

        std::function<void(int id, KillMode killMode, int timeout)> killProcess;
        std::function<ProcessStatus(int id)> processStatus;
        std::function<QList<int>()> runningProcesses;
        std::function<int(int id)> parentProcess;
        std::function<QString(int id)> processCommand;
        std::function<Priority(int id)> processPriority;

        friend std::unique_ptr<Process> std::make_unique<Process>();
    };

    // Dummy implementations
    static void killProcessDummy(int, Process::KillMode, int) {}
    static Process::ProcessStatus processStatusDummy(int) { return Process::ProcessStatus::Running; }
    static QList<int> runningProcessesDummy() { return {}; }
    static int parentProcessDummy(int) { return {}; }
    static QString processCommandDummy(int) { return {}; }
    static Process::Priority processPriorityDummy(int) { return {}; }
}
