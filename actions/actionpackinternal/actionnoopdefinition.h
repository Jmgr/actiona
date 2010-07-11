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

#ifndef ACTIONNOOPDEFINITION_H
#define ACTIONNOOPDEFINITION_H

#include "actiondefinition.h"
#include "actionnoopinstance.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionNoopDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionNoopDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
	}

	QString name() const													{ return QObject::tr("No-op"); }
	QString id() const														{ return "ActionNoop"; }
	Flag flags() const														{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const												{ return QObject::tr("Does nothing"); }
	Tools::Version version() const											{ return Tools::Version(1, 0, 0); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionNoopInstance(this); }
	Status status() const													{ return Stable; }
	Category category() const												{ return Internal; }
	QPixmap icon() const													{ return QPixmap(":/icons/code.png"); }

private:
	Q_DISABLE_COPY(ActionNoopDefinition)
};

#endif // ACTIONNOOPDEFINITION_H
