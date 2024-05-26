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

#include "tcpserver.hpp"
#include "tcp.hpp"

#include <QJSValueIterator>

namespace Code
{
	TcpServer::TcpServer()
		: CodeClass()
	{
        connect(&mTcpServer, &QTcpServer::newConnection, this, &TcpServer::newConnection);
	}

    TcpServer::TcpServer(const QJSValue &parameters)
        : TcpServer()
    {
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return;
        }

        QJSValueIterator it(parameters);

        while(it.hasNext())
        {
            it.next();

            if(it.name() == QLatin1String("onNewConnection"))
                mOnNewConnection = it.value();
        }
    }
	
	TcpServer::~TcpServer()
	{
        mTcpServer.close();
    }

    TcpServer *TcpServer::listen(const QString &address, int port)
	{
		if(!mTcpServer.listen(QHostAddress(address), port))
			throwError(QStringLiteral("ListenError"), tr("Unable to start listening"));
		
        return this;
	}
	
    TcpServer *TcpServer::waitForNewConnection(int waitTime)
	{
		if(!mTcpServer.waitForNewConnection(waitTime))
			throwError(QStringLiteral("WaitForNewConnectionError"), tr("Waiting for new connection failed"));
		
        return this;
	}
	
	QJSValue TcpServer::nextPendingConnection()
	{
		QTcpSocket *tcpSocket = mTcpServer.nextPendingConnection();
		if(!tcpSocket)
		{
            throwError(QStringLiteral("NoPendingConnectionError"), tr("There is no pending connection"));
            return {};
        }

        return CodeClass::construct<Tcp>(tcpSocket);
	}
	
	QString TcpServer::address() const
	{
		return mTcpServer.serverAddress().toString();
	}
	
	int TcpServer::port() const
	{
		return mTcpServer.serverPort();
	}

    void TcpServer::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<TcpServer*>("const TcpServer *");

        CodeClass::registerClass<TcpServer>(QStringLiteral("TcpServer"), scriptEngine);
    }
	
	void TcpServer::newConnection()
	{
        if(!mOnNewConnection.isUndefined())
            mOnNewConnection.call();
	}
}
