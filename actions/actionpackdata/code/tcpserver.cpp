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

#include "tcpserver.h"
#include "tcp.h"

#include <QScriptValueIterator>

namespace Code
{
	QScriptValue TcpServer::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		TcpServer *tcpServer = new TcpServer;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == "onNewConnection")
				tcpServer->mOnNewConnection = it.value();
		}

		return tcpServer->mThisObject = engine->newQObject(tcpServer, QScriptEngine::ScriptOwnership);
	}
	
	TcpServer::TcpServer()
		: QObject(),
		QScriptable()
	{
		connect(&mTcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
	}
	
	TcpServer::~TcpServer()
	{
		mTcpServer.close();
	}
	
	QScriptValue TcpServer::listen(const QString &address, int port)
	{
		if(!mTcpServer.listen(QHostAddress(address), port))
			context()->throwError(tr("Unable to start listening"));
		
		return context()->thisObject();
	}
	
	QScriptValue TcpServer::waitForNewConnection(int waitTime)
	{
		if(!mTcpServer.waitForNewConnection(waitTime))
			context()->throwError(tr("Waiting for bytes written failed"));
		
		return context()->thisObject();
	}
	
	QScriptValue TcpServer::nextPendingConnection()
	{
		QTcpSocket *tcpSocket = mTcpServer.nextPendingConnection();
		if(!tcpSocket)
		{
			context()->throwError(tr("There is no pending connection"));
			return engine()->undefinedValue();
		}
		
		return Tcp::constructor(tcpSocket, engine());
	}
	
	QString TcpServer::address() const
	{
		return mTcpServer.serverAddress().toString();
	}
	
	int TcpServer::port() const
	{
		return mTcpServer.serverPort();
	}
	
	void TcpServer::newConnection()
	{
		if(mOnNewConnection.isValid())
			mOnNewConnection.call(mThisObject);
	}
}
