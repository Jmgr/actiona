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
#include <QTcpServer>

namespace Code
{
	class TcpServer : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(QJSValue onNewConnection READ onNewConnection WRITE setOnNewConnection)
		
	public:
        Q_INVOKABLE TcpServer();
        Q_INVOKABLE TcpServer(const QJSValue &parameters);
		~TcpServer() override;
		
		void setOnNewConnection(const QJSValue &onNewConnection)		{ mOnNewConnection = onNewConnection; }
		
		QJSValue onNewConnection() const								{ return mOnNewConnection; }
		
        Q_INVOKABLE QString toString() const override                                            { return QStringLiteral("TcpServer"); }
        Q_INVOKABLE TcpServer *listen(const QString &address = QString(), int port = 0);
        Q_INVOKABLE TcpServer *waitForNewConnection(int waitTime = 30000);
        Q_INVOKABLE QJSValue nextPendingConnection();
        Q_INVOKABLE QString address() const;
        Q_INVOKABLE int port() const;

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private slots:
		void newConnection();
		
	private:
		QTcpServer mTcpServer;
		QJSValue mOnNewConnection;
	};
}

