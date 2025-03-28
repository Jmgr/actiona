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

#include "actiontools/code/codeclass.hpp"
#include "actiontools/scriptengine.hpp"

#include <QJSValue>
#include <QStringList>

namespace Code
{
    void CodeClass::throwError(const QString &errorType, const QString &message) const
    {
        auto engine = ActionTools::ScriptEngine::current();
        auto error = engine->newErrorObject(QJSValue::TypeError, message);
        error.setProperty(QStringLiteral("name"), errorType);
        engine->throwError(error);
    }

    CodeClass::CodeClass(QObject *parent)
        : QObject(parent)
	{
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

	QStringList CodeClass::arrayParameterToStringList(const QJSValue &scriptValue)
	{
        if(!scriptValue.isArray())
            return {};

        auto length = scriptValue.property(QStringLiteral("length")).toInt();
        QStringList back;

        back.reserve(length);

        for(int i = 0; i < length; i++)
        {
            back.append(scriptValue.property(i).toString());
        }

		return back;
	}

    std::tuple<bool, QString, int> checkSyntax(const QString &program)
    {
        QString prefix = QStringLiteral("function checkSyntax() { ");
        QJSEngine engine;
        QJSValue result = engine.evaluate(QStringLiteral("%1%2\n }").arg(prefix, program));

        if(!result.isError())
            return {true, {}, {}};

        auto message = result.property(QStringLiteral("message")).toString();
        auto line = result.property(QStringLiteral("lineNumber")).toInt();

        return {false, message, line};
    }
}
