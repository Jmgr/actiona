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

#ifndef ACTIONREADINIFILEDEFINITION_H
#define ACTIONREADINIFILEDEFINITION_H

#include "actiondefinition.h"
#include "actionreadinifileinstance.h"
#include "textparameterdefinition.h"
#include "fileparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionReadIniFileDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionReadIniFileDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition("file", tr("File"), this);
		file->setTooltip(tr("The file to read from"));
		file->setMode(ActionTools::FileEdit::FileOpen);
		file->setCaption(tr("Choose the INI file"));
		file->setFilter(tr("INI files (*.ini);;All files (*.*)"));
		addElement(file);

		ActionTools::TextParameterDefinition *section = new ActionTools::TextParameterDefinition("section", tr("Section"), this);
		section->setTooltip(tr("The section name of the parameter"));
		addElement(section);

		ActionTools::TextParameterDefinition *parameter = new ActionTools::TextParameterDefinition("parameter", tr("Parameter"), this);
		parameter->setTooltip(tr("The parameter name"));
		addElement(parameter);

		ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition("variable", tr("Variable"), this);
		variable->setTooltip(tr("The variable where to store the data"));
		addElement(variable);

		addException(ActionReadIniFileInstance::UnableToReadFileException, tr("Unable to read file"));
		addException(ActionReadIniFileInstance::UnableToFindSectionException, tr("Unable to find section"));
	}

	QString name() const													{ return QObject::tr("Read INI file"); }
	QString id() const														{ return "ActionReadIniFile"; }
	Flag flags() const														{ return ActionDefinition::flags() | Official; }
	QString description() const												{ return QObject::tr("Read an entry in an INI file"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionReadIniFileInstance(this); }
	Category category() const												{ return Data; }
	QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }

private:
	Q_DISABLE_COPY(ActionReadIniFileDefinition)
};

#endif // ACTIONREADINIFILEDEFINITION_H
