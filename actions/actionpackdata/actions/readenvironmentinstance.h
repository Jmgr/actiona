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
#include <QProcess>
#include <QStringList>

//temp. time to find how to coorectly read the environment
#include <QClipboard>

namespace Actions
{
	class ReadEnvironmentInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		ReadEnvironmentInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)												{}

		void startExecution()
		{
			bool ok = true;

			QString variable = evaluateVariable(ok, "variable");

			if(!ok)
				return;

			//is variable an array ? (or set it ?)
			//QStringList ListVE=QProcess::systemEnvironment();
			//how to affect a QStringList to a variable ???

			//while we have not the answer, just read the clipboard :-(
			QClipboard *clipboard = QApplication::clipboard();

			setVariable(variable, clipboard->text());

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(ReadEnvironmentInstance)
	};
}

#endif // READENVIRONMENTINSTANCE_H
