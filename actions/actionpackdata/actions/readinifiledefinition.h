/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#ifndef READINIFILEDEFINITION_H
#define READINIFILEDEFINITION_H

#include "actiondefinition.h"
#include "readinifileinstance.h"
#include "textparameterdefinition.h"
#include "fileparameterdefinition.h"
#include "variableparameterdefinition.h"
#include "booleanparameterdefinition.h"
#include "groupdefinition.h"


namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class ReadIniFileDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit ReadIniFileDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("ReadIniFileInstance::modes", ReadIniFileInstance::modes);

			ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition(ActionTools::Name("file", tr("File")), this);
			file->setTooltip(tr("The file to read from"));
			file->setMode(ActionTools::FileEdit::FileOpen);
			file->setCaption(tr("Choose the INI file"));
			file->setFilter(tr("INI files (*.ini);;All files (*.*)"));
			addElement(file);

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition(ActionTools::Name("variable", tr("Variable")), this);
			variable->setTooltip(tr("The variable where to store the data"));
			addElement(variable);

			ActionTools::ListParameterDefinition *mode = new ActionTools::ListParameterDefinition(ActionTools::Name("mode", tr("Mode")), this);
			mode->setTooltip(tr("The INI file read mode"));
			mode->setItems(ReadIniFileInstance::modes);
			mode->setDefaultValue(ReadIniFileInstance::modes.second.at(ReadIniFileInstance::Full));
			addElement(mode, 1);

			ActionTools::GroupDefinition *selectionMode = new ActionTools::GroupDefinition(this);
			selectionMode->setMasterList(mode);
			selectionMode->setMasterValues(QStringList() << ReadIniFileInstance::modes.first.at(ReadIniFileInstance::Selection));

			ActionTools::TextParameterDefinition *section = new ActionTools::TextParameterDefinition(ActionTools::Name("section", tr("Section")), this);
			section->setTooltip(tr("The section name of the parameter"));
			selectionMode->addMember(section, 1);

			ActionTools::TextParameterDefinition *parameter = new ActionTools::TextParameterDefinition(ActionTools::Name("parameter", tr("Parameter")), this);
			parameter->setTooltip(tr("The parameter name"));
			selectionMode->addMember(section, 1);

			addException(ReadIniFileInstance::UnableToReadFileException, tr("Unable to read file"));
			addException(ReadIniFileInstance::UnableToFindSectionException, tr("Unable to find section"));
		}

		QString name() const													{ return QObject::tr("Read INI file"); }
		QString id() const														{ return "ActionReadIniFile"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Read one or all the entries in an INI file"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new ReadIniFileInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Data; }
		QPixmap icon() const													{ return QPixmap(":/icons/readini.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(ReadIniFileDefinition)
	};
}

#endif // READINIFILEDEFINITION_H
