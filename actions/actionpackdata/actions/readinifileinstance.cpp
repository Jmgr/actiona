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

#include "readinifileinstance.h"
#include "stringlistpair.h"

#include <config.h>

namespace Actions
{
	ActionTools::StringListPair ReadIniFileInstance::modes = qMakePair(
            QStringList() << "singleParameter" << "wholeFile",
            QStringList() << QT_TRANSLATE_NOOP("ReadIniFileInstance::modes", "Read a single parameter") << QT_TRANSLATE_NOOP("ReadIniFileInstance::modes", "Read the entire file"));

	void ReadIniFileInstance::startExecution()
	{
		bool ok = true;

		QString filename = evaluateString(ok, "file");
		QString variable = evaluateVariable(ok, "variable");
		Mode mode = evaluateListElement<Mode>(ok, modes, "mode");

		if(!ok)
			return;

        if(filename.isEmpty())
        {
            emit executionException(UnableToReadFileException, tr("Unable to read the file"));
            return;
        }

        rude::Config config;
        if(!config.load(filename.toLocal8Bit()))
        {
            setCurrentParameter("filename");
            emit executionException(UnableToReadFileException, tr("Unable to read the file"));
            return;
        }

        if(mode == WholeFile)
		{
            QHash<QString, QString> fileContent;
            int sectionCount = config.getNumSections();

            for(int sectionIndex = 0; sectionIndex < sectionCount; ++sectionIndex)
            {
                QString sectionName = QString::fromLatin1(config.getSectionNameAt(sectionIndex));

                if(!config.setSection(sectionName.toLatin1(), false))
                {
                    emit executionException(UnableToReadFileException, tr("Unable to read the file"));
                    return;
                }

                int parameterCount = config.getNumDataMembers();

                for(int parameterIndex = 0; parameterIndex < parameterCount; ++parameterIndex)
                {
                    QString parameterName = QString::fromLatin1(config.getDataNameAt(parameterIndex));
                    QString parameterValue = QString::fromLatin1(config.getStringValue(parameterName.toLatin1()));

                    fileContent[sectionName + "/" + parameterName] = parameterValue;
                }
            }

            setArrayKeyValue(variable, fileContent);
        }
        else
        {
            QString section = evaluateString(ok, "section");
            QString parameter = evaluateString(ok, "parameter");

            if(!ok)
                return;

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
