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

#ifndef ACTIONPACKINTERNAL_H
#define ACTIONPACKINTERNAL_H

#include "actionpack.h"
#include "actionpausedefinition.h"
#include "actioncodedefinition.h"
#include "actiongotodefinition.h"
#include "actionloopdefinition.h"
#include "actionnoopdefinition.h"
#include "actionstopdefinition.h"
#include "actionvariabledefinition.h"
#include "actiontimeconditiondefinition.h"

#include <QtCore/qplugin.h>

namespace ActionTools
{
	class ActionDefinition;
}

class ActionPackInternal : public QObject, public ActionTools::ActionPack
{
	Q_OBJECT
	Q_INTERFACES(ActionTools::ActionPack)

public:
	ActionPackInternal()
	{
		addActionDefinition(new ActionPauseDefinition(this));
		addActionDefinition(new ActionCodeDefinition(this));
		addActionDefinition(new ActionGotoDefinition(this));
		addActionDefinition(new ActionLoopDefinition(this));
		addActionDefinition(new ActionNoopDefinition(this));
		addActionDefinition(new ActionStopDefinition(this));
		addActionDefinition(new ActionVariableDefinition(this));
		addActionDefinition(new ActionTimeConditionDefinition(this));
	}

	QString id() const								{ return tr("internal"); }
	QString name() const							{ return tr("Internal actions"); }
	Tools::Version version() const					{ return Tools::Version(0, 0, 1); }

private:
	Q_DISABLE_COPY(ActionPackInternal)
};

Q_EXPORT_PLUGIN2(ActionPackInternal, ActionPackInternal)

#endif // ACTIONPACKINTERNAL_H
