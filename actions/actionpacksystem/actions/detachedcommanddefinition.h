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

#ifndef DETACHEDCOMMANDDEFINITION_H
#define DETACHEDCOMMANDDEFINITION_H

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
	class DetachedCommandDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit DetachedCommandDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::FileParameterDefinition *command = new ActionTools::FileParameterDefinition(ActionTools::Name("command", tr("Command")), this);
			command->setTooltip(tr("The command to execute"));
			addElement(command);

			ActionTools::TextParameterDefinition *parameters = new ActionTools::TextParameterDefinition(ActionTools::Name("parameters", tr("Parameters")), this);
			parameters->setTooltip(tr("The command's parameters"));
			addElement(parameters);

			ActionTools::FileParameterDefinition *workingDirectory = new ActionTools::FileParameterDefinition(ActionTools::Name("workingDirectory", tr("Working directory")), this);
			workingDirectory->setTooltip(tr("The command's working directory"));
			workingDirectory->setCaption(tr("Command working directory"));
			workingDirectory->setMode(ActionTools::FileEdit::DirectoryOpen);
			addElement(workingDirectory);

			ActionTools::VariableParameterDefinition *processId = new ActionTools::VariableParameterDefinition(ActionTools::Name("processId", tr("Process id")), this);
			processId->setTooltip(tr("The command's process id"));
			addElement(processId, 1);

			addException(DetachedCommandInstance::DetachedCommandFailedException, tr("Unable to execute the detached command"));
		}

		QString name() const													{ return QObject::tr("Detached command"); }
		QString id() const														{ return "ActionDetachedCommand"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Executes a detached command"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new DetachedCommandInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::System; }
		QPixmap icon() const													{ return QPixmap(":/icons/command.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(DetachedCommandDefinition)
	};
}

#endif // DETACHEDCOMMANDDEFINITION_H
