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

#include "codestdio.hpp"

#include <QTextStream>

namespace Execution
{
    void CodeStdio::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<CodeStdio*>("const Stdio *");

        CodeClass::registerClassWithStaticFunctions<CodeStdio, StaticCodeStdio>(
            QStringLiteral("Stdio"),
            {QStringLiteral("print"), QStringLiteral("println"), QStringLiteral("printWarning"), QStringLiteral("printlnWarning"),
             QStringLiteral("printError"), QStringLiteral("printlnError"), QStringLiteral("clear")},
            scriptEngine
            );
    }

    StaticCodeStdio *StaticCodeStdio::print(const QString &text)
    {
        printInternal(text);

        return this;
    }

    StaticCodeStdio *StaticCodeStdio::println(const QString &text)
    {
        printInternal(QStringLiteral("%1\n").arg(text));

        return this;
    }

    StaticCodeStdio *StaticCodeStdio::printWarning(const QString &text)
    {
        printInternal(tr("Warning: %1").arg(text));

        return this;
    }

    StaticCodeStdio *StaticCodeStdio::printlnWarning(const QString &text)
    {
        printInternal(tr("Warning: %1\n").arg(text));

        return this;
    }

    StaticCodeStdio *StaticCodeStdio::printError(const QString &text)
    {
        printInternal(tr("Error: %1").arg(text));

        return this;
    }

    StaticCodeStdio *StaticCodeStdio::printlnError(const QString &text)
    {
        printInternal(tr("Error: %1\n").arg(text));

        return this;
    }

    StaticCodeStdio *StaticCodeStdio::clear()
    {
        // Do nothing here

        return this;
    }

    void StaticCodeStdio::printInternal(const QString &text)
    {
        QTextStream stream(stdout);
        stream << text;
        stream.flush();
    }
}
