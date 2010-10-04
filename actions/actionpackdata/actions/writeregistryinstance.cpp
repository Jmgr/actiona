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

#include "actionwriteregistryinstance.h"
#include "actionreadregistryinstance.h"
#include "actioninstanceexecutionhelper.h"

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

void ActionWriteRegistryInstance::startExecution()
{
	ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
#ifdef Q_WS_WIN
	ActionTools::Registry::Key key;
	QString subKey;
	QString value;
	QVariant data;

	if(!actionInstanceExecutionHelper.evaluateListElement(key, ActionReadRegistryInstance::keys, "key") ||
	   !actionInstanceExecutionHelper.evaluateString(subKey, "subKey") ||
	   !actionInstanceExecutionHelper.evaluateString(value, "value") ||
	   !actionInstanceExecutionHelper.evaluateVariant(data, "data"))
		return;

	switch(ActionTools::Registry::write(data, key, subKey, value))
	{
	case ActionTools::Registry::WriteCannotFindSubKey:
		actionInstanceExecutionHelper.setCurrentParameter("subKey");
		emit executionException(CannotFindSubKeyException, tr("Cannot find subKey \"%1\"").arg(subKey));
		return;
	case ActionTools::Registry::WriteCannotWriteValue:
		actionInstanceExecutionHelper.setCurrentParameter("value");
		emit executionException(CannotWriteValueException, tr("Cannot write value \"%1\"").arg(value));
		return;
	default:
		break;
	}
#endif

	emit executionEnded();
}
