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

#include "actiontools/code/prettyprinting.hpp"

#include <QString>
#include <QJSValue>
#include <QJSValueIterator>
#include <QJSEngine>

namespace Code
{
    void prettyPrintScriptValue(QString &result, std::size_t tabCount, const QJSValue &value, bool quoteString);

    void prettyPrintArrayOrObject(QString &result, std::size_t tabCount, const QJSValue &value)
    {
        bool isArray = value.isArray();

        result += isArray ? QStringLiteral("[\n") : QStringLiteral("{\n");

        ++tabCount;

        QJSValueIterator it(value);
        bool first{true};
        while(it.hasNext())
        {
            it.next();

            if(first)
                first = false;
            else
                result += QStringLiteral(",\n");

            for(std::size_t tabIndex{}; tabIndex < tabCount; ++tabIndex)
                result += QStringLiteral("    ");

            if(!isArray)
                result += it.name() + QStringLiteral(": ");

            prettyPrintScriptValue(result, tabCount, it.value(), true);
        }

        result += QStringLiteral("\n");

        --tabCount;

        for(std::size_t tabIndex{}; tabIndex < tabCount; ++tabIndex)
            result += QStringLiteral("    ");

        result += isArray ? QStringLiteral("]") : QStringLiteral("}");
    }

    void prettyPrintScriptValue(QString &result, std::size_t tabCount, const QJSValue &value, bool quoteString)
    {
        if(value.isQObject())
            result += value.toString();
        else if(value.isArray() || value.isObject())
            prettyPrintArrayOrObject(result, tabCount, value);
        else if(value.isString() && quoteString)
            result += QStringLiteral("\"") + value.toString() + QStringLiteral("\"");
        else
            result += value.toString();
    }

    QJSValue toStringFunction(QScriptContext *context, QJSEngine *engine)
    {
        Q_UNUSED(engine)

        QString result;

        prettyPrintScriptValue(result, 0, context->thisObject(), false);

        return result;
    }
}
