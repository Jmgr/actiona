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

#ifndef ACTIONPACKSYSTEM_H
#define ACTIONPACKSYSTEM_H

#include "actionpack.h"
#include "actions/commanddefinition.h"
#include "actions/killprocessdefinition.h"
#include "actions/notifydefinition.h"
#include "actions/pixelcolordefinition.h"

#include "code/system.h"
#include "code/mediaplaylist.h"
#include "code/notify.h"
#include "code/process.h"

#include <QtCore/qplugin.h>

namespace ActionTools
{
	class ActionDefinition;
}

class ActionPackSystem : public QObject, public ActionTools::ActionPack
{
	Q_OBJECT
	Q_INTERFACES(ActionTools::ActionPack)

public:
	ActionPackSystem()
	{
		addActionDefinition(new Actions::CommandDefinition(this));
		addActionDefinition(new Actions::KillProcessDefinition(this));
		addActionDefinition(new Actions::NotifyDefinition(this));
		addActionDefinition(new Actions::PixelColorDefinition(this));
	}

	QString id() const							{ return tr("system"); }
	QString name() const						{ return tr("Actions dealing with the operating system"); }
	Tools::Version version() const				{ return Tools::Version(0, 0, 1); }
	
	void codeInit(QScriptEngine *scriptEngine) const
	{
		addCodeClass<Code::System>("System", scriptEngine);
		addCodeClass<Code::MediaPlaylist>("MediaPlaylist", scriptEngine);
		addCodeClass<Code::Notify>("Notify", scriptEngine);
		addCodeClass<Code::Process>("Process", scriptEngine);
		addCodeStaticMethod(&Code::Process::list, "Process", "list", scriptEngine);
		addCodeStaticMethod(&Code::Process::startDetached, "Process", "startDetached", scriptEngine);
		addCodeStaticMethod(&Code::Process::thisProcess, "Process", "thisProcess", scriptEngine);
	}

private:
	Q_DISABLE_COPY(ActionPackSystem)
};

Q_EXPORT_PLUGIN2(ActionPackSystem, ActionPackSystem)

#endif // ACTIONPACKSYSTEM_H
