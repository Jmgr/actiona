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

	Contact: jmgr@jmgr.info
*/

#include "writeregistryinstance.hpp"
#include "readregistryinstance.hpp"

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

namespace Actions
{
	void WriteRegistryInstance::startExecution()
	{
	#ifdef Q_OS_WIN
		bool ok = true;

        ActionTools::Registry::Key key = evaluateListElement<ActionTools::Registry::Key>(ok, ReadRegistryInstance::keys, QStringLiteral("key"));
        QString subKey = evaluateString(ok, QStringLiteral("subKey"));
        QString value = evaluateString(ok, QStringLiteral("value"));
        QJSValue data = evaluateValue(ok, QStringLiteral("data"));

		if(!ok)
			return;

        switch(ActionTools::Registry::write(data.toVariant(), key, subKey, value))
		{
		case ActionTools::Registry::WriteCannotFindSubKey:
            setCurrentParameter(QStringLiteral("subKey"));
			emit executionException(CannotFindSubKeyException, tr("Cannot find subKey \"%1\"").arg(subKey));
			return;
		case ActionTools::Registry::WriteCannotWriteValue:
            setCurrentParameter(QStringLiteral("value"));
			emit executionException(CannotWriteValueException, tr("Cannot write value \"%1\"").arg(value));
			return;
		default:
			break;
		}
	#endif

		executionEnded();
	}
}
