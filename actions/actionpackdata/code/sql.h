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

#ifndef SQL_H
#define SQL_H

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>
#include <QSqlQuery>

class QSqlDatabase;

namespace Code
{
	class Sql : public QObject, public QScriptable
	{
		Q_OBJECT
		Q_CLASSINFO("type", "CodeClass")
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
			DB2
		};
		enum IndexStyle
		{
			IndexNumber,
			IndexName
		};
		
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
		Sql(Driver driver);
		~Sql();
		
	public slots:
		QString toString() const					{ return "Sql"; }
		QScriptValue connect(const QScriptValue &parameters) const;
		QScriptValue prepare(const QString &queryString, const QScriptValue &parameters);
		QScriptValue execute(const QString &queryString = QString());
		QScriptValue fetchResult(IndexStyle indexStyle = IndexName);
		QScriptValue disconnect() const;
	
	private:
		QString mDriverName;
		QSqlDatabase *mDatabase;
		QSqlQuery mQuery;
	};
}

#endif // SQL_H
