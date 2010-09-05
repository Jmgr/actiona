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

#ifndef ACTIONWRITEINIFILEDEFINITION_H
#define ACTIONWRITEINIFILEDEFINITION_H

#include "actiondefinition.h"
#include "actionwriteinifileinstance.h"
#include "textparameterdefinition.h"
#include "fileparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionWriteIniFileDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionWriteIniFileDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition("file", tr("File"), this);
		file->setTooltip(tr("The file to create or edit"));
		file->setMode(ActionTools::FileEdit::FileSave);
		file->setCaption(tr("Choose the INI file"));
		file->setFilter(tr("INI files (*.ini);;All files (*.*)"));
		addElement(file);
		
		ActionTools::TextParameterDefinition *section = new ActionTools::TextParameterDefinition("section", tr("Section"), this);
		section->setTooltip(tr("The section name of the parameter"));
		addElement(section);
		
		ActionTools::TextParameterDefinition *parameter = new ActionTools::TextParameterDefinition("parameter", tr("Parameter"), this);
		parameter->setTooltip(tr("The parameter name"));
		addElement(parameter);
		
		ActionTools::TextParameterDefinition *value = new ActionTools::TextParameterDefinition("value", tr("Value"), this);
		value->setTooltip(tr("The new value"));
		addElement(value);
		
		addException(ActionWriteIniFileInstance::UnableToWriteFileException, tr("Unable to write file"));
	}

	QString name() const													{ return QObject::tr("Write INI file"); }
	QString id() const														{ return "ActionWriteIniFile"; }
	Flag flags() const														{ return ActionDefinition::flags() | Official; }
	QString description() const												{ return QObject::tr("Write an entry in an INI file"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionWriteIniFileInstance(this); }
	Category category() const												{ return Data; }
	QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }

private:
	Q_DISABLE_COPY(ActionWriteIniFileDefinition)
};

#endif // ACTIONWRITEINIFILEDEFINITION_H
