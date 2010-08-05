/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#ifndef IFACTIONVALUE_H
#define IFACTIONVALUE_H

#include "actiontools_global.h"

#include <QString>

namespace ActionTools
{
	class ACTIONTOOLSSHARED_EXPORT IfActionValue
	{
	public:
		IfActionValue(const QString &action = QString(), const QString &line = QString())
			: mAction(action), mLine(line)								{}
		
		void setAction(const QString &action)							{ mAction = action; }
		void setLine(const QString &line)								{ mLine = line; }
		
		QString action() const											{ return mAction; }
		QString line() const											{ return mLine; }
		
		QString &action()												{ return mAction; }
		QString &line()													{ return mLine; }
		
		static const char *WAIT;
		static const char *GOTO;
		
	private:
		QString mAction;
		QString mLine;
	};
}

#endif // IFACTIONVALUE_H
