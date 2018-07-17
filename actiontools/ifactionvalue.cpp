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

#include "ifactionvalue.h"

namespace ActionTools
{
	QLatin1String IfActionValue::WAIT = QLatin1Literal("wait");
	QLatin1String IfActionValue::GOTO = QLatin1Literal("goto");
	QLatin1String IfActionValue::RUNCODE = QLatin1Literal("run_code");
	QLatin1String IfActionValue::CALLPROCEDURE = QLatin1Literal("call_procedure");
}
