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

#pragma once

#include "actiontools_global.h"

#include <QString>
#include <QList>
#include <QObject>

class QWidget;

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT CrossPlatform : public QObject
	{
		Q_OBJECT
		Q_ENUMS(KillMode)
		Q_ENUMS(ProcessStatus)
		
	public:
		enum KillMode
		{
			Graceful,
			Forceful,
			GracefulThenForceful
		};
		enum ProcessStatus
		{
			Running,
			Stopped
		};

		//Windows
		static void setForegroundWindow(QWidget *window);

		//Processes
		static bool killProcess(int id, KillMode killMode = GracefulThenForceful, int timeout = 3000);
		static ProcessStatus processStatus(int id);
		static QList<int> runningProcesses();

		static int lastError()						{ return mLastError; }
		static QString lastErrorString()			{ return mLastErrorString; }
		
		static void sleep(int milliseconds);

	private:
		static void setupLastError();

		static QString mLastErrorString;
		static int mLastError;
	};
}

