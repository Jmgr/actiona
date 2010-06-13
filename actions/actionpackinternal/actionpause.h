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

#ifndef ACTIONPAUSE_H
#define ACTIONPAUSE_H

#include "actioninterface.h"
#include "numberparameterdefinition.h"
#include "actionpauseinstance.h"

#include <limits.h>

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionPause : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionPause(ActionTools::ActionPackInterface *pack)
	: ActionPause::ActionInterface(pack)
	{
		ActionTools::NumberParameterDefinition *duration = new ActionTools::NumberParameterDefinition(ActionTools::ElementDefinition::INPUT,
																								"duration",
																								tr("Duration"),
																								this);

		duration->setTooltip(tr("The duration of the pause"));
		duration->setSuffix(tr(" ms", "milliseconds"));
		duration->setMinimum(0);
		duration->setMaximum(INT_MAX);
		addElement(duration);
	}

	QString name() const											{ return QObject::tr("Pause"); }
	QString id() const												{ return metaObject()->className(); }
	Flag flags() const												{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const										{ return QObject::tr("Pauses the script execution"); }
	Tools::Version version() const									{ return Tools::Version(0, 0, 1); }
	ActionTools::Action *newAction()								{ return new ActionPauseInstance(this, 0); }
	Status status() const											{ return Alpha; }
	Category category() const										{ return Internal; }
	QString author() const											{ return "The Actionaz Team"; }
	QString website() const											{ return "www.actionaz.eu"; }
	QString email() const											{ return "jmgr@jmgr.info"; }
	QPixmap icon() const											{ return QPixmap(":/icons/sleep.png"); }
	ActionTools::Action *scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionPause) }

private:
	Q_DISABLE_COPY(ActionPause)
};

#endif // ACTIONPAUSE_H
