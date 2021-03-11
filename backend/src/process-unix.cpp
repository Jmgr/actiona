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

#include "backend/process-unix.hpp"
#include "backend/backend.hpp"

#include <QDir>
#include <QString>
#include <QElapsedTimer>
#include <QThread>
#include <QProcess>

#include <csignal>

namespace Backend
{
    QString lastErrorString()
    {
        return QString::fromUtf8(strerror(errno));
    }

    void killProcessUnix(int id, Process::KillMode killMode, int timeout)
    {
        switch(killMode)
        {
        case Process::KillMode::Graceful:
            {
                if(kill(id, SIGTERM) != 0)
                    throw BackendError(lastErrorString());
            }
        case Process::KillMode::Forceful:
            {
                if(kill(id, SIGKILL) != 0)
                    throw BackendError(lastErrorString());
            }
        case Process::KillMode::GracefulThenForceful:
            {
                if(kill(id, SIGTERM) != 0)
                    throw BackendError(lastErrorString());

                QElapsedTimer timer;
                timer.start();

                while(true)
                {
                    if(Instance::process().processStatus(id) == Process::ProcessStatus::Stopped)
                        return;

                    if(timer.elapsed() >= timeout)
                    {
                        auto res = kill(id, SIGKILL);
                        if(res != 0)
                        {
                            if(res == ESRCH) //No such process
                                return;

                            throw BackendError(lastErrorString());
                        }

                        return;
                    }

                    QThread::msleep(100);
                }
            }
        default:
            throw BackendError(QStringLiteral("unknown process kill mode: %1").arg(static_cast<int>(killMode)));
        }
    }

    Process::ProcessStatus processStatusUnix(int id)
    {
        auto res = kill(id, 0);
        switch(res)
        {
        case 0:
            return Process::ProcessStatus::Running;
        case -1:
            throw BackendError(lastErrorString());
        default:
            return Process::ProcessStatus::Stopped;
        }
    }

    QList<int> runningProcessesUnix()
    {
        QDir procDir(QStringLiteral("/proc"));

        if(!procDir.exists())
            return {};

        QStringList processes = procDir.entryList(QDir::Dirs);
        QList<int> back;
        back.reserve(processes.size());

        for(const QString &processId: qAsConst(processes))
        {
            bool ok;
            int id = processId.toInt(&ok);

            if(ok)
                back.append(id);
        }

        return back;
    }

    int parentProcessUnix(int id)
    {
        QProcess process;
        process.start(QStringLiteral("ps"), {QStringLiteral("h") , QStringLiteral("-p %1").arg(id), QStringLiteral("-oppid")}, QIODevice::ReadOnly);
        if(!process.waitForStarted(2000) || !process.waitForReadyRead(2000) || !process.waitForFinished(2000) || process.exitCode() != 0)
            throw BackendError(QStringLiteral("failed to get the process parent id"));

        bool ok = true;
        auto res = process.readAll().trimmed();
        int result = res.toInt(&ok);

        if(!ok)
            throw BackendError(QStringLiteral("failed to get the process parent id, 'ps' returned %1").arg(QString::fromLocal8Bit(res)));

        return result;
    }

    QString processCommandUnix(int id)
    {
        QProcess process;
        process.start(QStringLiteral("ps"), {QStringLiteral("h"), QStringLiteral("-p %1").arg(id), QStringLiteral("-ocommand")}, QIODevice::ReadOnly);
        if(!process.waitForStarted(2000) || !process.waitForReadyRead(2000) || !process.waitForFinished(2000) || process.exitCode() != 0)
            throw BackendError(QStringLiteral("failed to get the process command"));

        return QLatin1String(process.readAll().trimmed());
    }

    Process::Priority processPriorityUnix(int id)
    {
        Q_UNUSED(id)

        throw BackendError(QStringLiteral("this is not available under your operating system"));
    }
}

