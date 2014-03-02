/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#ifndef PAUSEDEFINITION_H
#define PAUSEDEFINITION_H

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
	class PauseDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit PauseDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			translateItems("PauseInstance::units", PauseInstance::units);

			ActionTools::NumberParameterDefinition *duration = new ActionTools::NumberParameterDefinition(ActionTools::Name("duration", tr("Duration")), this);
			duration->setTooltip(tr("The duration of the pause"));
			duration->setMinimum(0);
			duration->setMaximum(std::numeric_limits<int>::max());
			duration->setDefaultValue(5);
			addElement(duration);

			ActionTools::ListParameterDefinition *unit = new ActionTools::ListParameterDefinition(ActionTools::Name("unit", tr("Unit")), this);
			unit->setTooltip(tr("The pause duration unit"));
			unit->setItems(PauseInstance::units);
			unit->setDefaultValue(PauseInstance::units.second.at(PauseInstance::Seconds));
			addElement(unit);
		}

		QString name() const													{ return QObject::tr("Pause"); }
		QString id() const														{ return "ActionPause"; }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Pauses the script execution"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new PauseInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/pause.png"); }

	private:
		Q_DISABLE_COPY(PauseDefinition)
	};
}

#endif // PAUSEDEFINITION_H
