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

#ifndef ACTIONREADINIFILEINSTANCE_H
#define ACTIONREADINIFILEINSTANCE_H

#include "actioninstanceexecutionhelper.h"
#include "actioninstance.h"

#include <config.h>

class ActionReadIniFileInstance : public ActionTools::ActionInstance
{
	Q_OBJECT

public:
	enum Exceptions
	{
		UnableToReadFileException = ActionTools::ActionException::UserException,
		UnableToFindSectionException
	};

	ActionReadIniFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
		: ActionTools::ActionInstance(definition, parent)											{}

	void startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		QString filename;
		QString section;
		QString parameter;
		QString variable;

		if(!actionInstanceExecutionHelper.evaluateString(filename, "file") ||
		   !actionInstanceExecutionHelper.evaluateString(section, "section") ||
		   !actionInstanceExecutionHelper.evaluateString(parameter, "parameter") ||
		   !actionInstanceExecutionHelper.evaluateVariable(variable, "variable"))
			return;

		rude::Config config;
#ifdef Q_WS_WIN
		if(!config.load(filename.toLatin1()))
#else
		if(!config.load(filename.toUtf8()))
#endif
		{
			actionInstanceExecutionHelper.setCurrentParameter("filename");
			emit executionException(UnableToReadFileException, tr("Unable to read the file"));
			return;
		}

		if(!config.setSection(section.toLatin1(), false))
		{
			actionInstanceExecutionHelper.setCurrentParameter("section");
			emit executionException(UnableToFindSectionException, tr("Unable to find the section named \"%1\"").arg(section));
			return;
		}

		actionInstanceExecutionHelper.setVariable(variable, QString::fromLatin1(config.getStringValue(parameter.toLatin1())));

		emit executionEnded();
	}

public slots:
	bool read(const QString &filename, const QString &section, const QString &parameter, QString &value)
	{
		rude::Config config;

#ifdef Q_WS_WIN
		if(!config.load(filename.toLatin1()))
			return false;
#else
		if(!config.load(filename.toUtf8()))
			return false;
#endif

		if(!config.setSection(section.toLatin1(), false))
			return false;

		value = QString::fromLatin1(config.getStringValue(parameter.toLatin1()));

		return true;
	}

private:
	Q_DISABLE_COPY(ActionReadIniFileInstance)
};

#endif // ACTIONREADINIFILEINSTANCE_H
