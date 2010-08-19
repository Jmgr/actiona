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

#ifndef ACTIONWRITEINIFILEINSTANCE_H
#define ACTIONWRITEINIFILEINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"

#include <config.h>

class ActionWriteIniFileInstance : public ActionTools::ActionInstance
{
	Q_OBJECT

public:
	enum Exceptions
	{
		UnableToWriteFileException = ActionTools::ActionException::UserException
	};

	ActionWriteIniFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
		: ActionTools::ActionInstance(definition, parent)											{}

	void startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		QString filename;
		QString section;
		QString parameter;
		QString value;

		if(!actionInstanceExecutionHelper.evaluateString(filename, "file") ||
		   !actionInstanceExecutionHelper.evaluateString(section, "section") ||
		   !actionInstanceExecutionHelper.evaluateString(parameter, "parameter") ||
		   !actionInstanceExecutionHelper.evaluateString(value, "value"))
			return;

		if(!write(filename, section, parameter, value))
		{
			actionInstanceExecutionHelper.setCurrentParameter("filename");
			emit executionException(UnableToWriteFileException, tr("Unable to write to the file"));
			return;
		}

		emit executionEnded();
	}

public slots:
	bool write(const QString &filename, const QString &section, const QString &parameter, const QString &value)
	{
		rude::Config config;

#ifdef Q_WS_WIN
		config.setConfigFile(filename.toLatin1());
#else
		config.setConfigFile(filename.toUtf8());
#endif

		if(!config.setSection(section.toLatin1(), true))
			return false;

		config.setStringValue(parameter.toLatin1(), value.toLatin1());

		if(!config.save())
			return false;

		return true;
	}

private:
	Q_DISABLE_COPY(ActionWriteIniFileInstance)
};

#endif // ACTIONWRITEINIFILEINSTANCE_H
