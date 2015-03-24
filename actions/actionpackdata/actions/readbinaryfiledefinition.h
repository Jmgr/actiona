/*
	Actiona
	Copyright (C) 2008-2015 Jonathan Mercier-Ganady

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

#ifndef READBINARYFILEDEFINITION_H
#define READBINARYFILEDEFINITION_H

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
	class ReadBinaryFileDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit ReadBinaryFileDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition(ActionTools::Name("file", tr("File")), this);
			file->setTooltip(tr("The file to read from"));
			file->setMode(ActionTools::FileEdit::FileOpen);
			file->setCaption(tr("Choose the file"));
			file->setFilter(tr("All files (*.*)"));
			addElement(file);

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition(ActionTools::Name("variable", tr("Variable")), this);
			variable->setTooltip(tr("The variable where to store the data"));
			addElement(variable);

			addException(ReadBinaryFileInstance::UnableToReadFileException, tr("Unable to read file"));
		}

		QString name() const													{ return QObject::tr("Read binary file"); }
		QString id() const														{ return "ActionReadBinaryFile"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Read a binary file"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new ReadBinaryFileInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Data; }
		QPixmap icon() const													{ return QPixmap(":/icons/readbinary.png"); }

	private:
		Q_DISABLE_COPY(ReadBinaryFileDefinition)
	};
}

#endif // READBINARYFILEDEFINITION_H
