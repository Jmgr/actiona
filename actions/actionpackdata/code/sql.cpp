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

#include "sql.h"

#include <QScriptValueIterator>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QUuid>

namespace Code
{
	QScriptValue Sql::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		if(context->argumentCount() < 1)
		{
			context->throwError(tr("Please specify the database driver that should be used"));
			return engine->undefinedValue();
		}
		
		Driver driver = static_cast<Driver>(context->argument(0).toInteger());
		return engine->newQObject(new Sql(driver), QScriptEngine::ScriptOwnership);
	}
	
	Sql::Sql(Driver driver)
		: QObject(),
		QScriptable(),
		mDatabase(new QSqlDatabase)
	{
		switch(driver)
		{
		case SQLite2:
			mDriverName = "QSQLITE2";
			break;
		case SQLite:
			mDriverName = "QSQLITE";
			break;
		case PostgreSQL:
			mDriverName = "QPSQL";
			break;
		case MySQL:
			mDriverName = "QMYSQL";
			break;
		case ODBC:
			mDriverName = "QODBC";
			break;
		case InterBase:
			mDriverName = "QIBASE";
			break;
		case OCI:
			mDriverName = "QOCI";
			break;
		case TDS:
			mDriverName = "QTDS";
			break;
		case DB2:
			mDriverName = "QDB2";
			break;
		default:
			break;
		}
	}
	
	Sql::~Sql()
	{
		QString connectionName = mDatabase->connectionName();
		
		mDatabase->close();
		delete mDatabase;
		
		QSqlDatabase::removeDatabase(connectionName);
	}
	
	QScriptValue Sql::connect(const QScriptValue &parameters) const
	{
		mDatabase->close();
		
		if(!QSqlDatabase::isDriverAvailable(mDriverName))
		{
			context()->throwError(tr("The requested database driver is not available"));
			return context()->thisObject();
		}
		
		*mDatabase = QSqlDatabase::addDatabase(mDriverName, QUuid::createUuid().toString());
		if(!mDatabase->isValid())
		{
			context()->throwError(tr("The requested database driver is not available"));
			return context()->thisObject();
		}
		
		QScriptValueIterator it(parameters);
		QString hostName;
		int port = 0;
		QString databaseName;
		QString userName;
		QString password;
		QString options;
	
		while(it.hasNext())
		{
			it.next();
	
			if(it.name() == "hostName")
				hostName = it.value().toString();
			else if(it.name() == "port")
				port = it.value().toInteger();
			else if(it.name() == "databaseName")
				databaseName = it.value().toString();
			else if(it.name() == "userName")
				userName = it.value().toString();
			else if(it.name() == "password")
				password = it.value().toString();
			else if(it.name() == "options")
				options = it.value().toString();
		}
		
		mDatabase->setHostName(hostName);
		if(port != 0)
			mDatabase->setPort(port);
		mDatabase->setDatabaseName(databaseName);
		mDatabase->setConnectOptions(options);
		if(!mDatabase->open(userName, password))
		{
			context()->throwError(tr("Unable to establish a connection to the database"));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}
	
	QScriptValue Sql::prepare(const QString &queryString, const QScriptValue &parameters)
	{
		mQuery = QSqlQuery(*mDatabase);
		mQuery.setForwardOnly(true);
		if(!mQuery.prepare(queryString))
		{
			context()->throwError(tr("Failed to prepare the query"));
			return context()->thisObject();
		}
		
		QScriptValueIterator it(parameters);
		while(it.hasNext())
		{
			it.next();
	
			mQuery.bindValue(it.name(), it.value().toString());
		}
	
		return context()->thisObject();
	}
	
	QScriptValue Sql::execute(const QString &queryString)
	{
		if(!queryString.isEmpty())
		{
			mQuery = QSqlQuery(queryString, *mDatabase);
			mQuery.setForwardOnly(true);
		}
		
		if(!mQuery.exec())
		{
			QSqlError error = mQuery.lastError();
			context()->throwError(tr("Failed to execute the query : %1").arg(error.text()));
			return context()->thisObject();
		}
		
		return context()->thisObject();
	}
	
	QScriptValue Sql::fetchResult(IndexStyle indexStyle)
	{
		if(!mQuery.isSelect())
		{
			context()->throwError(tr("Cannot fetch the result of a non-select query"));
			return context()->thisObject();
		}
		
		int size = mQuery.size();
		if(size == -1)
			size = 0;
		
		QScriptValue back = engine()->newArray(size);
		
		switch(indexStyle)
		{
		case IndexNumber:
			{
				QSqlRecord record = mQuery.record();
				for(int index = 0; mQuery.next(); ++index)
				{
					QScriptValue row = engine()->newArray();
					for(int columnIndex = 0; columnIndex < record.count(); ++columnIndex)
					{
						row.setProperty(columnIndex, engine()->newVariant(mQuery.value(columnIndex)));
					}
					
					back.setProperty(index, row);
				}
			}
			break;
		case IndexName:
			{
				for(int index = 0; mQuery.next(); ++index)
				{
					QSqlRecord record = mQuery.record();
					QScriptValue row = engine()->newArray(record.count());
					for(int columnIndex = 0; columnIndex < record.count(); ++columnIndex)
					{
						row.setProperty(record.fieldName(columnIndex), engine()->newVariant(record.value(columnIndex)));
					}
					
					back.setProperty(index, row);
				}
			}
			break;
		}
		
		return back;
	}
	
	QScriptValue Sql::disconnect() const
	{
		mDatabase->close();
		
		return context()->thisObject();
	}
}
