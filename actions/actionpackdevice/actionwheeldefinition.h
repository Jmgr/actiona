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

#ifndef ACTIONWHEELDEFINITION_H
#define ACTIONWHEELDEFINITION_H

#include "actiondefinition.h"
#include "actionwheelinstance.h"
#include "numberparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionWheelDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionWheelDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::NumberParameterDefinition *intensity = new ActionTools::NumberParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"intensity",
																								tr("Intensity"),
																								this);
		intensity->setTooltip(tr("Intensity of the movement, positive is up, negative is down"));
		intensity->setMinimum(INT_MIN);
		intensity->setMaximum(INT_MAX);
		addElement(intensity);
	}

	QString name() const													{ return QObject::tr("Wheel"); }
	QString id() const														{ return "ActionWheel"; }
	Flag flags() const														{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const												{ return QObject::tr("Emulates the mouse wheel"); }
	Tools::Version version() const											{ return Tools::Version(0, 0, 1); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionWheelInstance(this); }
	Status status() const													{ return Alpha; }
	Category category() const												{ return Device; }
	QPixmap icon() const													{ return QPixmap(":/icons/key.png"); }

private:
	Q_DISABLE_COPY(ActionWheelDefinition)
};

#endif // ACTIONWHEELDEFINITION_H
