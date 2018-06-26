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
	class ConsoleDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit ConsoleDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("ConsoleInstance::outputs", ConsoleInstance::outputs);

            auto &text = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("text"), tr("Text")});
            text.setTooltip(tr("The text to write"));

            auto &output = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("output"), tr("Output")});
            output.setTooltip(tr("The console output"));
            output.setItems(ConsoleInstance::outputs);
            output.setDefaultValue(ConsoleInstance::outputs.second.at(ConsoleInstance::Information));
		}

		QString name() const override													{ return QObject::tr("Console"); }
		QString id() const override														{ return QStringLiteral("ActionConsole"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Write an entry in the console"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new ConsoleInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Internal; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/console.png")); }

	private:
		Q_DISABLE_COPY(ConsoleDefinition)
	};
}

