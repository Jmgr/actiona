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

#include "actiontools/ifactionvalue.hpp"

namespace ActionTools
{
    QLatin1String IfActionValue::WAIT = QLatin1String("wait");
    QLatin1String IfActionValue::GOTO = QLatin1String("goto");
    QLatin1String IfActionValue::RUNCODE = QLatin1String("run_code");
    QLatin1String IfActionValue::CALLPROCEDURE = QLatin1String("call_procedure");
    QLatin1String IfActionValue::STOPEXECUTION = QLatin1String("stop_execution");
}
