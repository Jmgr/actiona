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

#include "actiontools_global.h"

#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT CodeTools
	{
	public:
		template<typename T>
		static void addClassToScriptEngine(const QString &name, QScriptEngine *scriptEngine)
		{
			QScriptValue metaObject = scriptEngine->newQMetaObject(&T::staticMetaObject, scriptEngine->newFunction(&T::constructor));
			scriptEngine->globalObject().setProperty(name, metaObject);
		}
		
		template<typename T>
		static void addClassToScriptEngine(QScriptEngine *scriptEngine)
		{
			addClassToScriptEngine<T>(removeCodeNamespace(QLatin1String(T::staticMetaObject.className())), scriptEngine);
		}

		static void addClassGlobalFunctionToScriptEngine(const QString &className, QScriptEngine::FunctionSignature function, const QString &functionName, QScriptEngine *scriptEngine);

		template<typename T>
		static void addClassGlobalFunctionToScriptEngine(QScriptEngine::FunctionSignature function, const QString &functionName, QScriptEngine *scriptEngine)
		{
			addClassGlobalFunctionToScriptEngine(removeCodeNamespace(QLatin1String(T::staticMetaObject.className())), function, functionName, scriptEngine);
		}

	private:
		static QString removeCodeNamespace(const QString &className);
	};
}

