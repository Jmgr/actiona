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
#include "codeinstance.hpp"
#include "actiontools/textparameterdefinition.hpp"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class CodeDefinition : public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit CodeDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
            auto &code = addParameter<ActionTools::TextParameterDefinition>({QStringLiteral("code"), tr("Code")});
            code.setTooltip(tr("The code to evaluate"));
            code.setTextCodeMode(ActionTools::TextParameterDefinition::CodeOnly);
		}

		QString name() const override													{ return QObject::tr("Code"); }
		QString id() const override														{ return QStringLiteral("ActionCode"); }
		QString description() const override												{ return QObject::tr("Evaluates some code"); }
		ActionTools::ActionInstance *newActionInstance() const override					{ return new CodeInstance(this); }
		ActionTools::ActionCategory category() const override							{ return ActionTools::Internal; }
		QPixmap icon() const override													{ return QPixmap(QStringLiteral(":/actions/icons/code.png")); }

	private:
		Q_DISABLE_COPY(CodeDefinition)
	};
}

