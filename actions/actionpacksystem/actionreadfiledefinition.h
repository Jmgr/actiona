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

#ifndef ACTIONREADFILEDEFINITION_H
#define ACTIONREADFILEDEFINITION_H

#include "actiondefinition.h"
#include "actionreadfileinstance.h"
#include "fileparameterdefinition.h"
#include "variableparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "listparameterdefinition.h"
#include "groupdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionReadFileDefinition : public QObject, public ActionTools::ActionDefinition
{
	Q_OBJECT

public:
	explicit ActionReadFileDefinition(ActionTools::ActionPack *pack)
        : ActionDefinition(pack)
	{
		ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition( ActionTools::ElementDefinition::INPUT,
																							   "file",
																							   tr("File"),
																							   this);
		file->setTooltip(tr("The file you want to read"));
		addElement(file);

		ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition( ActionTools::ElementDefinition::OUTPUT,
																										 "variable",
																										 tr("Variable"),
																										 this);
		variable->setTooltip(tr("The variable where to save the text read from the file"));
		addElement(variable);

		ActionTools::ListParameterDefinition *mode = new ActionTools::ListParameterDefinition( ActionTools::ElementDefinition::INPUT,
																								 "mode",
																								 tr("Mode"),
																								 this);
		mode->setTooltip(tr("The file read mode"));
		mode->setItems(ActionReadFileInstance::modes);
		mode->setDefaultValue(ActionReadFileInstance::modes.second.at(ActionReadFileInstance::Full));
		addElement(mode, 1);

		ActionTools::GroupDefinition *selectionMode = new ActionTools::GroupDefinition(		ActionTools::ElementDefinition::INPUT,
																							"selectionmode",
																							tr("Selection mode"),
																							this);
		selectionMode->setMasterList(mode);
		selectionMode->setMasterValues(QStringList() << ActionReadFileInstance::modes.first.at(ActionReadFileInstance::Selection));

		ActionTools::NumberParameterDefinition *firstline = new ActionTools::NumberParameterDefinition( ActionTools::ElementDefinition::INPUT,
																										"firstline",
																										tr("First line"),
																										this);
		firstline->setTooltip(tr("The line where to start reading the file"));
		firstline->setMinimum(1);
		firstline->setDefaultValue(1);
		selectionMode->addMember(firstline, 1);

		ActionTools::NumberParameterDefinition *lastline = new ActionTools::NumberParameterDefinition( ActionTools::ElementDefinition::INPUT,
																									   "lastline",
																									   tr("Last line"),
																									   this);
		lastline->setTooltip(tr("The line where to stop reading the file"));
		lastline->setMinimum(1);
		lastline->setDefaultValue(1);
		selectionMode->addMember(lastline, 1);

		addElement(selectionMode, 1);

		addException(ActionReadFileInstance::CannotOpenFileException, tr("Cannot read file"));
	}

	QString name() const													{ return QObject::tr("Read file"); }
	QString id() const														{ return "ActionReadFile"; }
	Flag flags() const														{ return ActionDefinition::flags() | Official; }
	QString description() const												{ return QObject::tr("Read a plain text file"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionReadFileInstance(this); }
	Category category() const												{ return System; }
	QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }
	QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

private:
	Q_DISABLE_COPY(ActionReadFileDefinition)
};

#endif // ACTIONREADFILEDEFINITION_H
