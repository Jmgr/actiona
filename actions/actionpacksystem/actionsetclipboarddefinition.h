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

#ifndef ACTIONSETCLIPBOARDDEFINITION_H
#define ACTIONSETCLIPBOARDDEFINITION_H

#include "actiondefinition.h"
#include "actionsetclipboardinstance.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionSetClipboardDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionSetClipboardDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::TextParameterDefinition *input = new ActionTools::TextParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"value",
																								 tr("Value"),
																								 this);
		input->setTooltip(tr("The new clipboard value"));
		addElement(input);
	}

	QString name() const													{ return QObject::tr("Set clipboard"); }
	QString id() const														{ return "ActionSetClipboard"; }
	Flag flags() const														{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const												{ return QObject::tr("Set the clipboard contents"); }
	Tools::Version version() const											{ return Tools::Version(1, 0, 0); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionSetClipboardInstance(this); }
	Status status() const													{ return Stable; }
	Category category() const												{ return System; }
	QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }

private:
	Q_DISABLE_COPY(ActionSetClipboardDefinition)
};

#endif // ACTIONSETCLIPBOARDDEFINITION_H
