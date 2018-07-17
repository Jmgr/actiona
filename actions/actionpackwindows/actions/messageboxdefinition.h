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
#include "messageboxinstance.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"
#include "groupdefinition.h"
#include "ifactionparameterdefinition.h"
#include "imageparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class MessageBoxDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit MessageBoxDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("MessageBoxInstance::icons", MessageBoxInstance::icons);
			translateItems("MessageBoxInstance::buttons", MessageBoxInstance::buttons);
			translateItems("MessageBoxInstance::textmodes", MessageBoxInstance::textmodes);

            auto &text = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("message"), tr("Message")});
            text.setTooltip(tr("The text to show"));

            auto &title = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("title"), tr("Title")});
            title.setTooltip(tr("The title to show"));

            auto &icon = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("icon"), tr("Icon")});
            icon.setTooltip(tr("The icon to use"));
            icon.setItems(MessageBoxInstance::icons);
            icon.setDefaultValue(MessageBoxInstance::icons.second.at(MessageBoxInstance::None));

            auto &type = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("type"), tr("Type")}, 1);
            type.setTooltip(tr("The message box type"));
            type.setItems(MessageBoxInstance::buttons);
            type.setDefaultValue(MessageBoxInstance::buttons.second.at(MessageBoxInstance::OkButton));

            auto &customIcon = addParameter<ActionTools::ImageParameterDefinition>({QStringLiteral("customIcon"), tr("Custom icon")}, 1);
            customIcon.setTooltip(tr("The custom icon to use"));
            customIcon.setMode(ActionTools::FileEdit::FileOpen);
            customIcon.setCaption(tr("Select the icon to use"));
            customIcon.setFilter(tr("Images (*.jpg *.jpeg *.png *.bmp *.gif *.pbm *.pgm *.ppm *.xbm *.xpm)"));

            auto &windowIcon = addParameter<ActionTools::ImageParameterDefinition>({QStringLiteral("windowIcon"), tr("Window icon")}, 1);
            windowIcon.setTooltip(tr("The window icon to use"));
            windowIcon.setMode(ActionTools::FileEdit::FileOpen);
            windowIcon.setCaption(tr("Select the icon to use"));
            windowIcon.setFilter(tr("Images (*.jpg *.jpeg *.png *.bmp *.gif *.pbm *.pgm *.ppm *.xbm *.xpm)"));

            auto &textMode = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("textMode"), tr("Text mode")}, 1);
            textMode.setTooltip(tr("The message box text mode"));
            textMode.setItems(MessageBoxInstance::textmodes);
            textMode.setDefaultValue(MessageBoxInstance::textmodes.second.at(MessageBoxInstance::AutoTextMode));

            auto &yesNoGroup = addGroup(1);
            yesNoGroup.setMasterList(type);
            yesNoGroup.setMasterValues({MessageBoxInstance::buttons.first.at(MessageBoxInstance::YesNoButtons)});

            auto &ifYes = yesNoGroup.addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifYes"), tr("If yes")});
            ifYes.setTooltip(tr("What to do if the yes button is pressed"));

            auto &ifNo = yesNoGroup.addParameter<ActionTools::IfActionParameterDefinition>({QStringLiteral("ifNo"), tr("If no")});
            ifNo.setTooltip(tr("What to do if the no button is pressed"));
		}

		QString name() const override													{ return QObject::tr("Message Box"); }
		QString id() const override														{ return QStringLiteral("ActionMessageBox"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Shows a message box"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new MessageBoxInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Windows; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/msg.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(MessageBoxDefinition)
	};
}

