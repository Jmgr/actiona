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

#include <QVersionNumber>

namespace Execution
{
    class EXECUTIONSHARED_EXPORT CodeActiona : public Code::CodeClass
	{
        friend class StaticCodeActiona;

		Q_OBJECT

	public:
        Q_INVOKABLE CodeActiona() = default;

        Q_INVOKABLE QString toString() const override { return QStringLiteral("Actiona"); }

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

		static void setActExec(bool isActExec);
        static void setActionaVersion(const QVersionNumber &version);
		static void setScriptVersion(const QVersionNumber &version);
	private:
		static bool mIsActExec;
        static QVersionNumber mActionaVersion;
		static QVersionNumber mScriptVersion;
	};

    class EXECUTIONSHARED_EXPORT StaticCodeActiona : public Code::CodeClass
    {
        Q_OBJECT

    public:
        StaticCodeActiona(QObject *parent): CodeClass(parent) {}

        Q_INVOKABLE QString toString() const override { return QStringLiteral("StaticActiona"); }
        Q_INVOKABLE QString version();
        Q_INVOKABLE QString scriptVersion();
        Q_INVOKABLE bool isActExec();
        Q_INVOKABLE bool isActiona();
    };
}

