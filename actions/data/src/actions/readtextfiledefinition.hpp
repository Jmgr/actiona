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
#include "readtextfileinstance.hpp"
#include "actiontools/fileparameterdefinition.hpp"
#include "actiontools/variableparameterdefinition.hpp"
#include "actiontools/numberparameterdefinition.hpp"
#include "actiontools/listparameterdefinition.hpp"
#include "actiontools/groupdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ReadTextFileDefinition : public ActionTools::ActionDefinition
	{
		Q_OBJECT

	public:
		explicit ReadTextFileDefinition(ActionTools::ActionPack *pack)
			: ActionDefinition(pack)
		{
			translateItems("ReadTextFileInstance::modes", ReadTextFileInstance::modes);

			auto &file = addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("file"), tr("File")});
            file.setTooltip(tr("The file you want to read"));

			auto &variable = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("variable"), tr("Variable")});
            variable.setTooltip(tr("The variable where to save the text read from the file"));

            auto &mode = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("mode"), tr("Mode")}, 1);
            mode.setTooltip(tr("The file read mode"));
            mode.setItems(ReadTextFileInstance::modes);
            mode.setDefaultValue(ReadTextFileInstance::modes.second.at(ReadTextFileInstance::Full));

            auto &selectionMode = addGroup(1);
            selectionMode.setMasterList(mode);
            selectionMode.setMasterValues({ReadTextFileInstance::modes.first.at(ReadTextFileInstance::Selection)});

            auto &firstline = selectionMode.addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("firstline"), tr("First line")}, 1);
            firstline.setTooltip(tr("The line where to start reading the file"));
            firstline.setMinimum(1);
            firstline.setDefaultValue(QStringLiteral("1"));

            auto &lastline = selectionMode.addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("lastline"), tr("Last line")}, 1);
            lastline.setTooltip(tr("The line where to stop reading the file"));
            lastline.setMinimum(1);
            lastline.setDefaultValue(QStringLiteral("1"));

			addException(ReadTextFileInstance::CannotOpenFileException, tr("Cannot read file"));
		}

		QString name() const override													{ return QObject::tr("Read text file"); }
		QString id() const override														{ return QStringLiteral("ActionReadTextFile"); }
		QString description() const override												{ return QObject::tr("Read a plain text file"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new ReadTextFileInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/readtext.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(ReadTextFileDefinition)
	};
}

