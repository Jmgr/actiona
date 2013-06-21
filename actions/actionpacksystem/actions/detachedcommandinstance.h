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

#ifndef DETACHEDCOMMANDINSTANCE_H
#define DETACHEDCOMMANDINSTANCE_H

#include "actioninstance.h"
#include "script.h"

#include <QProcess>

namespace Actions
{
	class DetachedCommandInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum Exceptions
		{
			DetachedCommandFailedException = ActionTools::ActionException::UserException
		};

		DetachedCommandInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)
		{
		}

		void startExecution()
		{
			bool ok = true;

			QString command = evaluateString(ok, "command");
			QString parameters = evaluateString(ok, "parameters");
			QString workingDirectory = evaluateString(ok, "workingDirectory");
			QString processId = evaluateVariable(ok, "processId");

			if(!ok)
				return;

			QStringList parameterList = parameters.split(QChar(' '));

			qint64 processIdValue;
			if(!QProcess::startDetached(command,
										parameters.isEmpty() ? QStringList() : parameterList,
										workingDirectory,
										&processIdValue))
			{
				emit executionException(DetachedCommandFailedException, tr("Unable to start the detached command"));
				return;
			}

            setVariable(processId, QString::number(processIdValue));

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(DetachedCommandInstance)
	};
}

#endif // DETACHEDCOMMANDINSTANCE_H
