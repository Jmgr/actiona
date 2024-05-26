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

#include "codeactiona.hpp"

#include <QTextStream>

namespace Execution
{
    bool CodeActiona::mIsActExec = false;
    QVersionNumber CodeActiona::mActionaVersion = QVersionNumber();
    QVersionNumber CodeActiona::mScriptVersion = QVersionNumber();

    void CodeActiona::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<CodeActiona*>("const Actiona *");

        CodeClass::registerClassWithStaticFunctions<CodeActiona, StaticCodeActiona>(
            QStringLiteral("Actiona"),
            {QStringLiteral("version"), QStringLiteral("scriptVersion"), QStringLiteral("isActExec"), QStringLiteral("isActiona")},
            scriptEngine
            );
    }

    void CodeActiona::setActExec(bool isActExec)
	{
		mIsActExec = isActExec;
	}

    void CodeActiona::setActionaVersion(const QVersionNumber &version)
	{
        mActionaVersion = version;
	}

    void CodeActiona::setScriptVersion(const QVersionNumber &version)
	{
		mScriptVersion = version;
	}

    QString StaticCodeActiona::version()
    {
        return CodeActiona::mActionaVersion.toString();
    }

    QString StaticCodeActiona::scriptVersion()
    {
        return CodeActiona::mScriptVersion.toString();
    }

    bool StaticCodeActiona::isActExec()
    {
        return CodeActiona::mIsActExec;
    }

    bool StaticCodeActiona::isActiona()
    {
        return !CodeActiona::mIsActExec;
    }
}
