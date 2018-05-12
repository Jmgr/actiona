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
#include "writebinaryfileinstance.h"
#include "fileparameterdefinition.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WriteBinaryFileDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit WriteBinaryFileDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition(ActionTools::Name(QStringLiteral("file"), tr("File")), this);
			file->setTooltip(tr("The file to write to"));
			file->setMode(ActionTools::FileEdit::FileSave);
			file->setCaption(tr("Choose the file"));
			file->setFilter(tr("All files (*.*)"));
			addElement(file);

			ActionTools::TextParameterDefinition *data = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("data"), tr("Data")), this);
			data->setTooltip(tr("The data to write to the file"));
			addElement(data);

			addException(WriteBinaryFileInstance::UnableToWriteFileException, tr("Unable to write to the file"));
		}

		QString name() const													{ return QObject::tr("Write binary file"); }
		QString id() const														{ return QStringLiteral("ActionWriteBinaryFile"); }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Write to a binary file"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new WriteBinaryFileInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Data; }
		QPixmap icon() const													{ return QPixmap(QStringLiteral(":/icons/writebinary.png")); }

	private:
		Q_DISABLE_COPY(WriteBinaryFileDefinition)
	};
}

