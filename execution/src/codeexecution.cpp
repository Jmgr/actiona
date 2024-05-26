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

#include "codeexecution.hpp"
#include "actiontools/crossplatform.hpp"
#include "actiontools/scriptengine.hpp"

namespace Execution
{
    void CodeExecution::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<CodeExecution*>("const Execution *");

        CodeClass::registerClassWithStaticFunctions<CodeExecution, StaticCodeExecution>(
            QStringLiteral("Execution"),
            {QStringLiteral("pause"), QStringLiteral("sleep"), QStringLiteral("stop")},
            scriptEngine
            );
    }

    StaticCodeExecution *StaticCodeExecution::pause(int duration)
	{
        ActionTools::CrossPlatform::sleep(duration);

        return this;
	}
	
    StaticCodeExecution *StaticCodeExecution::sleep(int duration)
	{
        pause(duration);

        return this;
	}
	
    StaticCodeExecution *StaticCodeExecution::stop()
    {
        ActionTools::ScriptEngine::current()->setInterrupted(true);

        return this;
	}
}
