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

#ifndef ACTIONSTOP_H
#define ACTIONSTOP_H

#include "actioninterface.h"
#include "actionstopinstance.h"

namespace ActionTools
{
	class ActionPackInterface;
	class Action;
}

class ActionStop : public QObject, public ActionTools::ActionInterface
{
   Q_OBJECT

public:
	explicit ActionStop(ActionTools::ActionPackInterface *pack)
	: ActionInterface(pack)
	{
	}

	QString name() const											{ return QObject::tr("Stop"); }
	QString id() const												{ return metaObject()->className(); }
	Flag flags() const												{ return WorksOnWindows | WorksOnGnuLinux | WorksOnMac | Official; }
	QString description() const										{ return QObject::tr("Stop the script execution"); }
	Tools::Version version() const									{ return Tools::Version(0, 0, 1); }
	ActionTools::Action *newAction()								{ return new ActionStopInstance(this, 0); }
	Status status() const											{ return Stable; }
	Category category() const										{ return Internal; }
	QPixmap icon() const											{ return QPixmap(":/icons/goto.png"); }
	ActionTools::Action *scriptInit(QScriptEngine *scriptEngine)	{ SCRIPT_INIT(ActionStop) }

private:
	Q_DISABLE_COPY(ActionStop)
};

#endif // ACTIONSTOP_H
