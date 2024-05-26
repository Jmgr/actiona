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

#include "sql.hpp"
#include "actiontools/scriptengine.hpp"

#include <QJSValueIterator>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QUuid>
#include <QStringList>
#include <QSet>

namespace Code
{
	Sql::Sql(Driver driver)
		: CodeClass(),
		mDatabase(new QSqlDatabase)
	{
		mDriverName = driverName(driver);
	}

	Sql::~Sql()
	{
		QString connectionName = mDatabase->connectionName();

		mDatabase->close();
		delete mDatabase;

        QSqlDatabase::removeDatabase(connectionName);
    }

    Sql *Sql::connect(const QJSValue &parameters)
	{
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return this;
        }

		mDatabase->close();

		if(!QSqlDatabase::isDriverAvailable(mDriverName))
		{
			throwError(QStringLiteral("DatabaseDriverUnavailableError"), tr("The requested database driver is not available"));
            return this;
		}

		*mDatabase = QSqlDatabase::addDatabase(mDriverName, QUuid::createUuid().toString());
		if(!mDatabase->isValid())
		{
			throwError(QStringLiteral("DatabaseDriverUnavailableError"), tr("The requested database driver is not available"));
            return this;
		}

		QJSValueIterator it(parameters);
		QString hostName;
		int port = 0;
		QString databaseName;
		QString userName;
		QString password;
		QString options;

		while(it.hasNext())
		{
			it.next();

			if(it.name() == QLatin1String("hostName"))
				hostName = it.value().toString();
			else if(it.name() == QLatin1String("port"))
                port = it.value().toInt();
			else if(it.name() == QLatin1String("databaseName"))
				databaseName = it.value().toString();
			else if(it.name() == QLatin1String("userName"))
				userName = it.value().toString();
			else if(it.name() == QLatin1String("password"))
				password = it.value().toString();
			else if(it.name() == QLatin1String("options"))
				options = it.value().toString();
		}

		mDatabase->setHostName(hostName);
		if(port != 0)
			mDatabase->setPort(port);
		mDatabase->setDatabaseName(databaseName);
		mDatabase->setConnectOptions(options);
		if(!mDatabase->open(userName, password))
		{
			throwError(QStringLiteral("ConnectionError"), tr("Unable to establish a connection to the database"));
            return this;
		}

        return this;
	}

    Sql *Sql::prepare(const QString &queryString, const QJSValue &parameters)
	{
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return this;
        }

		mQuery = QSqlQuery(*mDatabase);
		mQuery.setForwardOnly(true);
		if(!mQuery.prepare(queryString))
		{
			throwError(QStringLiteral("PrepareQueryError"), tr("Failed to prepare the query"));
            return this;
		}

		QJSValueIterator it(parameters);
		while(it.hasNext())
		{
			it.next();

			mQuery.bindValue(it.name(), it.value().toString());
		}

        return this;
	}

    Sql *Sql::execute(const QString &queryString)
	{
        bool result = false;

        if(queryString.isEmpty())
		{
            result = mQuery.exec();
		}
        else
        {
            mQuery = QSqlQuery(*mDatabase);
            mQuery.setForwardOnly(true);

            result = mQuery.exec(queryString);
        }

        if(!result)
		{
			QSqlError error = mQuery.lastError();
			throwError(QStringLiteral("ExecuteQueryError"), tr("Failed to execute the query : %1").arg(error.text()));
            return this;
		}

        return this;
	}

	QJSValue Sql::fetchResult(IndexStyle indexStyle)
	{
		if(!mQuery.isSelect())
		{
			throwError(QStringLiteral("FetchError"), tr("Cannot fetch the result of a non-select query"));
            return {};
		}

		int size = mQuery.size();
		if(size == -1)
			size = 0;

        QJSValue back = ActionTools::ScriptEngine::current()->newArray(size);

		switch(indexStyle)
		{
		case IndexNumber:
			{
				QSqlRecord record = mQuery.record();
				for(int index = 0; mQuery.next(); ++index)
				{
                    QJSValue row = ActionTools::ScriptEngine::current()->newArray();
					for(int columnIndex = 0; columnIndex < record.count(); ++columnIndex)
					{
                        row.setProperty(columnIndex, ActionTools::ScriptEngine::current()->toScriptValue(mQuery.value(columnIndex)));
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
                    QJSValue row = ActionTools::ScriptEngine::current()->newArray(record.count());
					for(int columnIndex = 0; columnIndex < record.count(); ++columnIndex)
					{
                        row.setProperty(record.fieldName(columnIndex), ActionTools::ScriptEngine::current()->toScriptValue(record.value(columnIndex)));
					}

					back.setProperty(index, row);
				}
			}
			break;
		}

		return back;
	}

    Sql *Sql::disconnect()
	{
		mDatabase->close();

        return this;
	}

    void Sql::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Sql*>("const Sql *");

        CodeClass::registerClassWithStaticFunctions<Sql, StaticSql>(QStringLiteral("Sql"),  {QStringLiteral("drivers")}, scriptEngine);
    }

	QString Sql::driverName(Driver driver)
	{
		switch(driver)
		{
		case SQLite2:
			return QStringLiteral("QSQLITE2");
		case SQLite:
			return QStringLiteral("QSQLITE");
		case PostgreSQL:
			return QStringLiteral("QPSQL");
		case MySQL:
			return QStringLiteral("QMYSQL");
		case ODBC:
			return QStringLiteral("QODBC");
		case InterBase:
			return QStringLiteral("QIBASE");
		case OCI:
			return QStringLiteral("QOCI");
		case TDS:
			return QStringLiteral("QTDS");
		case DB2:
			return QStringLiteral("QDB2");
		default:
			return QString();
		}
	}

    QJSValue StaticSql::drivers()
    {
        QStringList driverNames = QSqlDatabase::drivers();
        QSet<Sql::Driver> driverList;

        for(int index = 0; index < driverNames.size(); ++index)
        {
            const QString &driverNameToInclude = driverNames.at(index);

            for(int driverIndex = 0; driverIndex < Sql::DriverCount; ++driverIndex)
            {
                    if(Sql::driverName(static_cast<Sql::Driver>(driverIndex)) == driverNameToInclude)
                        driverList.insert(static_cast<Sql::Driver>(driverIndex));
            }
        }

        QJSValue back = ActionTools::ScriptEngine::current()->newArray(driverList.size());
        int index = 0;
        for(const Sql::Driver &driver: driverList)
        {
            back.setProperty(index, driver);

            ++index;
        }

        return back;
    }
}
