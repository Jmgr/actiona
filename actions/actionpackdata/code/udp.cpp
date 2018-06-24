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

#include "udp.h"
#include "code/rawdata.h"

#include <QScriptValueIterator>

namespace Code
{
	QScriptValue Udp::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Udp *udp = new Udp;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();
			
			if(it.name() == QLatin1String("onConnected"))
				udp->mOnConnected = it.value();
			else if(it.name() == QLatin1String("onDisconnected"))
				udp->mOnDisconnected = it.value();
			else if(it.name() == QLatin1String("onReadyRead"))
				udp->mOnReadyRead = it.value();
			else if(it.name() == QLatin1String("onError"))
				udp->mOnError = it.value();
		}

		return CodeClass::constructor(udp, context, engine);
	}
	
	Udp::Udp()
		: CodeClass(),
		mUdpSocket(new QUdpSocket(this))
	{
        QObject::connect(mUdpSocket, &QUdpSocket::connected, this, &Udp::connected);
        QObject::connect(mUdpSocket, &QUdpSocket::disconnected, this, &Udp::disconnected);
        QObject::connect(mUdpSocket, &QUdpSocket::readyRead, this, &Udp::readyRead);
        QObject::connect(mUdpSocket, static_cast<void (QUdpSocket::*)(QAbstractSocket::SocketError)>(&QUdpSocket::error), this, &Udp::error);
	}
	
    Udp::~Udp() = default;
	
	QScriptValue Udp::connect(const QString &hostname, quint16 port, OpenMode openMode)
	{
		mUdpSocket->connectToHost(hostname, port, static_cast<QIODevice::OpenMode>(openMode));
		
		return thisObject();
	}
	
	QScriptValue Udp::waitForConnected(int waitTime)
	{
		if(!mUdpSocket->waitForConnected(waitTime))
			throwError(QStringLiteral("ConnectionError"), tr("Cannot establish a connection to the host"));
		
		return thisObject();
	}
	
	QScriptValue Udp::waitForReadyRead(int waitTime)
	{
		if(!mUdpSocket->waitForReadyRead(waitTime))
			throwError(QStringLiteral("ReadyReadError"), tr("Waiting for ready read failed"));
		
		return thisObject();
	}
	
	QScriptValue Udp::write(const QScriptValue &data)
	{
		QObject *object = data.toQObject();
		if(auto rawData = qobject_cast<RawData*>(object))
		{
			if(mUdpSocket->write(rawData->byteArray()) == -1)
				throwError(QStringLiteral("WriteError"), tr("Write failed"));
		}
		else
		{
			if(mUdpSocket->write(data.toVariant().toByteArray()) == -1)
				throwError(QStringLiteral("WriteError"), tr("Write failed"));
		}
	
		return thisObject();
	}
	
	QScriptValue Udp::writeText(const QString &data, Encoding encoding)
	{
		if(mUdpSocket->write(toEncoding(data, encoding)) == -1)
			throwError(QStringLiteral("WriteError"), tr("Write failed"));
		
		return thisObject();
	}
	
	QScriptValue Udp::read()
	{
		return RawData::constructor(mUdpSocket->readAll(), engine());
	}
	
	QString Udp::readText(Encoding encoding)
	{
		return fromEncoding(mUdpSocket->readAll(), encoding);
	}
	
	QScriptValue Udp::disconnect()
	{
		mUdpSocket->disconnectFromHost();
		
		return thisObject();
	}
	
	void Udp::connected()
	{
		if(mOnConnected.isValid())
			mOnConnected.call(thisObject());
	}

	void Udp::disconnected()
	{
		if(mOnDisconnected.isValid())
			mOnDisconnected.call(thisObject());
	}

	void Udp::readyRead()
	{
		if(mOnReadyRead.isValid())
			mOnReadyRead.call(thisObject());
	}

	void Udp::error(QAbstractSocket::SocketError socketError)
	{
		Q_UNUSED(socketError)

		if(mOnError.isValid())
			mOnError.call(thisObject(), QScriptValueList() << mUdpSocket->errorString());
	}
}
