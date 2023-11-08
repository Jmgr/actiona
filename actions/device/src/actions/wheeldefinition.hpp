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
#include "wheelinstance.hpp"
#include "actiontools/numberparameterdefinition.hpp"

#include <limits>

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class WheelDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT
	
	public:
		explicit WheelDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			auto &intensity = addParameter<ActionTools::NumberParameterDefinition>({QStringLiteral("intensity"), tr("Intensity")});
            intensity.setTooltip(tr("Intensity of the movement, positive is up, negative is down"));
            intensity.setMinimum(std::numeric_limits<int>::min());
            intensity.setMaximum(std::numeric_limits<int>::max());

			addException(WheelInstance::FailedToSendInputException, tr("Send input failure"));
		}
	
		QString name() const override													{ return QObject::tr("Wheel"); }
		QString id() const override														{ return QStringLiteral("ActionWheel"); }
		QString description() const override												{ return QObject::tr("Emulates the mouse wheel"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new WheelInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Device; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/wheel.png")); }
		bool requirementCheck(QStringList &missingRequirements) const override			{ return requirementCheckXTest(missingRequirements); }
	
	private:
		Q_DISABLE_COPY(WheelDefinition)
	};
}

