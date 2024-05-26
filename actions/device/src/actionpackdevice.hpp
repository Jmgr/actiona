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

	Contact: jmgr@jmgr.info
*/

#pragma once

#include "actiontools/actionpack.hpp"
#include "device_global.hpp"
#include "actions/textdefinition.hpp"
#include "actions/clickdefinition.hpp"
#include "actions/wheeldefinition.hpp"
#include "actions/keydefinition.hpp"
#include "actions/movecursordefinition.hpp"
#include "actions/cursorpathdefinition.hpp"
#include "actions/keyboardkeyconditiondefinition.hpp"

#include "code/mouse.hpp"
#include "code/keyboard.hpp"

namespace ActionTools
{
	class ActionDefinition;
}

class ACTIONPACKDEVICE_EXPORT ActionPackDevice : public QObject, public ActionTools::ActionPack
{
	Q_OBJECT

public:
	ActionPackDevice()							= default;

	void createDefinitions() override
	{
		addActionDefinition(new Actions::TextDefinition(this));
		addActionDefinition(new Actions::ClickDefinition(this));
		addActionDefinition(new Actions::WheelDefinition(this));
		addActionDefinition(new Actions::KeyDefinition(this));
		addActionDefinition(new Actions::MoveCursorDefinition(this));
		addActionDefinition(new Actions::CursorPathDefinition(this));
        addActionDefinition(new Actions::KeyboardKeyConditionDefinition(this));
	}

	QString id() const override							{ return QStringLiteral("device"); }
	QString name() const override						{ return tr("Actions dealing with devices like the keyboard or the mouse"); }

    void codeInit(ActionTools::ScriptEngine &scriptEngine) const override
    {
        Code::Mouse::registerClass(scriptEngine);
        Code::Keyboard::registerClass(scriptEngine);
	}

private:
	Q_DISABLE_COPY(ActionPackDevice)
};

