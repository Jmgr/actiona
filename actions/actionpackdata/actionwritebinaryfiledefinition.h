/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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

#ifndef ACTIONWRITEBINARYFILEDEFINITION_H
#define ACTIONWRITEBINARYFILEDEFINITION_H

#include "actiondefinition.h"
#include "actionwritebinaryfileinstance.h"
#include "fileparameterdefinition.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionWriteBinaryFileDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionWriteBinaryFileDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition("file", tr("File"), this);
		file->setTooltip(tr("The file to write to"));
		file->setMode(ActionTools::FileEdit::FileSave);
		file->setCaption(tr("Choose the file"));
		file->setFilter(tr("All files (*.*)"));
		addElement(file);

		ActionTools::TextParameterDefinition *data = new ActionTools::TextParameterDefinition("data", tr("Data"), this);
		data->setTooltip(tr("The data to write to the file"));
		addElement(data);

		addException(ActionWriteBinaryFileInstance::UnableToWriteFileException, tr("Unable to write to the file"));
	}

	QString name() const													{ return QObject::tr("Write binary file"); }
	QString id() const														{ return "ActionWriteBinaryFile"; }
	Flag flags() const														{ return ActionDefinition::flags() | Official; }
	QString description() const												{ return QObject::tr("Write to a binary file"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionWriteBinaryFileInstance(this); }
	Category category() const												{ return Data; }
	QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }

private:
	Q_DISABLE_COPY(ActionWriteBinaryFileDefinition)
};

#endif // ACTIONWRITEBINARYFILEDEFINITION_H