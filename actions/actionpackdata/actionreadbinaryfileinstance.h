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

#ifndef ACTIONREADBINARYFILEINSTANCE_H
#define ACTIONREADBINARYFILEINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"

#include <QFile>

class ActionReadBinaryFileInstance : public ActionTools::ActionInstance
{
	Q_OBJECT

public:
	enum Exceptions
	{
		UnableToReadFileException = ActionTools::ActionException::UserException
	};

	ActionReadBinaryFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
		: ActionTools::ActionInstance(definition, parent)											{}

	void startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		QString filename;
		QString variable;

		if(!actionInstanceExecutionHelper.evaluateString(filename, "file") ||
		   !actionInstanceExecutionHelper.evaluateVariable(variable, "variable"))
			return;
		
		QFile file(filename);
		if(!file.open(QIODevice::ReadOnly))
		{
			actionInstanceExecutionHelper.setCurrentParameter("file");
			emit executionException(UnableToReadFileException, tr("Unable to read the file \"%1\"").arg(filename));
			return;
		}
		
		actionInstanceExecutionHelper.setVariable(variable, file.readAll());
		
		emit executionEnded();
	}

private:
	Q_DISABLE_COPY(ActionReadBinaryFileInstance)
};

#endif // ACTIONREADBINARYFILEINSTANCE_H
