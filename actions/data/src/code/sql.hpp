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

#pragma once

#include "actiontools/code/codeclass.hpp"

#include <QJSValue>
#include <QSqlQuery>

class QSqlDatabase;

namespace Code
{
	class Sql : public CodeClass
	{
        friend class StaticSql;

		Q_OBJECT

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
        Q_ENUM(Driver)
		enum IndexStyle
		{
			IndexNumber,
			IndexName
		};
        Q_ENUM(IndexStyle)
				
        Q_INVOKABLE Sql(Driver driver);
		~Sql() override;
		
        Q_INVOKABLE QString toString() const override                                { return QStringLiteral("Sql"); }
        Q_INVOKABLE Sql *connect(const QJSValue &parameters);
        Q_INVOKABLE Sql *prepare(const QString &queryString, const QJSValue &parameters);
        Q_INVOKABLE Sql *execute(const QString &queryString = QString());
        Q_INVOKABLE QJSValue fetchResult(IndexStyle indexStyle = IndexName);
        Q_INVOKABLE Sql *disconnect();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
	
	private:
		static QString driverName(Driver driver);

		QString mDriverName;
		QSqlDatabase *mDatabase;
		QSqlQuery mQuery;
	};

    class StaticSql : public CodeClass
    {
        Q_OBJECT

    public:
        StaticSql(QObject *parent): CodeClass(parent) {}

        Q_INVOKABLE QString toString() const override { return QStringLiteral("StaticSql"); }
        Q_INVOKABLE QJSValue drivers();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
    };
}

