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

	Contact : jmgr@jmgr.info
*/

#pragma once

#include "actioninstance.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

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
        bool write(const QString &filename, QString section, const QString &parameter, const QString &value)
		{
            try
            {
                boost::property_tree::ptree tree;

                QFileInfo fileInfo(filename);

                if(fileInfo.isReadable())
                    boost::property_tree::ini_parser::read_ini(filename.toStdString(), tree);

                //Create/get the parameter and the value
                boost::property_tree::ptree sectionTree;
                if(tree.count(section.toStdString()) > 0)
                    sectionTree = tree.get_child(section.toStdString());

                sectionTree.put(parameter.toStdString(), value.toStdString());

                if(section.isEmpty())
                {
					setCurrentParameter(QStringLiteral("filename"));
                    emit executionException(UnableToWriteFileException, tr("Unable to write to the file: the section name cannot be empty"));

                    return false;
                }

                //Create the section
                tree.put_child(section.toStdString(), sectionTree);

                boost::property_tree::ini_parser::write_ini(filename.toStdString(), tree);
            }
            catch(const std::runtime_error &e)
            {
				setCurrentParameter(QStringLiteral("filename"));
				emit executionException(UnableToWriteFileException, tr("Unable to write to the file: %1").arg(QString::fromUtf8(e.what())));

                return false;
            }

            return true;
		}

		Q_DISABLE_COPY(WriteIniFileInstance)
	};
}

