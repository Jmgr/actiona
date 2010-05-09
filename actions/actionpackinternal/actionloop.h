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

#ifndef ACTIONLOOP_H
#define ACTIONLOOP_H

#include "actioninterface.h"
#include "actionloopinstance.h"
#include "lineparameterdefinition.h"
#include "numberparameterdefinition.h"

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionLoop : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionLoop(ActionTools::ActionPackInterface *pack)
	: ActionLoop::ActionInterface(pack)
	{
		ActionTools::LineParameterDefinition *line = new ActionTools::LineParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"line",
																								 tr("Line"),
																								 this);
		line->setTooltip(tr("The line (or label) to go to"));
		addElement(line);
		ActionTools::NumberParameterDefinition *count = new ActionTools::NumberParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"count",
																								 tr("Count"),
																								 this);
		count->setTooltip(tr("The amount of times (evaluated the first time)"));
		count->setMinimum(0);
		count->setMaximum(INT_MAX);
		addElement(count);
	}

	QString name() const							{ return QObject::tr("Loop"); }
	QString id() const								{ return metaObject()->className(); }
	Flag flags() const								{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const						{ return QObject::tr("Go to a script line a specific number of times"); }
	Tools::Version version() const					{ return Tools::Version(0, 0, 1); }
	ActionTools::Action *newAction()				{ return new ActionLoopInstance(this, 0); }
	Status status() const							{ return Alpha; }
	Category category() const						{ return Internal; }
	QString author() const							{ return "Jmgr"; }
	QString website() const							{ return "www.jmgr.info"; }
	QString email() const							{ return "jmgr@jmgr.info"; }
	QPixmap icon() const							{ return QPixmap(":/icons/loop.png"); }
	void scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionLoop) }

private:
	Q_DISABLE_COPY(ActionLoop)
};

#endif // ACTIONLOOP_H
