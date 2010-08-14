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

#ifndef ACTIONKILLPROCESSDEFINITION_H
#define ACTIONKILLPROCESSDEFINITION_H

#include "actiondefinition.h"
#include "actionkillprocessinstance.h"
#include "textparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "listparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionKillProcessDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionKillProcessDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::TextParameterDefinition *processId = new ActionTools::TextParameterDefinition( ActionTools::ElementDefinition::INPUT,
																								"processId",
																								 tr("Process id"),
																								 this);
		processId->setTooltip(tr("The process id of the process to kill"));
		addElement(processId);
		
		ActionTools::ListParameterDefinition *killMode = new ActionTools::ListParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"killMode",
																								tr("Kill mode"),
																								this);
		killMode->setTooltip(tr("The kill mode"));
		killMode->setItems(ActionKillProcessInstance::killModes);
		killMode->setDefaultValue(ActionKillProcessInstance::killModes.second.at(ActionKillProcessInstance::GracefulThenForceful));
		addElement(killMode, 1);
		
		ActionTools::NumberParameterDefinition *timeout = new ActionTools::NumberParameterDefinition( ActionTools::ElementDefinition::INPUT,
																								"timeout",
																								 tr("Timeout"),
																								 this);
		timeout->setTooltip(tr("The timeout before doing a forceful kill"));
		timeout->setMinimum(0);
		timeout->setMaximum(INT_MAX);
		timeout->setDefaultValue(1000);
		addElement(timeout, 1);
	}

	QString name() const													{ return QObject::tr("Kill process"); }
	QString id() const														{ return "ActionKillProcess"; }
	Flag flags() const														{ return ActionDefinition::flags() | Official; }
	QString description() const												{ return QObject::tr("Kills a process"); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionKillProcessInstance(this); }
	Category category() const												{ return System; }
	QPixmap icon() const													{ return QPixmap(":/icons/clipboard.png"); }
	QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

private:
	Q_DISABLE_COPY(ActionKillProcessDefinition)
};

#endif // ACTIONKILLPROCESSDEFINITION_H
