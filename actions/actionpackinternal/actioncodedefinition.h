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

#ifndef ACTIONCODEDEFINITION_H
#define ACTIONCODEDEFINITION_H

#include "actiondefinition.h"
#include "actioncodeinstance.h"
#include "textparameterdefinition.h"

namespace ActionTools
{
	class ActionPack;
	class ActionInstance;
}

class ActionCodeDefinition : public QObject, public ActionTools::ActionDefinition
{
   Q_OBJECT

public:
	explicit ActionCodeDefinition(ActionTools::ActionPack *pack)
	: ActionDefinition(pack)
	{
		ActionTools::TextParameterDefinition *code = new ActionTools::TextParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"code",
																								 tr("Code"),
																								 this);
		code->setTooltip(tr("The code to evaluate"));
		code->setCodeOnly(true);
		addElement(code);
	}

	QString name() const													{ return QObject::tr("Code"); }
	QString id() const														{ return "ActionCode"; }
	Flag flags() const														{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const												{ return QObject::tr("Evaluates some code"); }
	Tools::Version version() const											{ return Tools::Version(0, 0, 1); }
	ActionTools::ActionInstance *newActionInstance() const					{ return new ActionCodeInstance(this); }
	Status status() const													{ return Alpha; }
	Category category() const												{ return Internal; }
	QPixmap icon() const													{ return QPixmap(":/icons/code.png"); }

private:
	Q_DISABLE_COPY(ActionCodeDefinition)
};

#endif // ACTIONCODEDEFINITION_H
