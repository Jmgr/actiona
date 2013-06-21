/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#include "readregistryinstance.h"

#include <QScriptEngine>

#ifdef Q_WS_WIN
#include <Windows.h>
#endif

namespace Actions
{
	ActionTools::StringListPair ReadRegistryInstance::keys = qMakePair(
			QStringList() << "classesRoot" << "currentConfig" << "currentUser" << "users" << "localMachine",
			QStringList() << "Classes root" << "Current config" << "Current user" << "Users" << "Local machine");
	//Note : This is not translated, because this is not translated either in Regedit

	void ReadRegistryInstance::startExecution()
	{
	#ifdef Q_WS_WIN
		bool ok = true;

		ActionTools::Registry::Key key = evaluateListElement<ActionTools::Registry::Key>(ok, keys, "key");
		QString subKey = evaluateString(ok, "subKey");
		QString value = evaluateString(ok, "value");
		QString variable = evaluateVariable(ok, "variable");

		if(!ok)
			return;

		QVariant resultValue;
		switch(ActionTools::Registry::read(resultValue, key, subKey, value))
		{
		case ActionTools::Registry::ReadCannotFindSubKey:
			setCurrentParameter("subKey");
			emit executionException(CannotFindSubKeyException, tr("Cannot find subKey \"%1\"").arg(subKey));
			return;
		case ActionTools::Registry::ReadCannotFindValue:
			setCurrentParameter("value");
			emit executionException(CannotFindValueException, tr("Cannot find value \"%1\"").arg(value));
			return;
		case ActionTools::Registry::ReadInvalidValueType:
			setCurrentParameter("value");
			emit executionException(CannotFindValueException, tr("Invalid value type"));
			return;
		default:
            setVariable(variable, scriptEngine()->newVariant(resultValue));
			break;
		}
	#endif

		emit executionEnded();
	}
}
