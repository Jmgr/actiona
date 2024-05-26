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

#include "../keyboarddevice.hpp"
#include "actiontools/code/codeclass.hpp"

#include <QJSValue>

namespace Code
{
	class Keyboard : public CodeClass
	{
		Q_OBJECT

	public:
        Q_INVOKABLE Keyboard();

        Q_INVOKABLE QString toString() const override                                { return QStringLiteral("Keyboard"); }
        Q_INVOKABLE Keyboard *pressKey(const QString &key);
        Q_INVOKABLE Keyboard *releaseKey(const QString &key);
        Q_INVOKABLE Keyboard *triggerKey(const QString &key);
        Q_INVOKABLE Keyboard *writeText(const QString &text, int delay = 0, bool noUnicodeCharacters = false);

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private:
		KeyboardDevice mKeyboardDevice;
	};
}

