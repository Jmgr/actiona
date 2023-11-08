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

#include "actiontools/actionpack.hpp"
#include "internal_global.hpp"
#include "actions/pausedefinition.hpp"
#include "actions/codedefinition.hpp"
#include "actions/gotodefinition.hpp"
#include "actions/loopdefinition.hpp"
#include "actions/noopdefinition.hpp"
#include "actions/stopdefinition.hpp"
#include "actions/variabledefinition.hpp"
#include "actions/timeconditiondefinition.hpp"
#include "actions/exitdefinition.hpp"
#include "actions/variableconditiondefinition.hpp"
#include "actions/consoledefinition.hpp"
#include "actions/beginproceduredefinition.hpp"
#include "actions/callproceduredefinition.hpp"
#include "actions/endproceduredefinition.hpp"
#include "actions/randomdefinition.hpp"

namespace ActionTools
{
	class ActionDefinition;
}

class ACTIONPACKINTERNAL_EXPORT ActionPackInternal : public QObject, public ActionTools::ActionPack
{
	Q_OBJECT

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
        addActionDefinition(new Actions::RandomDefinition(this));
	}

	QString id() const override								{ return QStringLiteral("internal"); }
	QString name() const override							{ return tr("Internal actions"); }

private:
	Q_DISABLE_COPY(ActionPackInternal)
};

