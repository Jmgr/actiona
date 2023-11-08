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

#include "actiontools/actiondefinition.hpp"
#include "readenvironmentinstance.hpp"
#include "actiontools/variableparameterdefinition.hpp"
#include "actiontools/listparameterdefinition.hpp"
#include "actiontools/environmentvariableparameterdefinition.hpp"
#include "actiontools/groupdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ReadEnvironmentVariableDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit ReadEnvironmentVariableDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            translateItems("ReadEnvironmentVariableInstance::modes", ReadEnvironmentVariableInstance::modes);

			auto &variable = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("variable"), tr("Variable")});
            variable.setTooltip(tr("The variable used to store the selected information from your system environment"));

			auto &mode = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("mode"), tr("Mode")});
            mode.setTooltip(tr("The environment read mode"));
            mode.setItems(ReadEnvironmentVariableInstance::modes);
            mode.setDefaultValue(ReadEnvironmentVariableInstance::modes.second.at(ReadEnvironmentVariableInstance::oneVariableMode));

            auto &selectionMode = addGroup();
            selectionMode.setMasterList(mode);
            selectionMode.setMasterValues({ReadEnvironmentVariableInstance::modes.first.at(ReadEnvironmentVariableInstance::oneVariableMode)});

            auto &environmentVariableName = selectionMode.addParameter<ActionTools::EnvironmentVariableParameterDefinition>({QStringLiteral("environmentVariableName"), tr("Environment Variable")});
            environmentVariableName.setTooltip(tr("The specific environment variable to read"));
		}

        QString name() const override													{ return QObject::tr("Read environment variable"); }
		QString id() const override														{ return QStringLiteral("ActionReadEnvironmentVariable"); }
        QString description() const override												{ return QObject::tr("Read a single or multiple environment variables"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new ReadEnvironmentVariableInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/readenvironment.png")); }

	private:
		Q_DISABLE_COPY(ReadEnvironmentVariableDefinition)
	};
}

