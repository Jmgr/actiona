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

#include "readenvironmentinstance.h"

#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QProcessEnvironment>
#include <QStringList>

namespace Actions
{
    Tools::StringListPair ReadEnvironmentVariableInstance::modes =
    {
        {
            QStringLiteral("allVariables"),
            QStringLiteral("oneVariable")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("ReadEnvironmentVariableInstance::modes", "Read all")),
            QStringLiteral(QT_TRANSLATE_NOOP("ReadEnvironmentVariableInstance::modes", "Read one"))
        }
    };

	void ReadEnvironmentVariableInstance::startExecution()
	{
		bool ok = true;

		QString variable = evaluateVariable(ok, QStringLiteral("variable"));
		Mode mode = evaluateListElement<Mode>(ok, modes, QStringLiteral("mode"));

		if(!ok)
			return;

		const QStringList &environment = QProcessEnvironment::systemEnvironment().toStringList();

		QHash<QString, QString> environmentHashVariableValue;
		environmentHashVariableValue.reserve(environment.count()+2); //doc said ideally 'slightly more than the maximum nb of item'

		for(QString environmentVariableAndValue: environment)
		{
			QStringList KeyValue = environmentVariableAndValue.split(QStringLiteral("="));
			environmentHashVariableValue[KeyValue.at(0)] = KeyValue.at(1);
		}

        if(mode == oneVariableMode)
		{
			QString environmentVariable	= evaluateString(ok, QStringLiteral("environmentVariableName"));

			if (environmentHashVariableValue.contains(environmentVariable))
				setVariable(variable, environmentHashVariableValue.value(environmentVariable));
			else
			{
                emit executionException(ActionTools::ActionException::InvalidParameterException, tr("The specified variable cannot be found in the system environment"));
				return;
			}
		}
		else
			setArrayKeyValue(variable, environmentHashVariableValue);


		executionEnded();
	}
}
