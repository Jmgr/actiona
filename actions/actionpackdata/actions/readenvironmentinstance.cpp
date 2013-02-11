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

#include "readenvironmentinstance.h"

#include <QFile>
#include <QTextStream>

namespace Actions
{
	ActionTools::StringListPair ReadEnvironmentVariableInstance::modes = qMakePair(
			QStringList() << "full" << "selection",
			QStringList() << QT_TRANSLATE_NOOP("ReadEnvironmentVariableInstance::modes", "Read all the environment variables") << QT_TRANSLATE_NOOP("ReadEnvironmentVariableInstance::modes", "Read only the specified variable"));

	void ReadEnvironmentVariableInstance::startExecution()
	{
		bool ok = true;

		QString variable = evaluateVariable(ok, "variable");
		Mode mode = evaluateListElement<Mode>(ok, modes, "mode");

		if(!ok)
			return;

		const QStringList &environment = QProcessEnvironment::systemEnvironment().toStringList();

		QHash<QString, QString> environmentHashVariableValue;
		environmentHashVariableValue.reserve(environment.count()+2); //doc said ideally 'slightly more than the maximum nb of item'

		foreach(QString environmentVariableAndValue, environment)
		{
			QStringList KeyValue = environmentVariableAndValue.split("=");
			environmentHashVariableValue[KeyValue.at(0)] = KeyValue.at(1);
		}

		if(mode == Selection)
		{
			QString envVariable	= evaluateString(ok, "envVariable");

			if (environmentHashVariableValue.contains(envVariable))
				setVariable(variable, environmentHashVariableValue.value(envVariable));
			else
				setVariable(variable, tr("[Undefined]"));
		}
		else
			setArrayKeyValue(variable, environmentHashVariableValue);


		emit executionEnded();
	}
}
