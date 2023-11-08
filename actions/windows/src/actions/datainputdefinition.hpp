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

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools/actiondefinition.hpp"
#include "datainputinstance.hpp"
#include "actiontools/textparameterdefinition.hpp"
#include "actiontools/variableparameterdefinition.hpp"
#include "actiontools/listparameterdefinition.hpp"
#include "actiontools/imageparameterdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class DataInputDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit DataInputDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("DataInputInstance::dataTypes", DataInputInstance::dataTypes);
            translateItems("DataInputInstance::editorTypes", DataInputInstance::editorTypes);

            auto &question = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("question"), tr("Question")});
            question.setTooltip(tr("The question to ask"));

            auto &dataType = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("dataType"), tr("Data type")});
            dataType.setTooltip(tr("The data type"));
            dataType.setItems(DataInputInstance::dataTypes);
            dataType.setDefaultValue(DataInputInstance::dataTypes.second.at(DataInputInstance::TextType));

            auto &editorType = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("editorType"), tr("Editor type")});
            editorType.setTooltip(tr("The editor type"));
            editorType.setItems(DataInputInstance::editorTypes);
            editorType.setDefaultValue(DataInputInstance::editorTypes.second.at(DataInputInstance::LineEditorType));

            auto &defaultValue = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("defaultValue"), tr("Default value")});
            defaultValue.setTooltip(tr("The default value"));

            auto &variable = addParameter<ActionTools::VariableParameterDefinition>({QStringLiteral("variable"), tr("Variable")});
            variable.setTooltip(tr("The variable where to save the entered input"));

            auto &windowTitle = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("windowTitle"), tr("Window title")}, 1);
            windowTitle.setTooltip(tr("The title of the window"));

            auto &windowIcon = addParameter<ActionTools::ImageParameterDefinition>({QStringLiteral("windowIcon"), tr("Window icon")}, 1);
            windowIcon.setTooltip(tr("The window icon to use"));
            windowIcon.setMode(ActionTools::FileEdit::FileOpen);
            windowIcon.setCaption(tr("Select the icon to use"));
            windowIcon.setFilter(tr("Images (*.jpg *.jpeg *.png *.bmp *.gif *.pbm *.pgm *.ppm *.xbm *.xpm)"));
		}

		QString name() const override													{ return QObject::tr("Data input"); }
		QString id() const override														{ return QStringLiteral("ActionDataInput"); }
		QString description() const override												{ return QObject::tr("Ask the user to enter some data"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new DataInputInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Windows; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/datainput.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(DataInputDefinition)
	};
}

