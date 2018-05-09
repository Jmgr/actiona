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

	Contact : jmgr@jmgr.info
*/

#include "codeactiona.h"

#include <QScriptContext>
#include <QTextStream>

namespace LibExecuter
{
    bool CodeActiona::mIsActExec = false;
    Tools::Version CodeActiona::mActionaVersion = Tools::Version();
    Tools::Version CodeActiona::mScriptVersion = Tools::Version();

    QScriptValue CodeActiona::constructor(QScriptContext *context, QScriptEngine *)
	{
		return context->thisObject();
	}

    QScriptValue CodeActiona::version(QScriptContext *, QScriptEngine *)
	{
        return mActionaVersion.toString();
	}

    QScriptValue CodeActiona::scriptVersion(QScriptContext *, QScriptEngine *)
	{
		return mScriptVersion.toString();
	}

    QScriptValue CodeActiona::isActExec(QScriptContext *, QScriptEngine *)
	{
		return mIsActExec;
	}

    QScriptValue CodeActiona::isActiona(QScriptContext *, QScriptEngine *)
	{
		return !mIsActExec;
	}

    void CodeActiona::setActExec(bool isActExec)
	{
		mIsActExec = isActExec;
	}

    void CodeActiona::setActionaVersion(const Tools::Version &version)
	{
        mActionaVersion = version;
	}

    void CodeActiona::setScriptVersion(const Tools::Version &version)
	{
		mScriptVersion = version;
	}
}
