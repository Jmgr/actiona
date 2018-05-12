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
#include "codeinstance.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

namespace Actions
{
	class CodeDefinition : public QObject, public ActionTools::ActionDefinition
	{
	   Q_OBJECT

	public:
		explicit CodeDefinition(ActionTools::ActionPack *pack)
		: ActionDefinition(pack)
		{
			ActionTools::TextParameterDefinition *code = new ActionTools::TextParameterDefinition(ActionTools::Name(QStringLiteral("code"), tr("Code")), this);
			code->setTooltip(tr("The code to evaluate"));
			code->setTextCodeMode(ActionTools::TextParameterDefinition::CodeOnly);
			addElement(code);
		}

		QString name() const													{ return QObject::tr("Code"); }
		QString id() const														{ return QStringLiteral("ActionCode"); }
		ActionTools::Flag flags() const											{ return ActionDefinition::flags() | ActionTools::Official; }
		QString description() const												{ return QObject::tr("Evaluates some code"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new CodeInstance(this); }
		ActionTools::ActionCategory category() const							{ return ActionTools::Internal; }
		QPixmap icon() const													{ return QPixmap(QStringLiteral(":/actions/icons/code.png")); }

	private:
		Q_DISABLE_COPY(CodeDefinition)
	};
}

