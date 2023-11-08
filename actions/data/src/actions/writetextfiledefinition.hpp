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
#include "writetextfileinstance.hpp"
#include "actiontools/fileparameterdefinition.hpp"
#include "actiontools/textparameterdefinition.hpp"
#include "actiontools/listparameterdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WriteTextFileDefinition : public ActionTools::ActionDefinition
	{
		Q_OBJECT

	public:
		explicit WriteTextFileDefinition(ActionTools::ActionPack *pack)
			: ActionDefinition(pack)
		{
			translateItems("WriteTextFileInstance::modes", WriteTextFileInstance::modes);

			auto &file = addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("file"), tr("File")});
            file.setTooltip(tr("The file you want to write"));

			auto &text = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("text"), tr("Text")});
            text.setTooltip(tr("The text to write to the file"));

            auto &mode = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("mode"), tr("Mode")}, 1);
            mode.setTooltip(tr("The file write mode"));
            mode.setItems(WriteTextFileInstance::modes);
            mode.setDefaultValue(WriteTextFileInstance::modes.second.at(WriteTextFileInstance::Truncate));

			addException(WriteTextFileInstance::CannotWriteFileException, tr("Cannot write file"));
		}

		QString name() const override													{ return QObject::tr("Write text file"); }
		QString id() const override														{ return QStringLiteral("ActionWriteTextFile"); }
		QString description() const override												{ return QObject::tr("Write a plain text file"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new WriteTextFileInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/writetext.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(WriteTextFileDefinition)
	};
}

