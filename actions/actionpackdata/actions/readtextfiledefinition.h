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

			auto file = new ActionTools::FileParameterDefinition(ActionTools::Name(QStringLiteral("file"), tr("File")), this);
			file->setTooltip(tr("The file you want to read"));
			addElement(file);

			auto variable = new ActionTools::VariableParameterDefinition(ActionTools::Name(QStringLiteral("variable"), tr("Variable")), this);
			variable->setTooltip(tr("The variable where to save the text read from the file"));
			addElement(variable);

			auto mode = new ActionTools::ListParameterDefinition(ActionTools::Name(QStringLiteral("mode"), tr("Mode")), this);
			mode->setTooltip(tr("The file read mode"));
			mode->setItems(ReadTextFileInstance::modes);
			mode->setDefaultValue(ReadTextFileInstance::modes.second.at(ReadTextFileInstance::Full));
			addElement(mode, 1);

			auto selectionMode = new ActionTools::GroupDefinition(this);
			selectionMode->setMasterList(mode);
			selectionMode->setMasterValues(QStringList() << ReadTextFileInstance::modes.first.at(ReadTextFileInstance::Selection));

			auto firstline = new ActionTools::NumberParameterDefinition(ActionTools::Name(QStringLiteral("firstline"), tr("First line")), this);
			firstline->setTooltip(tr("The line where to start reading the file"));
			firstline->setMinimum(1);
			firstline->setDefaultValue(QStringLiteral("1"));
			selectionMode->addMember(firstline, 1);

			auto lastline = new ActionTools::NumberParameterDefinition(ActionTools::Name(QStringLiteral("lastline"), tr("Last line")), this);
			lastline->setTooltip(tr("The line where to stop reading the file"));
			lastline->setMinimum(1);
			lastline->setDefaultValue(QStringLiteral("1"));
			selectionMode->addMember(lastline, 1);

			addElement(selectionMode, 1);

			addException(ReadTextFileInstance::CannotOpenFileException, tr("Cannot read file"));
		}

		QString name() const override													{ return QObject::tr("Read text file"); }
		QString id() const override														{ return QStringLiteral("ActionReadTextFile"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Read a plain text file"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new ReadTextFileInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Data; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/readtext.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(ReadTextFileDefinition)
	};
}

