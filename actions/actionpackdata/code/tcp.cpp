/*
	Actiona
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#include "tcp.h"
#include "code/rawdata.h"

#include <QScriptValueIterator>

namespace Code
{
	QScriptValue Tcp::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Tcp *tcp = new Tcp;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == "onConnected")
				tcp->mOnConnected = it.value();
			else if(it.name() == "onDisconnected")
				tcp->mOnDisconnected = it.value();
			else if(it.name() == "onReadyRead")
				tcp->mOnReadyRead = it.value();
			else if(it.name() == "onBytesWritten")
				tcp->mOnBytesWritten = it.value();
			else if(it.name() == "onError")
				tcp->mOnError = it.value();
		}

		return CodeClass::constructor(tcp, context, engine);
	}
	
	QScriptValue Tcp::constructor(QTcpSocket *tcpSocket, QScriptEngine *engine)
	{
		return CodeClass::constructor(new Tcp(tcpSocket), engine);
	}
	
	Tcp::Tcp()
		: CodeClass(),
		mTcpSocket(new QTcpSocket(this))
	{
		QObject::connect(mTcpSocket, SIGNAL(connected()), this, SLOT(connected()));
		QObject::connect(mTcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
		QObject::connect(mTcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
		QObject::connect(mTcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
		QObject::connect(mTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
	}
	
	Tcp::Tcp(QTcpSocket *tcpSocket)
		: CodeClass(),
		mTcpSocket(tcpSocket)
	{
		QObject::connect(mTcpSocket, SIGNAL(connected()), this, SLOT(connected()));
		QObject::connect(mTcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
		QObject::connect(mTcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
		QObject::connect(mTcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
		QObject::connect(mTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
	}
	
	Tcp::~Tcp()
    {
    }
	
	QScriptValue Tcp::connect(const QString &hostname, quint16 port, OpenMode openMode)
	{
		mTcpSocket->connectToHost(hostname, port, static_cast<QIODevice::OpenMode>(openMode));
		
		return thisObject();
	}
	
	QScriptValue Tcp::waitForConnected(int waitTime)
	{
		if(!mTcpSocket->waitForConnected(waitTime))
			throwError("ConnectionError", tr("Cannot establish a connection to the host"));
		
		return thisObject();
	}
	
	QScriptValue Tcp::waitForBytesWritten(int waitTime)
	{
		if(!mTcpSocket->waitForBytesWritten(waitTime))
			throwError("BytesWrittenError", tr("Waiting for bytes written failed"));
		
		return thisObject();
	}
	
	QScriptValue Tcp::waitForReadyRead(int waitTime)
	{
		if(!mTcpSocket->waitForReadyRead(waitTime))
			throwError("ReadyReadError", tr("Waiting for ready read failed"));
		
		return thisObject();
	}

	QScriptValue Tcp::waitForDisconnected(int waitTime)
	{
		if(!mTcpSocket->waitForDisconnected(waitTime))
			throwError("DisconnectionError", tr("Waiting for disconnection failed"));
		
		return thisObject();
	}
	
	QScriptValue Tcp::write(const QScriptValue &data)
	{
		QObject *object = data.toQObject();
		if(RawData *rawData = qobject_cast<RawData*>(object))
		{
			if(mTcpSocket->write(rawData->byteArray()) == -1)
				throwError("WriteError", tr("Write failed"));
		}
		else
		{
			if(mTcpSocket->write(data.toVariant().toByteArray()) == -1)
				throwError("WriteError", tr("Write failed"));
		}
	
		return thisObject();
	}
	
	QScriptValue Tcp::writeText(const QString &data, Encoding encoding)
	{
		if(mTcpSocket->write(toEncoding(data, encoding)) == -1)
			throwError("WriteError", tr("Write failed"));
		
		return thisObject();
	}
	
	QScriptValue Tcp::read()
	{
		return RawData::constructor(mTcpSocket->readAll(), engine());
	}
	
	QString Tcp::readText(Encoding encoding)
	{
		return fromEncoding(mTcpSocket->readAll(), encoding);
	}
	
	QScriptValue Tcp::disconnect()
	{
		mTcpSocket->disconnectFromHost();
		
		return thisObject();
	}
	
	void Tcp::connected()
	{
		if(mOnConnected.isValid())
			mOnConnected.call(thisObject());
	}

	void Tcp::disconnected()
	{
		if(mOnDisconnected.isValid())
			mOnDisconnected.call(thisObject());
	}

	void Tcp::readyRead()
	{
		if(mOnReadyRead.isValid())
			mOnReadyRead.call(thisObject());
	}

	void Tcp::bytesWritten(qint64 bytes)
	{
		if(mOnBytesWritten.isValid())
			mOnBytesWritten.call(thisObject(), QScriptValueList() << static_cast<qsreal>(bytes));
	}

	void Tcp::error(QAbstractSocket::SocketError socketError)
	{
		Q_UNUSED(socketError)

		if(mOnError.isValid())
			mOnError.call(thisObject(), QScriptValueList() << mTcpSocket->errorString());
	}
}
