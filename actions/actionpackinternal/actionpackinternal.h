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
#include "actions/pausedefinition.h"
#include "actions/codedefinition.h"
#include "actions/gotodefinition.h"
#include "actions/loopdefinition.h"
#include "actions/noopdefinition.h"
#include "actions/stopdefinition.h"
#include "actions/variabledefinition.h"
#include "actions/timeconditiondefinition.h"
#include "actions/exitdefinition.h"
#include "actions/variableconditiondefinition.h"

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
		addActionDefinition(new Actions::PauseDefinition(this));
		addActionDefinition(new Actions::CodeDefinition(this));
		addActionDefinition(new Actions::GotoDefinition(this));
		addActionDefinition(new Actions::LoopDefinition(this));
		addActionDefinition(new Actions::NoopDefinition(this));
		addActionDefinition(new Actions::StopDefinition(this));
		addActionDefinition(new Actions::VariableDefinition(this));
		addActionDefinition(new Actions::TimeConditionDefinition(this));
		addActionDefinition(new Actions::ExitDefinition(this));
		addActionDefinition(new Actions::VariableConditionDefinition(this));
	}

	QString id() const								{ return tr("internal"); }
	QString name() const							{ return tr("Internal actions"); }
	Tools::Version version() const					{ return Tools::Version(0, 0, 1); }

private:
	Q_DISABLE_COPY(ActionPackInternal)
};

Q_EXPORT_PLUGIN2(ActionPackInternal, ActionPackInternal)

#endif // ACTIONPACKINTERNAL_H
