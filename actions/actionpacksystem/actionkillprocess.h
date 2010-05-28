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

#ifndef ACTIONKILLPROCESS_H
#define ACTIONKILLPROCESS_H

#include "actioninterface.h"
#include "actionkillprocessinstance.h"
#include "textparameterdefinition.h"
#include "numberparameterdefinition.h"
#include "listparameterdefinition.h"

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionKillProcess : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionKillProcess(ActionTools::ActionPackInterface *pack)
	: ActionInterface(pack)
	{
		ActionTools::TextParameterDefinition *processId = new ActionTools::TextParameterDefinition( ActionTools::ElementDefinition::INPUT,
																								"processId",
																								 tr("Process id"),
																								 this);
		processId->setTooltip(tr("The process id of the process to kill"));
		addElement(processId);
		
		ActionTools::ListParameterDefinition *killMode = new ActionTools::ListParameterDefinition(	ActionTools::ElementDefinition::INPUT,
																								"killMode",
																								tr("Kill mode"),
																								this);
		killMode->setTooltip(tr("The kill mode"));
		killMode->setItems(ActionKillProcessInstance::killModes);
		killMode->setOption("default", ActionKillProcessInstance::killModes.second.at(ActionKillProcessInstance::GracefulThenForceful));
		addElement(killMode, 1);
		
		ActionTools::NumberParameterDefinition *timeout = new ActionTools::NumberParameterDefinition( ActionTools::ElementDefinition::INPUT,
																								"timeout",
																								 tr("Timeout"),
																								 this);
		timeout->setTooltip(tr("The timeout before doing a forceful kill"));
		timeout->setMinimum(0);
		timeout->setMaximum(INT_MAX);
		timeout->setOption("default", 1000);
		addElement(timeout, 1);
	}

	QString name() const											{ return QObject::tr("Kill process"); }
	QString id() const												{ return metaObject()->className(); }
	Flag flags() const												{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const										{ return QObject::tr("Kills a process"); }
	Tools::Version version() const									{ return Tools::Version(1, 0, 0); }
	ActionTools::Action *newAction()								{ return new ActionKillProcessInstance(this, 0); }
	Status status() const											{ return Stable; }
	Category category() const										{ return System; }
	QString author() const											{ return "The Actionaz Team"; }
	QString website() const											{ return "www.actionaz.eu"; }
	QString email() const											{ return "jmgr@jmgr.info"; }
	QPixmap icon() const											{ return QPixmap(":/icons/clipboard.png"); }
	ActionTools::Action *scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionKillProcess) }
	QStringList tabs() const										{ return QStringList() << tr("Standard") << tr("Advanced"); }

private:
	Q_DISABLE_COPY(ActionKillProcess)
};

#endif // ACTIONKILLPROCESS_H
