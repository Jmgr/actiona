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

#ifndef WRITETEXTFILEDEFINITION_H
#define WRITETEXTFILEDEFINITION_H

#include "actiondefinition.h"
#include "writetextfileinstance.h"
#include "fileparameterdefinition.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WriteTextFileDefinition : public QObject, public ActionTools::ActionDefinition
	{
		Q_OBJECT

	public:
		explicit WriteTextFileDefinition(ActionTools::ActionPack *pack)
			: ActionDefinition(pack)
		{
			translateItems("WriteTextFileInstance::modes", WriteTextFileInstance::modes);

			ActionTools::FileParameterDefinition *file = new ActionTools::FileParameterDefinition(ActionTools::Name("file", tr("File")), this);
			file->setTooltip(tr("The file you want to write"));
			addElement(file);

			ActionTools::TextParameterDefinition *text = new ActionTools::TextParameterDefinition(ActionTools::Name("text", tr("Text")), this);
			text->setTooltip(tr("The text to write to the file"));
			addElement(text);

			ActionTools::ListParameterDefinition *mode = new ActionTools::ListParameterDefinition(ActionTools::Name("mode", tr("Mode")), this);
			mode->setTooltip(tr("The file write mode"));
			mode->setItems(WriteTextFileInstance::modes);
			mode->setDefaultValue(WriteTextFileInstance::modes.second.at(WriteTextFileInstance::Truncate));
			addElement(mode, 1);

			addException(WriteTextFileInstance::CannotWriteFileException, tr("Cannot write file"));
		}

		QString name() const													{ return QObject::tr("Write text file"); }
		QString id() const														{ return "ActionWriteTextFile"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Write a plain text file"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new WriteTextFileInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Data; }
		QPixmap icon() const													{ return QPixmap(":/icons/writetext.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(WriteTextFileDefinition)
	};
}

#endif // WRITETEXTFILEDEFINITION_H
