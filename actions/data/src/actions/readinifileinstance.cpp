/*
	Actiona
    Copyright (C) 2005 Jonathan Mercier-Ganady

	Actiona is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actiona is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact: jmgr@jmgr.info
*/

#include "readinifileinstance.hpp"
#include "tools/stringlistpair.hpp"
#include "mini/ini.h"

namespace Actions
{
    Tools::StringListPair ReadIniFileInstance::modes =
    {
        {
            QStringLiteral("singleParameter"),
            QStringLiteral("wholeFile")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("ReadIniFileInstance::modes", "Read a single parameter")),
            QStringLiteral(QT_TRANSLATE_NOOP("ReadIniFileInstance::modes", "Read the entire file"))
        }
    };

	void ReadIniFileInstance::startExecution()
	{
		bool ok = true;

		QString filename = evaluateString(ok, QStringLiteral("file"));
		QString variable = evaluateVariable(ok, QStringLiteral("variable"));
		Mode mode = evaluateListElement<Mode>(ok, modes, QStringLiteral("mode"));

		if(!ok)
			return;

        if(filename.isEmpty())
        {
            emit executionException(UnableToReadFileException, tr("Unable to read the file"));
            return;
        }

        mINI::INIStructure structure;
        mINI::INIFile file(filename.toStdString());
        if(!file.read(structure))
        {
            setCurrentParameter(QStringLiteral("filename"));
            emit executionException(UnableToReadFileException, tr("Unable to read the file"));
            return;
        }

        if(mode == WholeFile)
		{
            QHash<QString, QString> fileContent;

            for(const auto &section: structure)
            {
                const auto &collection = section.second;
                for(const auto &entry: collection)
                    fileContent[QString::fromStdString(section.first + "/" + entry.first)] = QString::fromStdString(entry.second);
            }

            setArrayKeyValue(variable, fileContent);
        }
        else
        {
            QString sectionName = evaluateString(ok, QStringLiteral("section"));
            QString parameterName = evaluateString(ok, QStringLiteral("parameter"));

            if(!ok)
                return;

            if(!structure.has(sectionName.toStdString()))
            {
                setCurrentParameter(QStringLiteral("section"));
                emit executionException(UnableToFindSectionException, tr("Unable to find the section named \"%1\"").arg(sectionName));
                return;
            }

            const auto &section = structure.get(sectionName.toStdString());

            if(!section.has(parameterName.toStdString()))
            {
                setCurrentParameter(QStringLiteral("parameter"));
                emit executionException(UnableToFindSectionException, tr("Unable to find the parameter named \"%1\"").arg(parameterName));
                return;
            }

            setVariable(variable, QString::fromStdString(section.get(parameterName.toStdString())));
        }

		executionEnded();
	}
}
