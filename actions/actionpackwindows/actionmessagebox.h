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

#ifndef ACTIONMESSAGEBOX_H
#define ACTIONMESSAGEBOX_H

#include "actioninterface.h"
#include "actionmessageboxinstance.h"
#include "textparameterdefinition.h"
#include "listparameterdefinition.h"
#include "groupdefinition.h"
#include "ifactionparameterdefinition.h"

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionMessageBox : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionMessageBox(ActionTools::ActionPackInterface *pack)
	: ActionMessageBox::ActionInterface(pack)
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
		icon->setOption("default", ActionMessageBoxInstance::icons.second.at(ActionMessageBoxInstance::None));
		addElement(icon);

		ActionTools::ListParameterDefinition *type = new ActionTools::ListParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"type",
																								tr("Type"),
																								this);
		type->setTooltip(tr("The message box type"));
		type->setItems(ActionMessageBoxInstance::types);
		type->setOption("default", ActionMessageBoxInstance::types.second.at(ActionMessageBoxInstance::OkButton));
		addElement(type, 1);

		ActionTools::GroupDefinition *yesNoGroup = new ActionTools::GroupDefinition(			ActionTools::ElementDefinition::INPUT,
																								"ifyesno",
																								tr("Yes-No action"),
																								this);
		yesNoGroup->setMasterList(type);
		yesNoGroup->setMasterValues(QStringList() << ActionMessageBoxInstance::types.first.at(ActionMessageBoxInstance::YesNoButtons));

		ActionTools::IfActionParameterDefinition *ifYes = new ActionTools::IfActionParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"ifyes",
																								tr("If yes"),
																								this);
		ifYes->setTooltip(tr("What to do if the yes button is pressed"));
		ifYes->setOption("default", ActionMessageBoxInstance::types.second.at(0));
		yesNoGroup->addMember(ifYes);

		ActionTools::IfActionParameterDefinition *ifNo = new ActionTools::IfActionParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"ifno",
																								tr("If no"),
																								this);
		ifNo->setTooltip(tr("What to do if the no button is pressed"));
		ifNo->setOption("default", ActionMessageBoxInstance::types.second.at(0));
		yesNoGroup->addMember(ifNo);

		addElement(yesNoGroup, 1);
	}

	QString name() const											{ return QObject::tr("Message Box"); }
	QString id() const												{ return metaObject()->className(); }
	Flag flags() const												{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const										{ return QObject::tr("Shows a message box"); }
	Tools::Version version() const									{ return Tools::Version(0, 0, 1); }
	ActionTools::Action *newAction()								{ return new ActionMessageBoxInstance(this, 0); }
	Status status() const											{ return Alpha; }
	Category category() const										{ return Windows; }
	QString author() const											{ return "The Actionaz Team"; }
	QString website() const											{ return "www.actionaz.eu"; }
	QString email() const											{ return "jmgr@jmgr.info"; }
	QPixmap icon() const											{ return QPixmap(":/icons/msg.png"); }
	ActionTools::Action *scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionMessageBox) }
	QStringList tabs() const										{ return QStringList() << tr("Standard") << tr("Advanced"); }

private:
	Q_DISABLE_COPY(ActionMessageBox)
};

#endif // ACTIONMESSAGEBOX_H
