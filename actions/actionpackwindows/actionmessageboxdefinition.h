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

#ifndef ACTIONMESSAGEBOXDEFINITION_H
#define ACTIONMESSAGEBOXDEFINITION_H

#include "actiondefinition.h"
#include "actionmessageboxinstance.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"
#include "groupdefinition.h"
#include "ifactionparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionMessageBoxDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionMessageBoxDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::TextParameterDefinition *text = new ActionTools::TextParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"message",
																								 tr("Message"),
																								 this);
		text->setTooltip(tr("The text to show"));
		addElement(text);

		ActionTools::TextParameterDefinition *title = new ActionTools::TextParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"title",
																								tr("Title"),
																								this);
		title->setTooltip(tr("The title to show"));
		addElement(title);

		ActionTools::ListParameterDefinition *icon = new ActionTools::ListParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"icon",
																								tr("Icon"),
																								this);
		icon->setTooltip(tr("The icon to use"));
		icon->setItems(ActionMessageBoxInstance::icons);
		icon->setDefaultValue(ActionMessageBoxInstance::icons.second.at(ActionMessageBoxInstance::None));
		addElement(icon);

		ActionTools::ListParameterDefinition *type = new ActionTools::ListParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"type",
																								tr("Type"),
																								this);
		type->setTooltip(tr("The message box type"));
		type->setItems(ActionMessageBoxInstance::buttons);
		type->setDefaultValue(ActionMessageBoxInstance::buttons.second.at(ActionMessageBoxInstance::OkButton));
		addElement(type, 1);

		ActionTools::GroupDefinition *yesNoGroup = new ActionTools::GroupDefinition(			ActionTools::ElementDefinition::INPUT,
																								"ifyesno",
																								tr("Yes-No action"),
																								this);
		yesNoGroup->setMasterList(type);
		yesNoGroup->setMasterValues(QStringList() << ActionMessageBoxInstance::buttons.first.at(ActionMessageBoxInstance::YesNoButtons));

		ActionTools::IfActionParameterDefinition *ifYes = new ActionTools::IfActionParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"ifyes",
																								tr("If yes"),
																								this);
		ifYes->setTooltip(tr("What to do if the yes button is pressed"));
		yesNoGroup->addMember(ifYes);

		ActionTools::IfActionParameterDefinition *ifNo = new ActionTools::IfActionParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"ifno",
																								tr("If no"),
																								this);
		ifNo->setTooltip(tr("What to do if the no button is pressed"));
		yesNoGroup->addMember(ifNo);

		addElement(yesNoGroup, 1);
	}

	QString name() const													{ return QObject::tr("Message Box"); }
	QString id() const														{ return "ActionMessageBox"; }
	Flag flags() const														{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const												{ return QObject::tr("Shows a message box"); }
	Tools::Version version() const											{ return Tools::Version(0, 0, 1); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionMessageBoxInstance(this); }
	Status status() const													{ return Alpha; }
	Category category() const												{ return Windows; }
	QPixmap icon() const													{ return QPixmap(":/icons/msg.png"); }
	QStringList tabs() const												{ return QStringList() << tr("Standard") << tr("Advanced"); }

private:
	Q_DISABLE_COPY(ActionMessageBoxDefinition)
};

#endif // ACTIONMESSAGEBOXDEFINITION_H
