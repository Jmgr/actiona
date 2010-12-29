/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

	Actionaz is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Actionaz is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Contact : jmgr@jmgr.info
*/

#ifndef CODE_H
#define CODE_H

#include "actiontools_global.h"

#include <QString>
#include <QByteArray>
#include <QScriptValue>
#include <QScriptEngine>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT Code
	{
	public:
		enum Encoding
		{
			Native,
			Ascii,
			Latin1,
			UTF8,
		};

		template<typename T>
		static void addClassToScriptEngine(const QString &name, QScriptEngine *scriptEngine)
		{
			QScriptValue metaObject = scriptEngine->newQMetaObject(&T::staticMetaObject, scriptEngine->newFunction(&T::constructor));
			scriptEngine->globalObject().setProperty(name, metaObject);
		}
		
		template<typename T>
		static void addClassToScriptEngine(QScriptEngine *scriptEngine)
		{
			addClassToScriptEngine<T>(removeCodeNamespace(T::staticMetaObject.className()), scriptEngine);
		}

		static void addClassGlobalFunctionToScriptEngine(const QString &className, QScriptEngine::FunctionSignature function, const QString &functionName, QScriptEngine *scriptEngine)
		{
			QScriptValue classMetaObject = scriptEngine->globalObject().property(className);
			if(!classMetaObject.isValid())
			{
				classMetaObject = scriptEngine->newObject();
				scriptEngine->globalObject().setProperty(className, classMetaObject);
			}

			classMetaObject.setProperty(functionName, scriptEngine->newFunction(function));
		}

		template<typename T>
		static void addClassGlobalFunctionToScriptEngine(QScriptEngine::FunctionSignature function, const QString &functionName, QScriptEngine *scriptEngine)
		{
			addClassGlobalFunctionToScriptEngine(removeCodeNamespace(T::staticMetaObject.className()), function, functionName, scriptEngine);
		}

		static QByteArray toEncoding(const QString &string, Encoding encoding);
		static QString fromEncoding(const QByteArray &byteArray, Encoding encoding);
		static QStringList arrayParameterToStringList(const QScriptValue &scriptValue);
		static QScriptValue stringListToArrayParameter(QScriptEngine *engine, const QStringList &stringList);

	private:
		static QString removeCodeNamespace(const QString &className)
		{
			if(className.startsWith("Code::"))
				return className.right(className.size() - 6);
			else
				return className;
		}
	};
}

#endif // CODE_H
