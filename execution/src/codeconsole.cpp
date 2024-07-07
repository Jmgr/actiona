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

#include "codeconsole.hpp"
#include "execution.hpp"
#include "actiontools/actioninstance.hpp"
#include "actiontools/script.hpp"
#include "actiontools/scriptengine.hpp"

#include <QApplication>

namespace Execution
{
    void CodeConsole::registerClass(ActionTools::ScriptEngine &scriptEngine, Executer &executer)
    {
        qRegisterMetaType<CodeConsole*>("const Console *");

        CodeClass::registerStaticClass<CodeConsole>(QStringLiteral("Console"), scriptEngine, executer);
    }

    CodeConsole *CodeConsole::print(const QString &text)
    {
        if(!mExecuter.isExecuterRunning())
            return this;

        printCall(text, ActionTools::ConsoleWidget::Information);

        return this;
    }

    CodeConsole *CodeConsole::printWarning(const QString &text)
    {
        if(!mExecuter.isExecuterRunning())
            return this;

        printCall(text, ActionTools::ConsoleWidget::Warning);

        return this;
    }

    CodeConsole *CodeConsole::printError(const QString &text)
    {
        if(!mExecuter.isExecuterRunning())
            return this;

        printCall(text, ActionTools::ConsoleWidget::Error);

        return this;
    }

    CodeConsole *CodeConsole::clear()
    {
        if(!mExecuter.isExecuterRunning())
            return this;

        QApplication::processEvents();//Call this to prevent UI freeze when calling clear often

        mExecuter.consoleWidget()->clearExceptSeparators();

        return this;
    }

    void CodeConsole::printCall(const QString &text, ActionTools::ConsoleWidget::Type type)
    {
        QApplication::processEvents();//Call this to prevent UI freeze when calling print often

        switch(mExecuter.scriptEngine().context())
        {
        case ActionTools::ScriptEngine::Parameters:
        {
            auto [line, column] = mExecuter.scriptEngine().currentLineAndColumn();

            mExecuter.consoleWidget()->addScriptParameterLine(type, text,
                                                              mExecuter.currentParameter(),
                                                              line,
                                                              column);
        }
        break;
        case ActionTools::ScriptEngine::Actions:
        {
            ActionTools::ActionInstance *currentAction = mExecuter.script()->actionAt(mExecuter.currentActionIndex());
            qint64 currentActionRuntimeId = -1;
            if(currentAction)
                currentActionRuntimeId = currentAction->runtimeId();

            auto [line, column] = mExecuter.scriptEngine().currentLineAndColumn();

            mExecuter.consoleWidget()->addUserLine(type, text,
                                                   currentActionRuntimeId,
                                                   ActionTools::ScriptEngine::current()->globalObject().property(QStringLiteral("currentParameter")).toString(),
                                                   ActionTools::ScriptEngine::current()->globalObject().property(QStringLiteral("currentSubParameter")).toString(),
                                                   line,
                                                   column);
        }
        break;
        default:
            return;
        }
    }
}
