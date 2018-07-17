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

#include "actionpack.h"
#include "actions/commanddefinition.h"
#include "actions/killprocessdefinition.h"
#include "actions/notifydefinition.h"
#include "actions/pixelcolordefinition.h"
#include "actions/systemdefinition.h"
#include "actions/openurldefinition.h"
#include "actions/detachedcommanddefinition.h"
#include "actions/playsounddefinition.h"
#include "actions/findimagedefinition.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include "actions/texttospeechdefinition.h"
#endif

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
    Q_PLUGIN_METADATA(IID "tools.actiona.ActionPack" FILE "system.json")

public:
    ActionPackSystem()                           = default;

	void createDefinitions() override
	{
		addActionDefinition(new Actions::CommandDefinition(this));
		addActionDefinition(new Actions::KillProcessDefinition(this));
		addActionDefinition(new Actions::NotifyDefinition(this));
		addActionDefinition(new Actions::PixelColorDefinition(this));
		addActionDefinition(new Actions::SystemDefinition(this));
		addActionDefinition(new Actions::OpenURLDefinition(this));
		addActionDefinition(new Actions::DetachedCommandDefinition(this));
		addActionDefinition(new Actions::PlaySoundDefinition(this));
		addActionDefinition(new Actions::FindImageDefinition(this));
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
        addActionDefinition(new Actions::TextToSpeechDefinition(this));
#endif
	}

	QString id() const override							{ return QStringLiteral("system"); }
	QString name() const override						{ return tr("Actions dealing with the operating system"); }
	Tools::Version version() const override				{ return Tools::Version(0, 0, 1); }

	void codeInit(QScriptEngine *scriptEngine) const override
	{
		addCodeClass<Code::System>(QStringLiteral("System"), scriptEngine);
		addCodeClass<Code::MediaPlaylist>(QStringLiteral("MediaPlaylist"), scriptEngine);
		addCodeClass<Code::Notify>(QStringLiteral("Notify"), scriptEngine);
		addCodeClass<Code::Process>(QStringLiteral("Process"), scriptEngine);
		addCodeStaticMethod(&Code::Process::list, QStringLiteral("Process"), QStringLiteral("list"), scriptEngine);
		addCodeStaticMethod(&Code::Process::startDetached, QStringLiteral("Process"), QStringLiteral("startDetached"), scriptEngine);
		addCodeStaticMethod(&Code::Process::thisProcess, QStringLiteral("Process"), QStringLiteral("thisProcess"), scriptEngine);
	}

private:
	Q_DISABLE_COPY(ActionPackSystem)
};

