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

#include "actiondefinition.h"
#include "detachedcommandinstance.h"
#include "fileparameterdefinition.h"
#include "variableparameterdefinition.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class DetachedCommandDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit DetachedCommandDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            auto &command = addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("command"), tr("Command")});
            command.setTooltip(tr("The command to execute"));

            auto &parameters = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("parameters"), tr("Parameters")});
            parameters.setTooltip(tr("The command's parameters"));

            auto &workingDirectory = addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("workingDirectory"), tr("Working directory")});
            workingDirectory.setTooltip(tr("The command's working directory"));
            workingDirectory.setCaption(tr("Command working directory"));
            workingDirectory.setMode(ActionTools::FileEdit::DirectoryOpen);

            auto &processId = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("processId"), tr("Process id")}, 1);
            processId.setTooltip(tr("The command's process id"));

			addException(DetachedCommandInstance::DetachedCommandFailedException, tr("Unable to execute the detached command"));
		}

		QString name() const override													{ return QObject::tr("Detached command"); }
		QString id() const override														{ return QStringLiteral("ActionDetachedCommand"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Executes a detached command"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new DetachedCommandInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::System; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/command.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(DetachedCommandDefinition)
	};
}

