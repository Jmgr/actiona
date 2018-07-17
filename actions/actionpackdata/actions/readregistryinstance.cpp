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

#include "readregistryinstance.h"

#include <QScriptEngine>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace Actions
{
    Tools::StringListPair ReadRegistryInstance::keys =
    {
        {
            QStringLiteral("classesRoot"),
            QStringLiteral("currentConfig"),
            QStringLiteral("currentUser"),
            QStringLiteral("users"),
            QStringLiteral("localMachine")
        },
        {
            QStringLiteral("Classes root"),
            QStringLiteral("Current config"),
            QStringLiteral("Current user"),
            QStringLiteral("Users"),
            QStringLiteral("Local machine")
        }
    };
	//Note : This is not translated, because this is not translated either in Regedit

	void ReadRegistryInstance::startExecution()
	{
	#ifdef Q_OS_WIN
		bool ok = true;

		ActionTools::Registry::Key key = evaluateListElement<ActionTools::Registry::Key>(ok, keys, QStringLiteral("key"));
		QString subKey = evaluateString(ok, QStringLiteral("subKey"));
		QString value = evaluateString(ok, QStringLiteral("value"));
		QString variable = evaluateVariable(ok, QStringLiteral("variable"));

		if(!ok)
			return;

		QVariant resultValue;
		switch(ActionTools::Registry::read(resultValue, key, subKey, value))
		{
		case ActionTools::Registry::ReadCannotFindSubKey:
			setCurrentParameter(QStringLiteral("subKey"));
			emit executionException(CannotFindSubKeyException, tr("Cannot find subKey \"%1\"").arg(subKey));
			return;
		case ActionTools::Registry::ReadCannotFindValue:
			setCurrentParameter(QStringLiteral("value"));
			emit executionException(CannotFindValueException, tr("Cannot find value \"%1\"").arg(value));
			return;
		case ActionTools::Registry::ReadInvalidValueType:
			setCurrentParameter(QStringLiteral("value"));
			emit executionException(CannotFindValueException, tr("Invalid value type"));
			return;
		default:
            setVariable(variable, scriptEngine()->newVariant(resultValue));
			break;
		}
	#endif

		executionEnded();
	}
}
