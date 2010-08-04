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

#ifndef ACTIONTEXTDEFINITION_H
#define ACTIONTEXTDEFINITION_H

#include "actiondefinition.h"
#include "actiontextinstance.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionTextDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionTextDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::TextParameterDefinition *text = new ActionTools::TextParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"text",
																								tr("Text"),
																								this);
		text->setTooltip(tr("The text to write"));
		addElement(text);

		addException(ActionTextInstance::FailedToSendInputException, tr("Send input failure"));
	}

	QString name() const													{ return QObject::tr("Text"); }
	QString id() const														{ return "ActionText"; }
	Flag flags() const														{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const												{ return QObject::tr("Write some text"); }
	Tools::Version version() const											{ return Tools::Version(0, 0, 1); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionTextInstance(this); }
	Status status() const													{ return Alpha; }
	Category category() const												{ return Device; }
	QPixmap icon() const													{ return QPixmap(":/icons/key.png"); }

private:
	Q_DISABLE_COPY(ActionTextDefinition)
};

#endif // ACTIONTEXTDEFINITION_H
