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

#include <QObject>
#include <QScriptable>
#include <QByteArray>
#include <QScriptValue>

namespace Code
{
	class ACTIONTOOLSSHARED_EXPORT CodeClass : public QObject, public QScriptable
	{
		Q_OBJECT

	public:
		static void throwError(QScriptContext *context, QScriptEngine *engine, const QString &errorType, const QString &message, const QString &parent = QStringLiteral("Error"));

    public slots:
        virtual QString toString() const = 0;
        virtual bool equals(const QScriptValue &other) const = 0;

	protected:
		enum Encoding
		{
			Native,
			Ascii,
			Latin1,
			UTF8
		};

		explicit CodeClass();
        ~CodeClass() override = default;

        template<class T>
        bool defaultEqualsImplementation(const QScriptValue &other) const
        {
            if(other.isUndefined() || other.isNull())
                return false;

            QObject *object = other.toQObject();
            if(T *otherObject = qobject_cast<T*>(object))
                return (otherObject == this);

            return false;
        }

		void throwError(const QString &errorType, const QString &message, const QString &parent = QStringLiteral("Error")) const;

		static QScriptValue constructor(CodeClass *object, QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(CodeClass *object, QScriptEngine *engine);
		static QByteArray toEncoding(const QString &string, Encoding encoding);
		static QString fromEncoding(const QByteArray &byteArray, Encoding encoding);
        static QStringList arrayParameterToStringList(const QScriptValue &scriptValue);

        virtual int additionalMemoryCost() const { return 0; }

	private:
		static QScriptValue emptyFunction(QScriptContext *context, QScriptEngine *engine);
	};
}

