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

#include "codeclass.h"

#include <QScriptValue>
#include <QScriptEngine>
#include <QStringList>
#include <QScriptValueIterator>

namespace Code
{
	void CodeClass::throwError(QScriptContext *context, QScriptEngine *engine, const QString &errorType, const QString &message, const QString &parent)
	{
		QScriptValue errorTypeValue = engine->globalObject().property(errorType);
		if(!errorTypeValue.isValid())
		{
			errorTypeValue = engine->newFunction(emptyFunction);
			engine->globalObject().setProperty(errorType, errorTypeValue);
			errorTypeValue.setProperty(QStringLiteral("prototype"), engine->globalObject().property(parent).construct());
		}

		QScriptValue thrownError = errorTypeValue.construct();
		thrownError.setProperty(QStringLiteral("message"), message);
		thrownError.setProperty(QStringLiteral("name"), errorType);
		context->throwValue(thrownError);
	}

	CodeClass::CodeClass()
		: QObject(),
		QScriptable()
	{
	}

	void CodeClass::throwError(const QString &errorType, const QString &message, const QString &parent) const
	{
		throwError(context(), engine(), errorType, message, parent);
	}

	QScriptValue CodeClass::constructor(CodeClass *object, QScriptContext *context, QScriptEngine *engine)
	{
		if(context->isCalledAsConstructor())
        {
            engine->reportAdditionalMemoryCost(object->additionalMemoryCost());

            return engine->newQObject(context->thisObject(), object, QScriptEngine::ScriptOwnership);
        }
        else
			return constructor(object, engine);
	}

	QScriptValue CodeClass::constructor(CodeClass *object, QScriptEngine *engine)
	{
        engine->reportAdditionalMemoryCost(object->additionalMemoryCost());

        return engine->newQObject(object, QScriptEngine::ScriptOwnership);
	}

	QByteArray CodeClass::toEncoding(const QString &string, Encoding encoding)
	{
		switch(encoding)
		{
		case Native:
			return string.toLocal8Bit();
		case Ascii:
		case Latin1:
			return string.toLatin1();
		case UTF8:
			return string.toUtf8();
		default:
			return QByteArray();
		}
	}

	QString CodeClass::fromEncoding(const QByteArray &byteArray, Encoding encoding)
	{
		switch(encoding)
		{
		case Native:
			return QString::fromLocal8Bit(byteArray.data(), byteArray.size());
		case Ascii:
		case Latin1:
			return QString::fromLatin1(byteArray.data(), byteArray.size());
		case UTF8:
			return QString::fromUtf8(byteArray.data(), byteArray.size());
		default:
			return QString();
		}
	}

	QStringList CodeClass::arrayParameterToStringList(const QScriptValue &scriptValue)
	{
        QStringList back;

        qScriptValueToSequence(scriptValue, back);

		return back;
	}

	QScriptValue CodeClass::emptyFunction(QScriptContext *context, QScriptEngine *engine)
	{
		if(context->isCalledAsConstructor())
			return context->thisObject();
		else
			return engine->newObject();
	}
}
