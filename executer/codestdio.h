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

#pragma once

#include "executer_global.h"

#include <QObject>
#include <QScriptValue>

class QScriptContext;
class QScriptEngine;

namespace LibExecuter
{
	class EXECUTERSHARED_EXPORT CodeStdio : public QObject
	{
		Q_OBJECT

	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
				
		static QScriptValue print(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue println(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue printWarning(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue printlnWarning(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue printError(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue printlnError(QScriptContext *context, QScriptEngine *engine);
        static QScriptValue clear(QScriptContext *context, QScriptEngine *engine);

	private:
		static void print(const QString &text);
	};
}

