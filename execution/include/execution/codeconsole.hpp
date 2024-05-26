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
#include "actiontools/consolewidget.hpp"

namespace Execution
{
    class Executer;

    class EXECUTIONSHARED_EXPORT CodeConsole : public Code::CodeClass
    {
        Q_OBJECT

    public:
        CodeConsole(QObject *parent, Executer &executer): CodeClass(parent), mExecuter(executer) {}

        Q_INVOKABLE QString toString() const override { return QStringLiteral("Console"); }
        Q_INVOKABLE CodeConsole *print(const QString &text);
        Q_INVOKABLE CodeConsole *printWarning(const QString &text);
        Q_INVOKABLE CodeConsole *printError(const QString &text);
        Q_INVOKABLE CodeConsole *clear();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine, Executer &executer);

    private:
        void printCall(const QString &text, ActionTools::ConsoleWidget::Type type);

        Executer &mExecuter;
    };
}

