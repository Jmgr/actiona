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

#ifndef ACTIONREADBINARYFILEDEFINITION_H
#define ACTIONREADBINARYFILEDEFINITION_H

#include "actiondefinition.h"
#include "actionreadbinaryfileinstance.h"
#include "fileparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionReadBinaryFileDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionReadBinaryFileDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition("file", tr("File"), this);
		file->setTooltip(tr("The file to read from"));
		file->setMode(ActionTools::FileEdit::FileOpen);
		file->setCaption(tr("Choose the file"));
		file->setFilter(tr("All files (*.*)"));
		addElement(file);

		ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition("variable", tr("Variable"), this);
		variable->setTooltip(tr("The variable where to store the data"));
		addElement(variable);

		addException(ActionReadBinaryFileInstance::UnableToReadFileException, tr("Unable to read file"));
	}

	QString name() const													{ return QObject::tr("Read binary file"); }
	QString id() const														{ return "ActionReadBinaryFile"; }
	Flag flags() const														{ return ActionDefinition::flags() | Official; }
	QString description() const												{ return QObject::tr("Read a binary file"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionReadBinaryFileInstance(this); }
	Category category() const												{ return Data; }
	QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }

private:
	Q_DISABLE_COPY(ActionReadBinaryFileDefinition)
};

#endif // ACTIONREADBINARYFILEDEFINITION_H
