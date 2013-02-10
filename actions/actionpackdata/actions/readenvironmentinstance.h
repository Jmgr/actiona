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

#ifndef READENVIRONMENTINSTANCE_H
#define READENVIRONMENTINSTANCE_H

#include "actioninstance.h"
#include "script.h"

#include <QApplication>
#include <QProcessEnvironment>
#include <QStringList>

namespace Actions
{
	class ReadEnvironmentVariableInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		ReadEnvironmentVariableInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)												{}

		void startExecution()
		{
			bool ok = true;

			QString variable = evaluateVariable(ok, "variable");

			if(!ok)
				return;

			QStringList Keys, Values, KeyValue;
			//read the system environment
			QStringList ListVE = QProcessEnvironment::systemEnvironment().toStringList();

			//breaks the results in two lists : one for the keys, the other for the values
			for(int index = 0; index < ListVE.count(); ++index)
			{
				KeyValue = ListVE.at(index).split("=");
				Keys << KeyValue.at(0);
				Values << KeyValue.at(1);
			}

			setArrayKeyValue(variable, Keys, Values);

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(ReadEnvironmentVariableInstance)
	};
}

#endif // READENVIRONMENTINSTANCE_H
