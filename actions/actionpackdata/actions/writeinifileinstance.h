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

#ifndef WRITEINIFILEINSTANCE_H
#define WRITEINIFILEINSTANCE_H

#include "actioninstance.h"

#include <config.h>

namespace Actions
{
	class WriteIniFileInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum Exceptions
		{
			UnableToWriteFileException = ActionTools::ActionException::UserException
		};

		WriteIniFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)											{}

		void startExecution()
		{
			bool ok = true;

			QString filename = evaluateString(ok, "file");
			QString section = evaluateString(ok, "section");
			QString parameter = evaluateString(ok, "parameter");
			QString value = evaluateString(ok, "value");

			if(!ok)
				return;

			if(!write(filename, section, parameter, value))
			{
				setCurrentParameter("filename");
				emit executionException(UnableToWriteFileException, tr("Unable to write to the file"));
				return;
			}

			emit executionEnded();
		}

	private:
		bool write(const QString &filename, const QString &section, const QString &parameter, const QString &value)
		{
			rude::Config config;

			config.setConfigFile(filename.toLocal8Bit());

			config.load();

			if(!config.setSection(section.toLatin1(), true))
				return false;

			config.setStringValue(parameter.toLatin1(), value.toLatin1());

			if(!config.save())
				return false;

			return true;
		}

		Q_DISABLE_COPY(WriteIniFileInstance)
	};
}

#endif // WRITEINIFILEINSTANCE_H
