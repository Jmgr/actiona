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

#ifndef RECONFIGUREACTIONAZWINDOWINSTANCE_H
#define RECONFIGUREACTIONAZWINDOWINSTANCE_H

#include "actioninstance.h"

#include <QDebug>
#include <QPoint>

namespace Actions
{
	class ReconfigureActionazWindowInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		ReconfigureActionazWindowInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)										{}

		void startExecution()
		{
			bool ok = true;

			int executionVisible		= evaluateBoolean(ok, "executionPosition", "visible");
			QString executionPosition	= evaluateString(ok, "executionPosition", "windowposition");

			int consoleVisible		= evaluateBoolean(ok, "consolePosition", "visible");
			QString consolePosition	= evaluateString(ok, "consolePosition", "windowposition");

			bool clearConsole		= evaluateBoolean(ok, "clearConsole");
			QPoint sizeOfConsole	= evaluatePoint(ok, "sizeOfConsole");

			if(!ok)
				return;

			//TODO: emit the signals
			if(clearConsole)
				emit clearConsole();

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(ReconfigureActionazWindowInstance)
	};
}

#endif // RECONFIGUREACTIONAZWINDOWINSTANCE_H
