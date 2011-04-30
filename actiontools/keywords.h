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

#ifndef KEYWORDS_H
#define KEYWORDS_H

#include "actiontools_global.h"

#include <QStringList>

class QStandardItemModel;
struct QMetaObject;

namespace ActionTools
{
	extern const QStringList usedKeywords;
	extern const QStringList reservedKeywords;
	
	void ACTIONTOOLSSHARED_EXPORT addEcmaScriptObjectsKeywords(QStandardItemModel *model);
}

#endif // KEYWORDS_H
