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

#include "actiontools/code/processhandle.hpp"

#include <QProcess>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#endif

namespace Code
{
	ProcessHandle::ProcessHandle()
		: CodeClass()
	{
		
	}

	ProcessHandle::ProcessHandle(const ProcessHandle &other)
		: CodeClass(),
		mProcessId(other.processId())
	{
		
	}

	ProcessHandle::ProcessHandle(int processId)
		: CodeClass(),
		mProcessId(processId)
	{
		
	}
	
	ProcessHandle &ProcessHandle::operator=(ProcessHandle other)
	{
		swap(other);
		
		return *this;
	}

	ProcessHandle &ProcessHandle::operator=(int processId)
	{
		swap(processId);
		
		return *this;
	}
	
	void ProcessHandle::swap(ProcessHandle &other)
	{
		std::swap(mProcessId, other.mProcessId);
	}

	void ProcessHandle::swap(int &processId)
	{
		std::swap(mProcessId, processId);
	}
	
	int ProcessHandle::processId() const
	{
		return mProcessId;
	}
	
	QJSValue ProcessHandle::clone() const
	{
        return CodeClass::clone<ProcessHandle>();
	}

	bool ProcessHandle::equals(const QJSValue &other) const
	{
		if(other.isUndefined() || other.isNull())
			return false;
		
		QObject *object = other.toQObject();
		if(auto otherProcess = qobject_cast<ProcessHandle*>(object))
			return (otherProcess == this || otherProcess->mProcessId == mProcessId);
			
		return false;
	}

	QString ProcessHandle::toString() const
	{
		return QStringLiteral("ProcessHandle {id: %1}").arg(processId());
	}
	
	int ProcessHandle::id() const
	{
        return processId();
    }

    int ProcessHandle::parentId() const
    {
#ifdef Q_OS_WIN
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if(!snapshot)
        {
			throwError(QStringLiteral("CreateSnapshotError"), tr("Unable to create a snapshot"));
            return 0;
        }

        PROCESSENTRY32 processEntry;
        ZeroMemory(&processEntry, sizeof(processEntry));
        processEntry.dwSize = sizeof(processEntry);

        if(!Process32First(snapshot, &processEntry))
        {
            CloseHandle(snapshot);

			throwError(QStringLiteral("GetFirstProcessError"), tr("Unable to get the first process"));
            return 0;
        }

        do
        {
            if(processEntry.th32ProcessID == id())
            {
                CloseHandle(snapshot);

                return processEntry.th32ParentProcessID;
            }
        }
        while(Process32Next(snapshot, &processEntry));

        CloseHandle(snapshot);

        return 0;
#else
        QProcess process;
        process.start(QStringLiteral("ps"), {QStringLiteral("h") , QStringLiteral("-p %1").arg(id()), QStringLiteral("-oppid")}, QIODevice::ReadOnly);
        if(!process.waitForStarted(2000) || !process.waitForReadyRead(2000) || !process.waitForFinished(2000) || process.exitCode() != 0)
        {
			throwError(QStringLiteral("GetProcessError"), tr("Failed to get the process parent id"));
            return 0;
        }

        bool ok = true;
        int result = process.readAll().trimmed().toInt(&ok);

        if(!ok)
        {
			throwError(QStringLiteral("GetProcessError"), tr("Failed to get the process parent id"));
            return 0;
        }

        return result;
#endif
    }
	
	bool ProcessHandle::kill(KillMode killMode, int timeout) const
	{
		return ActionTools::CrossPlatform::killProcess(mProcessId, static_cast<ActionTools::CrossPlatform::KillMode>(killMode), timeout);
	}
	
	bool ProcessHandle::isRunning() const
	{
		return (ActionTools::CrossPlatform::processStatus(mProcessId) == ActionTools::CrossPlatform::Running);
	}

	QString ProcessHandle::command() const
	{
#ifdef Q_OS_WIN
		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, id());
		if(!process)
		{
			throwError(QStringLiteral("OpenProcessError"), tr("Unable to open the process"));
			return QString();
		}

		TCHAR buffer[256];
		if(!GetModuleFileNameEx(process, NULL, buffer, 256))
		{
			throwError(QStringLiteral("GetModuleFilenameError"), tr("Unable to retrieve the executable filename"));
			return QString();
		}

		CloseHandle(process);

		return QString::fromWCharArray(buffer);
#else
		QProcess process;
        process.start(QStringLiteral("ps"), {QStringLiteral("h"), QStringLiteral("-p %1").arg(id()), QStringLiteral("-ocommand")}, QIODevice::ReadOnly);
		if(!process.waitForStarted(2000) || !process.waitForReadyRead(2000) || !process.waitForFinished(2000) || process.exitCode() != 0)
		{
			throwError(QStringLiteral("GetProcessError"), tr("Failed to get the process command"));
			return QString();
		}

		return QLatin1String(process.readAll().trimmed());
#endif
	}

	ProcessHandle::Priority ProcessHandle::priority() const
	{
#ifdef Q_OS_WIN
		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id());
		if(!process)
		{
			throwError(QStringLiteral("OpenProcessError"), tr("Unable to open the process"));
			return Normal;
		}

		int priority = GetPriorityClass(process);
		CloseHandle(process);

		switch(priority)
		{
		case ABOVE_NORMAL_PRIORITY_CLASS:
			return AboveNormal;
		case BELOW_NORMAL_PRIORITY_CLASS:
			return BelowNormal;
		case HIGH_PRIORITY_CLASS:
			return High;
		case IDLE_PRIORITY_CLASS:
			return Idle;
		case NORMAL_PRIORITY_CLASS:
			return Normal;
		case REALTIME_PRIORITY_CLASS:
			return Realtime;
		default:
			throwError(QStringLiteral("GetPriorityClassError"), tr("Unable to retrieve the process priority"));
			return Normal;
		}
#else
		throwError(QStringLiteral("OperatingSystemError"), tr("This is not available under your operating system"));
		return Normal;
#endif
	}

    void ProcessHandle::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<ProcessHandle*>("const ProcessHandle *");

        CodeClass::registerClass<ProcessHandle>(QStringLiteral("ProcessHandle"), scriptEngine);
    }
}
