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

#ifndef CODEDEFINITION_H
#define CODEDEFINITION_H

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
			ActionTools::TextParameterDefinition *code = new ActionTools::TextParameterDefinition("code", tr("Code"), this);
			code->setTooltip(tr("The code to evaluate"));
			code->setCodeOnly(true);
			addElement(code);
		}

		QString name() const													{ return QObject::tr("Code"); }
		QString id() const														{ return "ActionCode"; }
		Flag flags() const														{ return ActionDefinition::flags() | Official; }
		QString description() const												{ return QObject::tr("Evaluates some code"); }
		ActionTools::ActionInstance *newActionInstance() const					{ return new CodeInstance(this); }
		Category category() const												{ return Internal; }
		QPixmap icon() const													{ return QPixmap(":/actions/icons/code.png"); }

	private:
		Q_DISABLE_COPY(CodeDefinition)
	};
}

#endif // CODEDEFINITION_H
