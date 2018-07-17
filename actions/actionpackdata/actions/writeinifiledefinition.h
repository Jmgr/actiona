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
#include "writeinifileinstance.h"
#include "textparameterdefinition.h"
#include "fileparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WriteIniFileDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit WriteIniFileDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			auto &file = addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("file"), tr("File")});
            file.setTooltip(tr("The file to create or edit"));
            file.setMode(ActionTools::FileEdit::FileSave);
            file.setCaption(tr("Choose the INI file"));
            file.setFilter(tr("INI files (*.ini);;All files (*.*)"));

			auto &section = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("section"), tr("Section")});
            section.setTooltip(tr("The section name of the parameter"));

			auto &parameter = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("parameter"), tr("Parameter")});
            parameter.setTooltip(tr("The parameter name"));

			auto &value = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("value"), tr("Value")});
            value.setTooltip(tr("The new value"));

			addException(WriteIniFileInstance::UnableToWriteFileException, tr("Unable to write file"));
		}

		QString name() const override													{ return QObject::tr("Write INI file"); }
		QString id() const override														{ return QStringLiteral("ActionWriteIniFile"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Write an entry in an INI file"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new WriteIniFileInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/writeini.png")); }

	private:
		Q_DISABLE_COPY(WriteIniFileDefinition)
	};
}

