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
    class Executer;

    class EXECUTIONSHARED_EXPORT CodeScript : public Code::CodeClass
    {
        Q_OBJECT
        Q_PROPERTY(QString nextLine READ nextLine WRITE setNextLine)
        Q_PROPERTY(bool doNotResetPreviousActions READ doNotResetPreviousActions WRITE setDoNotResetPreviousActions)
        Q_PROPERTY(int line READ line WRITE setLine)

    public:
        CodeScript(QObject *parent, Executer &executer): CodeClass(parent), mExecuter(executer) {}

        void setNextLine(const QString &nextLine) { mNextLine = nextLine; }
        QString nextLine() const { return mNextLine; }

        void setDoNotResetPreviousActions(bool value) { mDoNotResetPreviousActions = value; }
        bool doNotResetPreviousActions() const { return mDoNotResetPreviousActions; }

        void setLine(int line) { mLine = line; }
        int line() const { return mLine; }

        Q_INVOKABLE QString toString() const override { return QStringLiteral("Script"); }
        Q_INVOKABLE CodeScript *callProcedure(const QString &procedure);

        static void registerClass(ActionTools::ScriptEngine &scriptEngine, Executer &executer);

    private:
        Executer &mExecuter;
        QString mNextLine{QStringLiteral("1")};
        bool mDoNotResetPreviousActions{false};
        int mLine{1};
    };
}

