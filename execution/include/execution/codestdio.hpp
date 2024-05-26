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
    class EXECUTIONSHARED_EXPORT CodeStdio : public Code::CodeClass
	{
		Q_OBJECT

	public:
        Q_INVOKABLE CodeStdio() = default;

        Q_INVOKABLE QString toString() const override { return QStringLiteral("Stdio"); }

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
	};

    class EXECUTIONSHARED_EXPORT StaticCodeStdio : public Code::CodeClass
    {
        Q_OBJECT

    public:
        StaticCodeStdio(QObject *parent): CodeClass(parent) {}

        Q_INVOKABLE QString toString() const override { return QStringLiteral("StaticStdio"); }
        Q_INVOKABLE StaticCodeStdio *print(const QString &text);
        Q_INVOKABLE StaticCodeStdio *println(const QString &text);
        Q_INVOKABLE StaticCodeStdio *printWarning(const QString &text);
        Q_INVOKABLE StaticCodeStdio *printlnWarning(const QString &text);
        Q_INVOKABLE StaticCodeStdio *printError(const QString &text);
        Q_INVOKABLE StaticCodeStdio *printlnError(const QString &text);
        Q_INVOKABLE StaticCodeStdio *clear();

    private:
        void printInternal(const QString &text);
    };
}

