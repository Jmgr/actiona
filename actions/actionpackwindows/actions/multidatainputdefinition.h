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

#ifndef MULTIDATAINPUTDEFINITION_H
#define MULTIDATAINPUTDEFINITION_H

#include "actiondefinition.h"
#include "multidatainputinstance.h"
#include "multitextparameterdefinition.h"
#include "listparameterdefinition.h"
#include "fileparameterdefinition.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "groupdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class MultiDataInputDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit MultiDataInputDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("MultiDataInputInstance::modes", MultiDataInputInstance::modes);

			ActionTools::TextParameterDefinition *question = new ActionTools::TextParameterDefinition(ActionTools::Name("question", tr("Question")), this);
			question->setTooltip(tr("The question to ask"));
			addElement(question);

			ActionTools::ListParameterDefinition *mode = new ActionTools::ListParameterDefinition(ActionTools::Name("mode", tr("Mode")), this);
			mode->setTooltip(tr("The input mode"));
			mode->setItems(MultiDataInputInstance::modes);
			mode->setDefaultValue(MultiDataInputInstance::modes.second.at(MultiDataInputInstance::ComboBoxMode));
			addElement(mode);

			ActionTools::MultiTextParameterDefinition *items = new ActionTools::MultiTextParameterDefinition(ActionTools::Name("items", tr("Items")), this);
			items->setTooltip(tr("The item list"));
			addElement(items);

			ActionTools::TextParameterDefinition *defaultValue = new ActionTools::TextParameterDefinition(ActionTools::Name("defaultValue", tr("Default value")), this);
			defaultValue->setTooltip(tr("The default value"));
			addElement(defaultValue);

			ActionTools::VariableParameterDefinition *variable = new ActionTools::VariableParameterDefinition(ActionTools::Name("variable", tr("Variable")), this);
			variable->setTooltip(tr("The variable where to save the entered input"));
			addElement(variable);

			ActionTools::TextParameterDefinition *windowTitle = new ActionTools::TextParameterDefinition(ActionTools::Name("windowTitle", tr("Window title")), this);
			windowTitle->setTooltip(tr("The title of the window"));
			addElement(windowTitle, 1);

			ActionTools::FileParameterDefinition *windowIcon = new ActionTools::FileParameterDefinition(ActionTools::Name("windowIcon", tr("Window icon")), this);
			windowIcon->setTooltip(tr("The window icon to use"));
			windowIcon->setMode(ActionTools::FileEdit::FileOpen);
			windowIcon->setCaption(tr("Select the icon to use"));
			windowIcon->setFilter(tr("Images (*.jpg *.jpeg *.png *.bmp *.gif *.pbm *.pgm *.ppm *.xbm *.xpm)"));
			addElement(windowIcon, 1);

			ActionTools::GroupDefinition *choiceGroup = new ActionTools::GroupDefinition(this);
			choiceGroup->setMasterList(mode);
			choiceGroup->setMasterValues(QStringList()
										 << MultiDataInputInstance::modes.first.at(MultiDataInputInstance::ListMode)
										 << MultiDataInputInstance::modes.first.at(MultiDataInputInstance::CheckboxMode));

			ActionTools::NumberParameterDefinition *maximumChoiceCount = new ActionTools::NumberParameterDefinition(ActionTools::Name("maximumChoiceCount", tr("Maximum choice count")), this);
			maximumChoiceCount->setTooltip(tr("The maximum number of choices that can be made"));
			maximumChoiceCount->setMinimum(0);
			maximumChoiceCount->setDefaultValue(1);
			choiceGroup->addMember(maximumChoiceCount);

			addElement(choiceGroup, 1);
		}

		QString name() const													{ return QObject::tr("Multi data input"); }
		QString id() const														{ return "ActionMultiDataInput"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Ask the user to enter some data"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new MultiDataInputInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Windows; }
		QPixmap icon() const													{ return QPixmap(":/icons/datainput.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(MultiDataInputDefinition)
	};
}

#endif // MULTIDATAINPUTDEFINITION_H
