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
#include "numberparameterdefinition.h"
#include "listparameterdefinition.h"
#include "pauseinstance.h"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class PauseDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit PauseDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("PauseInstance::units", PauseInstance::units);

            auto &duration = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("duration"), tr("Duration")});
            duration.setTooltip(tr("The duration of the pause"));
            duration.setMinimum(0);
            duration.setMaximum(std::numeric_limits<int>::max());
            duration.setDefaultValue(QStringLiteral("5"));

            auto &unit = addParameter<ActionTools::ListParameterDefinition>({QStringLiteral("unit"), tr("Unit")});
            unit.setTooltip(tr("The pause duration unit"));
            unit.setItems(PauseInstance::units);
            unit.setDefaultValue(PauseInstance::units.second.at(PauseInstance::Seconds));
		}

		QString name() const override													{ return QObject::tr("Pause"); }
		QString id() const override														{ return QStringLiteral("ActionPause"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Pauses the script execution"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new PauseInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Internal; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/pause.png")); }

	private:
		Q_DISABLE_COPY(PauseDefinition)
	};
}

