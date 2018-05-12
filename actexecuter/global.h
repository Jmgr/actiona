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

#include "version.h"

namespace Global
{
#if (QT_VERSION >= 0x050600)
	const Tools::Version	ACTIONA_VERSION =	QVersionNumber::fromString(QLatin1String(VERSION_TO_STRING(ACT_VERSION)));
	const Tools::Version	SCRIPT_VERSION =	QVersionNumber::fromString(QLatin1String(VERSION_TO_STRING(ACT_SCRIPT_VERSION)));
#else
	const Tools::Version	ACTIONA_VERSION =	Tools::Version(QLatin1String(VERSION_TO_STRING(ACT_VERSION)));
	const Tools::Version	SCRIPT_VERSION =	Tools::Version(QLatin1String(VERSION_TO_STRING(ACT_SCRIPT_VERSION)));
#endif
	const QString			CONNECTIVITY_URL =	QStringLiteral("http://actiona.tools/");
}

