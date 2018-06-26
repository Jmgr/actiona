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
#include "readbinaryfileinstance.h"
#include "fileparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ReadBinaryFileDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit ReadBinaryFileDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            auto &file = addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("file"), tr("File")});
            file.setTooltip(tr("The file to read from"));
            file.setMode(ActionTools::FileEdit::FileOpen);
            file.setCaption(tr("Choose the file"));
            file.setFilter(tr("All files (*.*)"));

            auto &variable = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("variable"), tr("Variable")});
            variable.setTooltip(tr("The variable where to store the data"));

			addException(ReadBinaryFileInstance::UnableToReadFileException, tr("Unable to read file"));
		}

		QString name() const override													{ return QObject::tr("Read binary file"); }
		QString id() const override														{ return QStringLiteral("ActionReadBinaryFile"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Read a binary file"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new ReadBinaryFileInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/readbinary.png")); }

	private:
		Q_DISABLE_COPY(ReadBinaryFileDefinition)
	};
}

