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

#include "actioninstance.h"
#include "actiondefinition.h"
#include "script.h"

namespace Actions
{
	class CallProcedureInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		CallProcedureInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent)										{}

		void startExecution() override
		{
			bool ok = true;

			QString name = evaluateString(ok, QStringLiteral("name"));

			if(!ok)
				return;

			if(!callProcedure(name))
				return;

			executionEnded();
		}

	private:
		Q_DISABLE_COPY(CallProcedureInstance)
	};
}

