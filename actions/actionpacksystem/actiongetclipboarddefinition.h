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

#ifndef ACTIONGETCLIPBOARDDEFINITION_H
#define ACTIONGETCLIPBOARDDEFINITION_H

#include "actiondefinition.h"
#include "actiongetclipboardinstance.h"
#include "textparameterdefinition.h"
#include "variableparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionGetClipboardDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionGetClipboardDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::VariableParameterDefinition *output = new ActionTools::VariableParameterDefinition( ActionTools::ElementDefinition::OUTPUT,
																								"variable",
																								 tr("Variable"),
																								 this);
		output->setTooltip(tr("The variable where to save the current clipboard value"));
		addElement(output);
	}

	QString name() const													{ return QObject::tr("Get clipboard"); }
	QString id() const														{ return "ActionGetClipboard"; }
	Flag flags() const														{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const												{ return QObject::tr("Get the clipboard contents"); }
	Tools::Version version() const											{ return Tools::Version(1, 0, 0); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionGetClipboardInstance(this); }
	Status status() const													{ return Stable; }
	Category category() const												{ return System; }
	QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }

private:
	Q_DISABLE_COPY(ActionGetClipboardDefinition)
};

#endif // ACTIONGETCLIPBOARDDEFINITION_H
