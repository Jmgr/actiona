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

#include "execution_global.hpp"
#include "actiontools/code/codeclass.hpp"

namespace ActionTools
{
	class ActionFactory;
}

namespace Execution
{
	class ScriptAgent;
	
	class EXECUTIONSHARED_EXPORT CodeInitializer
	{
	public:
        static void initialize(ActionTools::ScriptEngine &scriptEngine,
                               ActionTools::ActionFactory *actionFactory,
                               const QString &filename);
	};

    class EXECUTIONSHARED_EXPORT CodeGlobal : public Code::CodeClass
    {
        Q_OBJECT

    public:
        Q_INVOKABLE CodeGlobal(QObject *parent): Code::CodeClass(parent) {};

        Q_INVOKABLE QString toString() const override { return QStringLiteral("Global"); }
        Q_INVOKABLE QJSValue includeFunction(const QString &filepath);

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
    };
}

