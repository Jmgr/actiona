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

#include "actiontools_global.hpp"

#include <QVariant>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT Registry
	{
	public:
		enum Key
		{
			ClassesRoot,
			CurrentConfig,
			CurrentUser,
			Users,
			LocalMachine,

			KeyCount
		};
		enum ReadResult
		{
			ReadOk,
			ReadCannotFindSubKey,
			ReadCannotFindValue,
			ReadInvalidValueType
		};
		enum WriteResult
		{
			WriteOk,
			WriteCannotFindSubKey,
			WriteCannotWriteValue
		};

#ifdef Q_OS_WIN
		static ReadResult read(QVariant &result, Key key, const QString &subkey, const QString &value = QString());
		static WriteResult write(const QVariant &data, Key key, const QString &subkey, const QString &value = QString());
#endif

		Registry() = delete;

    private:
#ifdef Q_OS_WIN
		static HKEY enumToKey(Key key);
#endif
	};
}

