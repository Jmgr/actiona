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

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools/actiondefinition.hpp"
#include "killprocessinstance.hpp"
#include "actiontools/textparameterdefinition.hpp"
#include "actiontools/numberparameterdefinition.hpp"
#include "actiontools/listparameterdefinition.hpp"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class KillProcessDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit KillProcessDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("KillProcessInstance::killModes", KillProcessInstance::killModes);

            auto &processId = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("processId"), tr("Process id")});
            processId.setTooltip(tr("The process id of the process to kill"));

            auto &killMode = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("killMode"), tr("Kill mode")}, 1);
            killMode.setTooltip(tr("The kill mode"));
            killMode.setItems(KillProcessInstance::killModes);
            killMode.setDefaultValue(KillProcessInstance::killModes.second.at(KillProcessInstance::GracefulThenForceful));

            auto &timeout = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("timeout"), tr("Timeout")}, 1);
            timeout.setTooltip(tr("The timeout before doing a forceful kill"));
            timeout.setMinimum(0);
            timeout.setMaximum(std::numeric_limits<int>::max());
            timeout.setDefaultValue(QStringLiteral("1000"));
		}

		QString name() const override													{ return QObject::tr("Kill process"); }
		QString id() const override														{ return QStringLiteral("ActionKillProcess"); }
		QString description() const override												{ return QObject::tr("Kills a process"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new KillProcessInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::System; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/icons/closeprocess.png")); }
		QStringList tabs() const override												{ return ActionDefinition::StandardTabs; }

	private:
		Q_DISABLE_COPY(KillProcessDefinition)
	};
}

