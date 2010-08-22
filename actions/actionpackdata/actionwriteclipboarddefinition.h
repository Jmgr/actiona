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

#ifndef ACTIONWRITECLIPBOARDDEFINITION_H
#define ACTIONWRITECLIPBOARDDEFINITION_H

#include "actiondefinition.h"
#include "actionwriteclipboardinstance.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionWriteClipboardDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionWriteClipboardDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::TextParameterDefinition *input = new ActionTools::TextParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"value",
																								 tr("Value"),
																								 this);
		input->setTooltip(tr("The new clipboard value"));
		addElement(input);
	}

	QString name() const													{ return QObject::tr("Write clipboard"); }
	QString id() const														{ return "ActionWriteClipboard"; }
	Flag flags() const														{ return ActionDefinition::flags() | Official; }
	QString description() const												{ return QObject::tr("Set the clipboard contents"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionWriteClipboardInstance(this); }
	Category category() const												{ return Data; }
	QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }

private:
	Q_DISABLE_COPY(ActionWriteClipboardDefinition)
};

#endif // ACTIONWRITECLIPBOARDDEFINITION_H
