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

#include "codetools.h"

#include <QStringList>
#include <QScriptEngine>

namespace Code
{
	void CodeTools::addClassGlobalFunctionToScriptEngine(const QString &className, QScriptEngine::FunctionSignature function, const QString &functionName, QScriptEngine *scriptEngine)
	{
		QScriptValue classMetaObject = scriptEngine->globalObject().property(className);
		if(!classMetaObject.isValid())
		{
			classMetaObject = scriptEngine->newObject();
			scriptEngine->globalObject().setProperty(className, classMetaObject);
		}

		classMetaObject.setProperty(functionName, scriptEngine->newFunction(function));
	}

	QString CodeTools::removeCodeNamespace(const QString &className)
	{
		if(className.startsWith(QStringLiteral("Code::")))
			return className.right(className.size() - 6);
		else
			return className;
	}
}
