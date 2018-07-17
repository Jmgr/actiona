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
#include "loopinstance.h"
#include "lineparameterdefinition.h"
#include "numberparameterdefinition.h"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class LoopDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit LoopDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            auto &line = addParameter<ActionTools::LineParameterDefinition>({QStringLiteral("line"), tr("Line")});
            line.setTooltip(tr("The line (or label) to go to"));

            auto &count = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("count"), tr("Count")});
            count.setTooltip(tr("The number of times (evaluated the first time)"));
            count.setMinimum(0);
            count.setMaximum(std::numeric_limits<int>::max());
		}

		QString name() const override													{ return QObject::tr("Loop"); }
		QString id() const override														{ return QStringLiteral("ActionLoop"); }
		ActionTools::Flag flags() const override											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const override												{ return QObject::tr("Go to a script line a specific number of times"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new LoopInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Internal; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/loop.png")); }

	private:
		Q_DISABLE_COPY(LoopDefinition)
	};
}

