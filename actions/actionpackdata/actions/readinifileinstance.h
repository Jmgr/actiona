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

#ifndef READINIFILEINSTANCE_H
#define READINIFILEINSTANCE_H

#include "actioninstance.h"

#include <QSettings>
#include <config.h>

namespace Actions
{
	class ReadIniFileInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT

	public:
		enum Exceptions
		{
			UnableToReadFileException = ActionTools::ActionException::UserException,
			UnableToFindSectionException
		};

		ReadIniFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0)
			: ActionTools::ActionInstance(definition, parent)											{}

		void startExecution()
		{
			bool ok = true;

			QString filename = evaluateString(ok, "file");
			QString section = evaluateString(ok, "section");
			QString parameter = evaluateString(ok, "parameter");
			QString variable = evaluateVariable(ok, "variable");
			bool	allFile = evaluateBoolean(ok, "complete");

			if(!ok)
				return;

			if(allFile)
			{
				QSettings settings( filename, QSettings::IniFormat);

				switch( settings.status())
				{
					case QSettings::FormatError	:
						emit executionException(UnableToReadFileException, tr("Bad syntax in the INI the file"));
						return;
					case QSettings::AccessError	:
						emit executionException(UnableToReadFileException, tr("Unable to read the file"));
						return;
					case QSettings::NoError :
						ok = true;
				}

				QStringList Keys = settings.allKeys();
				QStringList Values;

				for( int index = 0; index < Keys.size(); ++index)
					Values << settings.value(Keys.at(index)).toString();

				setArrayKeyValue(variable, Keys, Values);
			}
			else
			{
				rude::Config config;
				if(!config.load(filename.toLocal8Bit()))
				{
					setCurrentParameter("filename");
					emit executionException(UnableToReadFileException, tr("Unable to read the file"));
					return;
				}

				if(!config.setSection(section.toLatin1(), false))
				{
					setCurrentParameter("section");
					emit executionException(UnableToFindSectionException, tr("Unable to find the section named \"%1\"").arg(section));
					return;
				}

				setVariable(variable, QString::fromLatin1(config.getStringValue(parameter.toLatin1())));
			}

			emit executionEnded();
		}

	private:
		Q_DISABLE_COPY(ReadIniFileInstance)
	};
}

#endif // READINIFILEINSTANCE_H
