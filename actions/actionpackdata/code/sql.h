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

#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QSqlQuery>

class QSqlDatabase;

namespace Code
{
	class Sql : public CodeClass
	{
		Q_OBJECT
		Q_ENUMS(Driver)
		Q_ENUMS(IndexStyle)
		
	public:
		enum Driver
		{
			SQLite2,
			SQLite,
			PostgreSQL,
			MySQL,
			ODBC,
			InterBase,
			OCI,
			TDS,
			DB2,

			DriverCount
		};
		enum IndexStyle
		{
			IndexNumber,
			IndexName
		};
		
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue drivers(QScriptContext *context, QScriptEngine *engine);

		static void registerClass(QScriptEngine *scriptEngine);
		
		Sql(Driver driver);
		~Sql() override;
		
	public slots:
		QString toString() const override                                { return QStringLiteral("Sql"); }
        bool equals(const QScriptValue &other) const override    { return defaultEqualsImplementation<Sql>(other); }
		QScriptValue connect(const QScriptValue &parameters) const;
		QScriptValue prepare(const QString &queryString, const QScriptValue &parameters);
		QScriptValue execute(const QString &queryString = QString());
		QScriptValue fetchResult(IndexStyle indexStyle = IndexName);
		QScriptValue disconnect() const;
	
	private:
		static QString driverName(Driver driver);

		QString mDriverName;
		QSqlDatabase *mDatabase;
		QSqlQuery mQuery;
	};
}

