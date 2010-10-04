/*
		Actionaz
		Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#include "actionreadregistryinstance.h"
#include "actioninstanceexecutionhelper.h"

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

ActionTools::StringListPair ActionReadRegistryInstance::keys = qMakePair(
		QStringList() << "classesRoot" << "currentConfig" << "currentUser" << "users" << "localMachine",
		QStringList() << "Classes root" << "Current config" << "Current user" << "Users" << "Local machine");
//Note : This is not translated, because this is not translated either in Regedit

void ActionReadRegistryInstance::startExecution()
{
	ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
#ifdef Q_WS_WIN
	ActionTools::Registry::Key key;
	QString subKey;
	QString value;
	QString variable;

	if(!actionInstanceExecutionHelper.evaluateListElement(key, keys, "key") ||
	   !actionInstanceExecutionHelper.evaluateString(subKey, "subKey") ||
	   !actionInstanceExecutionHelper.evaluateString(value, "value") ||
	   !actionInstanceExecutionHelper.evaluateVariable(variable, "variable"))
		return;

	QVariant resultValue;
	switch(ActionTools::Registry::read(resultValue, key, subKey, value))
	{
	case ActionTools::Registry::ReadCannotFindSubKey:
		actionInstanceExecutionHelper.setCurrentParameter("subKey");
		emit executionException(CannotFindSubKeyException, tr("Cannot find subKey \"%1\"").arg(subKey));
		return;
	case ActionTools::Registry::ReadCannotFindValue:
		actionInstanceExecutionHelper.setCurrentParameter("value");
		emit executionException(CannotFindValueException, tr("Cannot find value \"%1\"").arg(value));
		return;
	case ActionTools::Registry::ReadInvalidValueType:
		actionInstanceExecutionHelper.setCurrentParameter("value");
		emit executionException(CannotFindValueException, tr("Invalid value type"));
		return;
	default:
		actionInstanceExecutionHelper.setVariable(variable, resultValue);
		break;
	}
#endif

	emit executionEnded();
}
