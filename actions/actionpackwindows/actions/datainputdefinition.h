/*
	Actiona
	Copyright (C) 2005-2017 Jonathan Mercier-Ganady

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
#include "datainputinstance.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"
#include "listparameterdefinition.h"
#include "imageparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class DataInputDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit DataInputDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("DataInputInstance::dataTypes", DataInputInstance::dataTypes);
            translateItems("DataInputInstance::editorTypes", DataInputInstance::editorTypes);

			ActionTools::TextParameterDefinition *question = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("question"), tr("Question")), this);
			question->setTooltip(tr("The question to ask"));
			addElement(question);

			ActionTools::ListParameterDefinition *dataType = new ActionTools::ListParameterDefinition(ActionTools::Name(QStringLiteral("dataType"), tr("Data type")), this);
			dataType->setTooltip(tr("The data type"));
			dataType->setItems(DataInputInstance::dataTypes);
			dataType->setDefaultValue(DataInputInstance::dataTypes.second.at(DataInputInstance::TextType));
			addElement(dataType);

			ActionTools::ListParameterDefinition *editorType = new ActionTools::ListParameterDefinition(ActionTools::Name(QStringLiteral("editorType"), tr("Editor type")), this);
            editorType->setTooltip(tr("The editor type"));
            editorType->setItems(DataInputInstance::editorTypes);
            editorType->setDefaultValue(DataInputInstance::editorTypes.second.at(DataInputInstance::LineEditorType));
            addElement(editorType);

			ActionTools::TextParameterDefinition *defaultValue = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("defaultValue"), tr("Default value")), this);
			defaultValue->setTooltip(tr("The default value"));
			addElement(defaultValue);

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition(ActionTools::Name(QStringLiteral("variable"), tr("Variable")), this);
			variable->setTooltip(tr("The variable where to save the entered input"));
			addElement(variable);

			ActionTools::TextParameterDefinition *windowTitle = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("windowTitle"), tr("Window title")), this);
			windowTitle->setTooltip(tr("The title of the window"));
			addElement(windowTitle, 1);

			ActionTools::ImageParameterDefinition *windowIcon = new ActionTools::ImageParameterDefinition(ActionTools::Name(QStringLiteral("windowIcon"), tr("Window icon")), this);
			windowIcon->setTooltip(tr("The window icon to use"));
			windowIcon->setMode(ActionTools::FileEdit::FileOpen);
			windowIcon->setCaption(tr("Select the icon to use"));
			windowIcon->setFilter(tr("Images (*.jpg *.jpeg *.png *.bmp *.gif *.pbm *.pgm *.ppm *.xbm *.xpm)"));
			addElement(windowIcon, 1);
		}

		QString name() const													{ return QObject::tr("Data input"); }
		QString id() const														{ return QStringLiteral("ActionDataInput"); }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Ask the user to enter some data"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new DataInputInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Windows; }
		QPixmap icon() const													{ return QPixmap(QStringLiteral(":/icons/datainput.png")); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(DataInputDefinition)
	};
}

