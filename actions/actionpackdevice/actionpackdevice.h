/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#ifndef ACTIONPACKDEVICE_H
#define ACTIONPACKDEVICE_H

#include "actionpack.h"
#include "actions/textdefinition.h"
#include "actions/clickdefinition.h"
#include "actions/wheeldefinition.h"
#include "actions/keydefinition.h"
#include "actions/movecursordefinition.h"
#include "actions/cursorpathdefinition.h"

#include "code/mouse.h"
#include "code/keyboard.h"

#include <QtCore/qplugin.h>

namespace ActionTools
{
	class ActionDefinition;
}

class ActionPackDevice : public QObject, public ActionTools::ActionPack
{
	Q_OBJECT
	Q_INTERFACES(ActionTools::ActionPack)

public:
	ActionPackDevice()							{}

	void createDefinitions()
	{
		addActionDefinition(new Actions::TextDefinition(this));
		addActionDefinition(new Actions::ClickDefinition(this));
		addActionDefinition(new Actions::WheelDefinition(this));
		addActionDefinition(new Actions::KeyDefinition(this));
		addActionDefinition(new Actions::MoveCursorDefinition(this));
		addActionDefinition(new Actions::CursorPathDefinition(this));
	}

	QString id() const							{ return "device"; }
	QString name() const						{ return tr("Actions dealing with devices like the keyboard or the mouse"); }
	Tools::Version version() const				{ return Tools::Version(0, 0, 1); }
	
	void codeInit(QScriptEngine *scriptEngine) const
	{
		addCodeClass<Code::Mouse>("Mouse", scriptEngine);
		addCodeClass<Code::Keyboard>("Keyboard", scriptEngine);
	}

private:
	Q_DISABLE_COPY(ActionPackDevice)
};

Q_EXPORT_PLUGIN2(ActionPackDevice, ActionPackDevice)

#endif // ACTIONPACKDEVICE_H
