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

//Settings stuff
//Warning : all enums have to be in the same order as in the combo boxes !

namespace ActionTools
{
	namespace Settings
	{
		enum
		{
			CHECK_FOR_UPDATES_UNKNOWN = -1,
			CHECK_FOR_UPDATES_NEVER,
			CHECK_FOR_UPDATES_DAY,
			CHECK_FOR_UPDATES_WEEK,
			CHECK_FOR_UPDATES_MONTH
		};

		enum
		{
			PROXY_TYPE_HTTP,
			PROXY_TYPE_SOCKS5
		};

		enum
		{
			PROXY_NONE,
			PROXY_SYSTEM,
			PROXY_CUSTOM
		};
	}
}

