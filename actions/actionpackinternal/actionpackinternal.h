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
#include "actions/consoledefinition.h"
#include "actions/beginproceduredefinition.h"
#include "actions/callproceduredefinition.h"
#include "actions/endproceduredefinition.h"

#include <QtCore/qplugin.h>

namespace ActionTools
{
	class ActionDefinition;
}

class ActionPackInternal : public QObject, public ActionTools::ActionPack
{
	Q_OBJECT
	Q_INTERFACES(ActionTools::ActionPack)
    Q_PLUGIN_METADATA(IID "tools.actiona.ActionPack" FILE "internal.json")

public:
	ActionPackInternal()							= default;

	void createDefinitions() override
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
		addActionDefinition(new Actions::ConsoleDefinition(this));
		addActionDefinition(new Actions::BeginProcedureDefinition(this));
		addActionDefinition(new Actions::CallProcedureDefinition(this));
		addActionDefinition(new Actions::EndProcedureDefinition(this));
	}

	QString id() const override								{ return QStringLiteral("internal"); }
	QString name() const override							{ return tr("Internal actions"); }
	Tools::Version version() const override					{ return Tools::Version(0, 0, 1); }

private:
	Q_DISABLE_COPY(ActionPackInternal)
};

