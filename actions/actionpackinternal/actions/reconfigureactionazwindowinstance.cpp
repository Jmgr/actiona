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

#include "reconfigureactionazwindowinstance.h"

#include <QApplication>
#include <QDesktopWidget>

namespace Actions
{
	const QRegExp ReconfigureActionazWindowInstance::mPositionExp("^Screen[0-9]:(N|NE|NW|S|SW|SE|W|E|C)$", Qt::CaseInsensitive, QRegExp::RegExp2);

	const QStringList ReconfigureActionazWindowInstance::screenPositions =
			QStringList() << "NW" << "W" << "SW" << "N" << "C" << "S" << "NE" << "E" << "SE";

	void ReconfigureActionazWindowInstance::startExecution()
	{
		bool ok = true;

		bool isExecutionVisible		= evaluateBoolean(ok, "executionPosition", "visible");
		QString executionPosition	= evaluateString(ok, "executionPosition", "windowposition");

		bool isConsoleVisible	= evaluateBoolean(ok, "consolePosition", "visible");
		QString consolePosition	= evaluateString(ok, "consolePosition", "windowposition");

		bool clearConsole		= evaluateBoolean(ok, "clearConsole");
		QPoint sizeOfConsole	= evaluatePoint(ok, "sizeOfConsole");

		if(!ok)
			return;

		//convert <window>Position in screen and pos value
		int consoleScreen;
		int consoleScreenPosition;
		ok = convertPosition(consolePosition, consoleScreen, consoleScreenPosition);
		int executionScreen;
		int executionScreenPosition;
		ok = convertPosition(executionPosition, executionScreen, executionScreenPosition);

		emit executionSetPosition(isExecutionVisible, executionScreen, executionScreenPosition);

		if(isConsoleVisible && sizeOfConsole != QPoint())
			emit consoleResize(sizeOfConsole.x(), sizeOfConsole.y());

		emit consoleSetPosition(isConsoleVisible, consoleScreen, consoleScreenPosition);

		if(clearConsole)
			emit consoleClear();

		emit executionEnded();
	}

	bool ReconfigureActionazWindowInstance::convertPosition(QString &stringPosition, int &screenNumber, int &screenPosition)
	{
		bool ok;

		//validate stringPosition:
		//match Screen\d:(NW|N|NE|W|C|E|SW|S|SE) ?
		ok = stringPosition.contains(mPositionExp);

		if(!ok)
			return ok;

		//decode screenNumber:
		screenNumber = stringPosition[6].digitValue();
		--screenNumber; //screenNumber is 0 based

		if((screenNumber < 0) || (screenNumber >= QApplication::desktop()->screenCount()))
			screenNumber = 0; //defaultScreen

		//decode screenPosition
		screenPosition = screenPositions.indexOf(stringPosition.split(":").at(1));

		if( screenPosition == -1)
			screenPosition = 4; //defaultPosition

		return ok;
	}
}

