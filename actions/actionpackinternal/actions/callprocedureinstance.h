/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#ifndef CALLPROCEDUREINSTANCE_H
#define CALLPROCEDUREINSTANCE_H

#include "actioninstance.h"
#include "actiondefinition.h"
#include "script.h"

namespace Actions
{
	class CallProcedureInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		CallProcedureInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)										{}

		void startExecution()
		{
			bool ok = true;

			QString name = evaluateString(ok, "name");

			if(!ok)
				return;

			//Search for the corresponding ActionBeginProcedure action
			int startLine = -1;

			for(int line = 0; line < script()->actionCount(); ++line)
			{
				ActionTools::ActionInstance *action = script()->actionAt(line);

				if(action->definition()->id() == "ActionBeginProcedure")
				{
					const ActionTools::SubParameter &subParameter = action->subParameter("name", "value");

					if(subParameter.value().toString() == name)
					{
						startLine = line;

						break;
					}
				}
			}

			if(startLine == -1)
			{
				emit executionException(ActionTools::ActionException::BadParameterException, tr("Unable to find any procedure named \"%1\"").arg(name));

				return;
			}
			else
				setNextLine(QString::number(startLine+2));

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(CallProcedureInstance)
	};
}

#endif // CALLPROCEDUREINSTANCE_H
