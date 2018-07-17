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

#include "codestdio.h"

#include <QScriptContext>
#include <QTextStream>

namespace LibExecuter
{
	QScriptValue CodeStdio::constructor(QScriptContext *context, QScriptEngine *)
	{
		return context->thisObject();
	}
	
	QScriptValue CodeStdio::print(QScriptContext *context, QScriptEngine *)
	{
		print(context->argument(0).toString());
		
		return context->thisObject();
	}

	QScriptValue CodeStdio::println(QScriptContext *context, QScriptEngine *)
	{
		print(QStringLiteral("%1\n").arg(context->argument(0).toString()));

		return context->thisObject();
	}

	QScriptValue CodeStdio::printWarning(QScriptContext *context, QScriptEngine *)
	{
		print(tr("Warning: %1").arg(context->argument(0).toString()));

		return context->thisObject();
	}

	QScriptValue CodeStdio::printlnWarning(QScriptContext *context, QScriptEngine *)
	{
		print(tr("Warning: %1\n").arg(context->argument(0).toString()));

		return context->thisObject();
	}

	QScriptValue CodeStdio::printError(QScriptContext *context, QScriptEngine *)
	{
		print(tr("Error: %1").arg(context->argument(0).toString()));
		
		return context->thisObject();
	}

	QScriptValue CodeStdio::printlnError(QScriptContext *context, QScriptEngine *)
	{
		print(tr("Error: %1\n").arg(context->argument(0).toString()));

		return context->thisObject();
	}

    QScriptValue CodeStdio::clear(QScriptContext *context, QScriptEngine *)
    {
        // Do nothing here

        return context->thisObject();
    }

	void CodeStdio::print(const QString &text)
	{
		QTextStream stream(stdout);
		stream << text;
		stream.flush();
	}
}
