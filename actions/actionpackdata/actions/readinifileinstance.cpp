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

#include "readinifileinstance.h"

#include <QSettings>

namespace Actions
{
	ActionTools::StringListPair ReadIniFileInstance::modes = qMakePair(
			QStringList() << "full" << "single",
			QStringList() << QT_TRANSLATE_NOOP("ReadIniFileInstance::modes", "Read the entire file") << QT_TRANSLATE_NOOP("ReadIniFileInstance::modes", "Read only a value"));

	void ReadIniFileInstance::startExecution()
	{
		bool ok = true;

		QString filename = evaluateString(ok, "file");
		QString variable = evaluateVariable(ok, "variable");
		Mode mode = evaluateListElement<Mode>(ok, modes, "mode");
		QString section = evaluateString(ok, "section");
		QString parameter = evaluateString(ok, "parameter");

		if(!ok)
			return;

		if(mode == Full)
		{
			QSettings settings(filename, QSettings::IniFormat);

			switch( settings.status())
			{
				case QSettings::FormatError	:
					emit executionException(UnableToDecodeFileException, tr("Bad syntax in the INI the file"));
					return;
				case QSettings::AccessError	:
					emit executionException(UnableToReadFileException, tr("Unable to read the file"));
					return;
				case QSettings::NoError :
					ok = true;
			}

			const QStringList &allParameters = settings.allKeys();

			QHash<QString, QString> hashParametersValues;
			hashParametersValues.reserve(allParameters.count()+2); //doc said ideally 'slightly more than the maximum nb of item'

			foreach(QString parameter, allParameters)
				hashParametersValues[parameter] = settings.value(parameter).toString();

			setArrayKeyValue(variable, hashParametersValues);
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
}

