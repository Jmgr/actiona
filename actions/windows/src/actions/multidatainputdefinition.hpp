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
#include "multidatainputinstance.hpp"
#include "actiontools/multitextparameterdefinition.hpp"
#include "actiontools/listparameterdefinition.hpp"
#include "actiontools/imageparameterdefinition.hpp"
#include "actiontools/textparameterdefinition.hpp"
#include "actiontools/variableparameterdefinition.hpp"
#include "actiontools/numberparameterdefinition.hpp"
#include "actiontools/groupdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class MultiDataInputDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit MultiDataInputDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("MultiDataInputInstance::modes", MultiDataInputInstance::modes);

            auto &question = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("question"), tr("Question")});
            question.setTooltip(tr("The question to ask"));

            auto &mode = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("mode"), tr("Mode")});
            mode.setTooltip(tr("The input mode"));
            mode.setItems(MultiDataInputInstance::modes);
            mode.setDefaultValue(MultiDataInputInstance::modes.second.at(MultiDataInputInstance::ComboBoxMode));

            auto &items = addParameter<ActionTools::MultiTextParameterDefinition>({QStringLiteral("items"), tr("Items")});
            items.setTooltip(tr("The item list"));

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

            auto &choiceGroup = addGroup(1);
            choiceGroup.setMasterList(mode);
            choiceGroup.setMasterValues({MultiDataInputInstance::modes.first.at(MultiDataInputInstance::ListMode), MultiDataInputInstance::modes.first.at(MultiDataInputInstance::CheckboxMode)});

            auto &maximumChoiceCount = choiceGroup.addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("maximumChoiceCount"), tr("Maximum choice count")});
            maximumChoiceCount.setTooltip(tr("The maximum number of choices that can be made"));
            maximumChoiceCount.setMinimum(0);
            maximumChoiceCount.setDefaultValue(QStringLiteral("1"));
		}

		QString name() const override													{ return QObject::tr("Multi data input"); }
		QString id() const override														{ return QStringLiteral("ActionMultiDataInput"); }
		QString description() const override												{ return QObject::tr("Ask the user to choose from a list"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new MultiDataInputInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Windows; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/datainput.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(MultiDataInputDefinition)
	};
}

