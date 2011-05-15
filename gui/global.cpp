/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#include "global.h"

#include <QSysInfo>

namespace Global
{
	QString currentOS()
	{
		QString wordSize = (QSysInfo::WordSize == 32) ? QString() : "64";

#ifdef Q_OS_LINUX
		return "linux" + wordSize;
#endif
#ifdef Q_OS_WIN
		QString windowsName;

		switch(QSysInfo::windowsVersion())
		{
		case QSysInfo::WV_95:
		case QSysInfo::WV_98:
			windowsName = "98";
			break;
		case QSysInfo::WV_Me:
			windowsName = "me";
			break;
		case QSysInfo::WV_NT:
		case QSysInfo::WV_2000:
		case QSysInfo::WV_XP:
		case QSysInfo::WV_2003:
			windowsName = "xp";
			break;
		case QSysInfo::WV_VISTA:
			windowsName = "vista";
			break;
		case QSysInfo::WV_WINDOWS7:
			windowsName = "seven";
			break;
                default:
                        windowsName = "unknown";
                        break;
		}

		return "windows" + windowsName + wordSize;
#endif

		return "unknown";
	}
}
