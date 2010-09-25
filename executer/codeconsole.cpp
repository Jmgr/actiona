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

#include "codeconsole.h"

#include <QTextStream>

namespace LibExecuter
{
	void CodeConsole::print(const QString &text)
	{
		QTextStream stream(stdout);
		stream << text;
		stream.flush();
	}

	void CodeConsole::printWarning(const QString &text)
	{
		QTextStream stream(stdout);
		stream << tr("Warning: ") << text;
		stream.flush();
	}

	void CodeConsole::printError(const QString &text)
	{
		QTextStream stream(stdout);
		stream << tr("Error: ") << text;
		stream.flush();
	}
}
