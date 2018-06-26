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
#include "copyfileinstance.h"
#include "fileparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class CopyFileDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit CopyFileDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			auto &source = addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("source"), tr("Source file")});
            source.setTooltip(tr("The file to copy"));
            source.setMode(ActionTools::FileEdit::FileOpen);
            source.setCaption(tr("Choose the file"));
            source.setFilter(tr("All files (*.*)"));

			auto &destination = addParameter<ActionTools::FileParameterDefinition>({QStringLiteral("destination"), tr("Destination")});
            destination.setTooltip(tr("The destination file"));
            destination.setMode(ActionTools::FileEdit::FileSave);
            destination.setCaption(tr("Choose the destination file"));
            destination.setFilter(tr("All files (*.*)"));

			addException(CopyFileInstance::UnableToReadFileException, tr("Unable to read source file"));
			addException(CopyFileInstance::UnableToWriteFileException, tr("Unable to write destination file"));
		}

		QString name() const override													{ return QObject::tr("Copy file"); }
		QString id() const override														{ return QStringLiteral("ActionCopyFile"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Copy a file"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new CopyFileInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/copyfile.png")); }

	private:
		Q_DISABLE_COPY(CopyFileDefinition)
	};
}

