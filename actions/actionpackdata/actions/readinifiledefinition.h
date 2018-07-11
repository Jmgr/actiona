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
#include "readinifileinstance.h"
#include "textparameterdefinition.h"
#include "fileparameterdefinition.h"
#include "variableparameterdefinition.h"
#include "listparameterdefinition.h"
#include "groupdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ReadIniFileDefinition : public ActionTools::ActionDefinition
	{
		Q_OBJECT

	public:
		explicit ReadIniFileDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("ReadIniFileInstance::modes", ReadIniFileInstance::modes);

            auto &file = addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("file"), tr("File")});
            file.setTooltip(tr("The file to read from"));
            file.setMode(ActionTools::FileEdit::FileOpen);
            file.setCaption(tr("Choose the INI file"));
            file.setFilter(tr("INI files (*.ini);;All files (*.*)"));

            auto &variable = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("variable"), tr("Variable")});
            variable.setTooltip(tr("The variable where to store the data"));

            auto &mode = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("mode"), tr("Mode")});
            mode.setTooltip(tr("The INI file read mode"));
            mode.setItems(ReadIniFileInstance::modes);
            mode.setDefaultValue(ReadIniFileInstance::modes.second.at(ReadIniFileInstance::SingleParameter));

            auto &selectionMode = addGroup();
            selectionMode.setMasterList(mode);
            selectionMode.setMasterValues({ReadIniFileInstance::modes.first.at(ReadIniFileInstance::SingleParameter)});

            auto &section = selectionMode.addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("section"), tr("Section")});
            section.setTooltip(tr("The parameter section"));

            auto &parameter = selectionMode.addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("parameter"), tr("Parameter")});
            parameter.setTooltip(tr("The parameter name"));

			addException(ReadIniFileInstance::UnableToReadFileException, tr("Unable to read file"));
			addException(ReadIniFileInstance::UnableToFindSectionException, tr("Unable to find section"));
		}

		QString name() const override													{ return QObject::tr("Read INI file"); }
		QString id() const override														{ return QStringLiteral("ActionReadIniFile"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Read one or all the entries in an INI file"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new ReadIniFileInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/readini.png")); }

	private:
		Q_DISABLE_COPY(ReadIniFileDefinition)
	};
}

