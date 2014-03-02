/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef CONSOLEDEFINITION_H
#define CONSOLEDEFINITION_H

#include "actiondefinition.h"
#include "consoleinstance.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ConsoleDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit ConsoleDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("ConsoleInstance::outputs", ConsoleInstance::outputs);

			ActionTools::TextParameterDefinition *text = new ActionTools::TextParameterDefinition(ActionTools::Name("text", tr("Text")), this);
			text->setTooltip(tr("The text to write"));
			addElement(text);

			ActionTools::ListParameterDefinition *output = new ActionTools::ListParameterDefinition(ActionTools::Name("output", tr("Output")), this);
			output->setTooltip(tr("The console output"));
			output->setItems(ConsoleInstance::outputs);
			output->setDefaultValue(ConsoleInstance::outputs.second.at(ConsoleInstance::Information));
			addElement(output);
		}

		QString name() const													{ return QObject::tr("Console"); }
		QString id() const														{ return "ActionConsole"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Write an entry in the console"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new ConsoleInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/console.png"); }

	private:
		Q_DISABLE_COPY(ConsoleDefinition)
	};
}

#endif // CONSOLEDEFINITION_H
