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

#ifndef ACTIONGOTO_H
#define ACTIONGOTO_H

#include "actioninterface.h"
#include "actiongotoinstance.h"
#include "lineparameterdefinition.h"

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionGoto : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionGoto(ActionTools::ActionPackInterface *pack)
	: ActionGoto::ActionInterface(pack)
	{
		ActionTools::LineParameterDefinition *line = new ActionTools::LineParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"line",
																								 tr("Line"),
																								 this);
		line->setTooltip(tr("The line (or label) to go to"));
		addElement(line);
	}

	QString name() const											{ return QObject::tr("Goto"); }
	QString id() const												{ return metaObject()->className(); }
	Flag flags() const												{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const										{ return QObject::tr("Go to a script line"); }
	Tools::Version version() const									{ return Tools::Version(0, 0, 1); }
	ActionTools::Action *newAction()								{ return new ActionGotoInstance(this, 0); }
	Status status() const											{ return Alpha; }
	Category category() const										{ return Internal; }
	QPixmap icon() const											{ return QPixmap(":/icons/goto.png"); }
	ActionTools::Action *scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionGoto) }

private:
	Q_DISABLE_COPY(ActionGoto)
};

#endif // ACTIONGOTO_H
