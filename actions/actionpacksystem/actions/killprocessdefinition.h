/*
	Actiona
	Copyright (C) 2005-2016 Jonathan Mercier-Ganady

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

#ifndef KILLPROCESSDEFINITION_H
#define KILLPROCESSDEFINITION_H

#include "actiondefinition.h"
#include "killprocessinstance.h"
#include "textparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "listparameterdefinition.h"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class KillProcessDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit KillProcessDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("KillProcessInstance::killModes", KillProcessInstance::killModes);

			ActionTools::TextParameterDefinition *processId = new ActionTools::TextParameterDefinition(ActionTools::Name("processId", tr("Process id")), this);
			processId->setTooltip(tr("The process id of the process to kill"));
			addElement(processId);

			ActionTools::ListParameterDefinition *killMode = new ActionTools::ListParameterDefinition(ActionTools::Name("killMode", tr("Kill mode")), this);
			killMode->setTooltip(tr("The kill mode"));
			killMode->setItems(KillProcessInstance::killModes);
			killMode->setDefaultValue(KillProcessInstance::killModes.second.at(KillProcessInstance::GracefulThenForceful));
			addElement(killMode, 1);

			ActionTools::NumberParameterDefinition *timeout = new ActionTools::NumberParameterDefinition(ActionTools::Name("timeout", tr("Timeout")), this);
			timeout->setTooltip(tr("The timeout before doing a forceful kill"));
			timeout->setMinimum(0);
			timeout->setMaximum(std::numeric_limits<int>::max());
			timeout->setDefaultValue(1000);
			addElement(timeout, 1);
		}

		QString name() const													{ return QObject::tr("Kill process"); }
		QString id() const														{ return "ActionKillProcess"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Kills a process"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new KillProcessInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::System; }
		QPixmap icon() const													{ return QPixmap(":/icons/closeprocess.png"); }
		QStringList tabs() const												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(KillProcessDefinition)
	};
}

#endif // KILLPROCESSDEFINITION_H
