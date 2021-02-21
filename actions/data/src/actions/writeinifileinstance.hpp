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

#pragma once

#include "actiontools/actioninstance.hpp"
#include "mini/ini.h"

#include <QFileInfo>

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

		WriteIniFileInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr)
			: ActionTools::ActionInstance(definition, parent)											{}

		void startExecution() override
		{
			bool ok = true;

			QString filename = evaluateString(ok, QStringLiteral("file"));
			QString section = evaluateString(ok, QStringLiteral("section"));
			QString parameter = evaluateString(ok, QStringLiteral("parameter"));
			QString value = evaluateString(ok, QStringLiteral("value"));

			if(!ok)
				return;

			if(!write(filename, section, parameter, value))
				return;

			executionEnded();
		}

	private:
        bool write(const QString &filename, const QString &sectionName, const QString &parameterName, const QString &value)
		{
            mINI::INIStructure structure;
            QFileInfo fileInfo(filename);

            if(fileInfo.isReadable())
            {
                mINI::INIFile file(filename.toStdString());
                if(!file.read(structure))
                {
                    setCurrentParameter(QStringLiteral("filename"));
                    emit executionException(UnableToWriteFileException, tr("Unable to read the existing file"));

                    return false;
                }
            }

            auto &section = structure[sectionName.toStdString()];

            section[parameterName.toStdString()] = value.toStdString();

            mINI::INIFile file(filename.toStdString());
            if(!file.write(structure))
            {
                setCurrentParameter(QStringLiteral("filename"));
                emit executionException(UnableToWriteFileException, tr("Unable to write to the file"));

                return false;
            }

            return true;
		}

		Q_DISABLE_COPY(WriteIniFileInstance)
	};
}

