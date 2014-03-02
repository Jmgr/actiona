/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef BEGINPROCEDUREINSTANCE_H
#define BEGINPROCEDUREINSTANCE_H

#include "actioninstance.h"
#include "actiondefinition.h"

namespace Actions
{
	class BeginProcedureInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		BeginProcedureInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)										{}

		void startExecution()
		{
			setNextLine(runtimeParameter("procedureEndLine").toInt() + 2);//Lines start at 1

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(BeginProcedureInstance)
	};
}

#endif // BEGINPROCEDUREINSTANCE_H
