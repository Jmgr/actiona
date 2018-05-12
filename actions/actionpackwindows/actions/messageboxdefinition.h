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
	class MessageBoxDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit MessageBoxDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("MessageBoxInstance::icons", MessageBoxInstance::icons);
			translateItems("MessageBoxInstance::buttons", MessageBoxInstance::buttons);
			translateItems("MessageBoxInstance::textmodes", MessageBoxInstance::textmodes);

			ActionTools::TextParameterDefinition *text = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("message"), tr("Message")), this);
			text->setTooltip(tr("The text to show"));
			addElement(text);

			ActionTools::TextParameterDefinition *title = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("title"), tr("Title")), this);
			title->setTooltip(tr("The title to show"));
			addElement(title);

			ActionTools::ListParameterDefinition *icon = new ActionTools::ListParameterDefinition(ActionTools::Name(QStringLiteral("icon"), tr("Icon")), this);
			icon->setTooltip(tr("The icon to use"));
			icon->setItems(MessageBoxInstance::icons);
			icon->setDefaultValue(MessageBoxInstance::icons.second.at(MessageBoxInstance::None));
			addElement(icon);

			ActionTools::ListParameterDefinition *type = new ActionTools::ListParameterDefinition(ActionTools::Name(QStringLiteral("type"), tr("Type")), this);
			type->setTooltip(tr("The message box type"));
			type->setItems(MessageBoxInstance::buttons);
			type->setDefaultValue(MessageBoxInstance::buttons.second.at(MessageBoxInstance::OkButton));
			addElement(type, 1);

			ActionTools::ImageParameterDefinition *customIcon = new ActionTools::ImageParameterDefinition(ActionTools::Name(QStringLiteral("customIcon"), tr("Custom icon")), this);
			customIcon->setTooltip(tr("The custom icon to use"));
			customIcon->setMode(ActionTools::FileEdit::FileOpen);
			customIcon->setCaption(tr("Select the icon to use"));
			customIcon->setFilter(tr("Images (*.jpg *.jpeg *.png *.bmp *.gif *.pbm *.pgm *.ppm *.xbm *.xpm)"));
			addElement(customIcon, 1);

			ActionTools::ImageParameterDefinition *windowIcon = new ActionTools::ImageParameterDefinition(ActionTools::Name(QStringLiteral("windowIcon"), tr("Window icon")), this);
			windowIcon->setTooltip(tr("The window icon to use"));
			windowIcon->setMode(ActionTools::FileEdit::FileOpen);
			windowIcon->setCaption(tr("Select the icon to use"));
			windowIcon->setFilter(tr("Images (*.jpg *.jpeg *.png *.bmp *.gif *.pbm *.pgm *.ppm *.xbm *.xpm)"));
			addElement(windowIcon, 1);

			ActionTools::ListParameterDefinition *textMode = new ActionTools::ListParameterDefinition(ActionTools::Name(QStringLiteral("textMode"), tr("Text mode")), this);
			textMode->setTooltip(tr("The message box text mode"));
			textMode->setItems(MessageBoxInstance::textmodes);
			textMode->setDefaultValue(MessageBoxInstance::textmodes.second.at(MessageBoxInstance::AutoTextMode));
			addElement(textMode, 1);

			ActionTools::GroupDefinition *yesNoGroup = new ActionTools::GroupDefinition(this);
			yesNoGroup->setMasterList(type);
			yesNoGroup->setMasterValues(QStringList() << MessageBoxInstance::buttons.first.at(MessageBoxInstance::YesNoButtons));

			ActionTools::IfActionParameterDefinition *ifYes = new ActionTools::IfActionParameterDefinition(ActionTools::Name(QStringLiteral("ifYes"), tr("If yes")), this);
			ifYes->setTooltip(tr("What to do if the yes button is pressed"));
			yesNoGroup->addMember(ifYes);

			ActionTools::IfActionParameterDefinition *ifNo = new ActionTools::IfActionParameterDefinition(ActionTools::Name(QStringLiteral("ifNo"), tr("If no")), this);
			ifNo->setTooltip(tr("What to do if the no button is pressed"));
			yesNoGroup->addMember(ifNo);

			addElement(yesNoGroup, 1);
		}

		QString name() const													{ return QObject::tr("Message Box"); }
		QString id() const														{ return QStringLiteral("ActionMessageBox"); }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Shows a message box"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new MessageBoxInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Windows; }
		QPixmap icon() const													{ return QPixmap(QStringLiteral(":/icons/msg.png")); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(MessageBoxDefinition)
	};
}

