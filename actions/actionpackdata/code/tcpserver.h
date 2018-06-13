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
#include <QTcpServer>

namespace Code
{
	class TcpServer : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(QScriptValue onNewConnection READ onNewConnection WRITE setOnNewConnection)
		
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
		TcpServer();
		~TcpServer() override;
		
		void setOnNewConnection(const QScriptValue &onNewConnection)		{ mOnNewConnection = onNewConnection; }
		
		QScriptValue onNewConnection() const								{ return mOnNewConnection; }
		
	public slots:
		QString toString() const override                                            { return QStringLiteral("TcpServer"); }
        bool equals(const QScriptValue &other) const override                { return defaultEqualsImplementation<TcpServer>(other); }
		QScriptValue listen(const QString &address = QString(), int port = 0);
		QScriptValue waitForNewConnection(int waitTime = 30000);
		QScriptValue nextPendingConnection();
		QString address() const;
		int port() const;
		
	private slots:
		void newConnection();
		
	private:
		QTcpServer mTcpServer;
		QScriptValue mOnNewConnection;
	};
}

