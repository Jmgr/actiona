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

	Contact : jmgr@jmgr.info
*/

#include "crossplatform.h"
#include "windowhandle.h"

#include <QWidget>
#include <QElapsedTimer>
#include <QDebug>
#include <QDir>

#ifdef Q_OS_UNIX
#include <QX11Info>
#include <X11/Xlib.h>
#include <cerrno>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#endif

#ifdef Q_OS_WIN
#include <Windows.h>
#include <Tlhelp32.h>
#endif

namespace ActionTools
{
	QString CrossPlatform::mLastErrorString;
	int CrossPlatform::mLastError = -1;

	void CrossPlatform::setForegroundWindow(QWidget *window)
	{
#ifdef Q_OS_UNIX
		XRaiseWindow(QX11Info::display(), window->winId());
#endif
#ifdef Q_OS_WIN
        if(IsIconic(reinterpret_cast<HWND>(window->winId())))
            ShowWindow(reinterpret_cast<HWND>(window->winId()), SW_RESTORE);
		else
		{
            if(!SetForegroundWindow(reinterpret_cast<HWND>(window->winId())))
				setupLastError();

            if(!SetWindowPos(reinterpret_cast<HWND>(window->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE))
				setupLastError();
		}
#endif
	}

	bool CrossPlatform::killProcess(int id, KillMode killMode, int timeout)
	{
#ifdef Q_OS_UNIX
		switch(killMode)
		{
		case Graceful:
			{
				int ret = kill(id, SIGTERM);

				if(ret != 0)
				{
					setupLastError();
					return false;
				}

				return true;
			}
		case Forceful:
			{
				int ret = kill(id, SIGKILL);

				if(ret != 0)
				{
					setupLastError();
					return false;
				}

				return true;
			}
		case GracefulThenForceful:
			{
				if(kill(id, SIGTERM) != 0)
				{
					setupLastError();
					return false;
				}

				QElapsedTimer timer;
				timer.start();

				while(true)
				{
					if(processStatus(id) == Stopped)
						return true;

					if(timer.elapsed() >= timeout)
					{
						int ret = kill(id, SIGKILL);
						if(ret != 0)
						{
							if(ret == ESRCH) //No such process
								return true;

							setupLastError();
							return false;
						}

						timespec req;
						req.tv_sec = 0;
						req.tv_nsec = 10000; //10 msec
						nanosleep(&req, nullptr);

						return (processStatus(id) == Stopped);
					}

					timespec req;
					req.tv_sec = 0;
					req.tv_nsec = 100000; //100 msec
					nanosleep(&req, nullptr);
				}
			}
		}

		return false;
#endif
#ifdef Q_OS_WIN
		HANDLE process = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION, FALSE, id);
		if(!process)
		{
			setupLastError();
			return false;
		}

		if(killMode == Forceful)
		{
			CloseHandle(process);
			bool ret = TerminateProcess(process, 0);
			if(!ret)
				setupLastError();

            return ret;
		}
		
        for(const ActionTools::WindowHandle &windowHandle: ActionTools::WindowHandle::windowList())
		{
			if(windowHandle.processId() == id)
				windowHandle.close();
		}

        QElapsedTimer timer;
		timer.start();

		DWORD exitCode;
		do
		{
			if(!GetExitCodeProcess(process, &exitCode))
			{
				CloseHandle(process);
				setupLastError();
				return false;
			}

			if(timer.elapsed() >= timeout)
				break;

			Sleep(100);
		}
		while(exitCode == STILL_ACTIVE);

		if(exitCode == STILL_ACTIVE)
		{
			if(killMode == Graceful)
			{
				CloseHandle(process);
				setupLastError();
				return false;
			}

			if(!TerminateProcess(process, 0))
			{
				CloseHandle(process);
				setupLastError();
				return false;
			}
		}

		CloseHandle(process);

		return true;
#endif
	}

	CrossPlatform::ProcessStatus CrossPlatform::processStatus(int id)
	{
#ifdef Q_OS_UNIX
		return (kill(id, 0) == 0) ? Running : Stopped;
#endif
#ifdef Q_OS_WIN
		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id);
		if(process)
		{
			CloseHandle(process);
			return Running;
		}

		return Stopped;
#endif
	}

	QList<int> CrossPlatform::runningProcesses()
	{
#ifdef Q_OS_UNIX
		QDir procDir(QStringLiteral("/proc"));
		QList<int> back;

		if(!procDir.exists())
			return back;

		QStringList processes = procDir.entryList(QDir::Dirs);
        for(const QString &processId: processes)
		{
			bool success;
			int id = processId.toInt(&success);

			if(success)
				back.append(id);
		}

		return back;
#endif
#ifdef Q_OS_WIN
		QList<int> back;
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if(!handle)
		{
			setupLastError();
			return back;
		}

		PROCESSENTRY32 info = {0};
		info.dwSize = sizeof(PROCESSENTRY32);

		bool first = Process32First(handle, &info);
		if(!first)
		{
			CloseHandle(handle);
			setupLastError();
			return back;
		}

		do
		{
			back.append(info.th32ProcessID);
		}
		while(Process32Next(handle, &info));

		CloseHandle(handle);

		return back;
#endif
	}

	void CrossPlatform::sleep(int milliseconds)
	{
#ifdef Q_OS_UNIX
		struct timespec timeout0;
		struct timespec timeout1;
		struct timespec* tmp;
		struct timespec* t0 = &timeout0;
		struct timespec* t1 = &timeout1;

		t0->tv_sec = milliseconds / 1000;
		t0->tv_nsec = (milliseconds % 1000) * (1000 * 1000);

		while ((nanosleep(t0, t1) == (-1)) && (errno == EINTR))
		{
			tmp = t0;
			t0 = t1;
			t1 = tmp;
		}
#endif
#ifdef Q_OS_WIN
		Sleep(milliseconds);
#endif
	}

	void CrossPlatform::setupLastError()
	{
#ifdef Q_OS_UNIX
		mLastError = errno;

		char *errorStr = strerror(errno);
		mLastErrorString = QString::fromUtf8(errorStr);
#endif
#ifdef Q_OS_WIN
		mLastError = GetLastError();
		LPTSTR message;

		if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
					  0, mLastError, 0, (LPTSTR)&message, 0, 0))
		{
			qDebug() << "Error: " << mLastError << ", and error " << GetLastError() << " while trying to get the error message";

			mLastErrorString = QString();
			return;
		}

		mLastErrorString = QString::fromWCharArray(message).trimmed();

		LocalFree(message);
#endif

		qDebug() << "Error: " << mLastErrorString << " (" << mLastError << ")";
	}
}
