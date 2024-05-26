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

namespace Execution
{
    class EXECUTIONSHARED_EXPORT CodeExecution : public Code::CodeClass
	{
		Q_OBJECT
		
	public:
        Q_INVOKABLE CodeExecution() = default;

        Q_INVOKABLE QString toString() const override { return QStringLiteral("Execution"); }

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
	};

    class EXECUTIONSHARED_EXPORT StaticCodeExecution : public Code::CodeClass
    {
        Q_OBJECT

    public:
        StaticCodeExecution(QObject *parent): CodeClass(parent) {}

        Q_INVOKABLE QString toString() const override { return QStringLiteral("StaticExecution"); }
        Q_INVOKABLE StaticCodeExecution *pause(int duration);
        Q_INVOKABLE StaticCodeExecution *sleep(int duration);
        Q_INVOKABLE StaticCodeExecution *stop();
    };
}

