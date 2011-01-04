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

#include "code.h"

#include <QScriptValueIterator>
#include <QStringList>
#include <QScriptEngine>

namespace Code
{
	void Code::addClassGlobalFunctionToScriptEngine(const QString &className, QScriptEngine::FunctionSignature function, const QString &functionName, QScriptEngine *scriptEngine)
	{
		QScriptValue classMetaObject = scriptEngine->globalObject().property(className);
		if(!classMetaObject.isValid())
		{
			classMetaObject = scriptEngine->newObject();
			scriptEngine->globalObject().setProperty(className, classMetaObject);
		}

		classMetaObject.setProperty(functionName, scriptEngine->newFunction(function));
	}

	QByteArray Code::toEncoding(const QString &string, Encoding encoding)
	{
		switch(encoding)
		{
		case Code::Native:
			return string.toLocal8Bit();
		case Code::Ascii:
			return string.toAscii();
		case Code::Latin1:
			return string.toLatin1();
		case Code::UTF8:
			return string.toUtf8();
		default:
			return QByteArray();
		}
	}
	
	QString Code::fromEncoding(const QByteArray &byteArray, Encoding encoding)
	{
		switch(encoding)
		{
		case Code::Native:
			return QString::fromLocal8Bit(byteArray.data(), byteArray.size());
		case Code::Ascii:
			return QString::fromAscii(byteArray.data(), byteArray.size());
		case Code::Latin1:
			return QString::fromLatin1(byteArray.data(), byteArray.size());
		case Code::UTF8:
			return QString::fromUtf8(byteArray.data(), byteArray.size());
		default:
			return QString();
		}
	}
	
	QStringList Code::arrayParameterToStringList(const QScriptValue &scriptValue)
	{
		QStringList back;
		
		QScriptValueIterator it(scriptValue);
		while(it.hasNext())
		{
			it.next();
			
			back << it.value().toString();
		}
		
		return back;
	}
	
	QScriptValue Code::stringListToArrayParameter(QScriptEngine *engine, const QStringList &stringList)
	{
		if(stringList.count() == 0)
			return engine->undefinedValue();
		
		QScriptValue back = engine->newArray(stringList.count());
		
		for(int index = 0; index < stringList.count(); ++index)
			back.setProperty(index, stringList.at(index));
		
		return back;
	}

	QString Code::removeCodeNamespace(const QString &className)
	{
		if(className.startsWith("Code::"))
			return className.right(className.size() - 6);
		else
			return className;
	}
}
