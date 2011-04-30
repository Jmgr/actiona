/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef READTEXTFILEDEFINITION_H
#define READTEXTFILEDEFINITION_H

#include "actiondefinition.h"
#include "readtextfileinstance.h"
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

namespace Actions
{
	class ReadTextFileDefinition : public QObject, public ActionTools::ActionDefinition
	{
		Q_OBJECT

	public:
		explicit ReadTextFileDefinition(ActionTools::ActionPack *pack)
			: ActionDefinition(pack)
		{
			translateItems("ReadTextFileInstance::modes", ReadTextFileInstance::modes);

			ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition("file", tr("File"), this);
			file->setTooltip(tr("The file you want to read"));
			addElement(file);

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition("variable", tr("Variable"), this);
			variable->setTooltip(tr("The variable where to save the text read from the file"));
			addElement(variable);

			ActionTools::ListParameterDefinition *mode = new ActionTools::ListParameterDefinition("mode", tr("Mode"), this);
			mode->setTooltip(tr("The file read mode"));
			mode->setItems(ReadTextFileInstance::modes);
			mode->setDefaultValue(ReadTextFileInstance::modes.second.at(ReadTextFileInstance::Full));
			addElement(mode, 1);

			ActionTools::GroupDefinition *selectionMode = new ActionTools::GroupDefinition("selectionmode", tr("Selection mode"), this);
			selectionMode->setMasterList(mode);
			selectionMode->setMasterValues(QStringList() << ReadTextFileInstance::modes.first.at(ReadTextFileInstance::Selection));

			ActionTools::NumberParameterDefinition *firstline = new ActionTools::NumberParameterDefinition("firstline", tr("First line"), this);
			firstline->setTooltip(tr("The line where to start reading the file"));
			firstline->setMinimum(1);
			firstline->setDefaultValue(1);
			selectionMode->addMember(firstline, 1);

			ActionTools::NumberParameterDefinition *lastline = new ActionTools::NumberParameterDefinition("lastline", tr("Last line"), this);
			lastline->setTooltip(tr("The line where to stop reading the file"));
			lastline->setMinimum(1);
			lastline->setDefaultValue(1);
			selectionMode->addMember(lastline, 1);

			addElement(selectionMode, 1);

			addException(ReadTextFileInstance::CannotOpenFileException, tr("Cannot read file"));
		}

		QString name() const													{ return QObject::tr("Read text file"); }
		QString id() const														{ return "ActionReadTextFile"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Read a plain text file"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new ReadTextFileInstance(this); }
		ActionTools::ActionCategory category() const									{ return ActionTools::Data; }
		QPixmap icon() const													{ return QPixmap(":/icons/readtext.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(ReadTextFileDefinition)
	};
}

#endif // READTEXTFILEDEFINITION_H
