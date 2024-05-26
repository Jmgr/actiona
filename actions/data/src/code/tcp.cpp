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

#include "tcp.hpp"
#include "actiontools/code/rawdata.hpp"

#include <QJSValueIterator>

namespace Code
{
	Tcp::Tcp()
		: CodeClass(),
		mTcpSocket(new QTcpSocket(this))
	{
        QObject::connect(mTcpSocket, &QTcpSocket::connected, this, &Tcp::connected);
        QObject::connect(mTcpSocket, &QTcpSocket::disconnected, this, &Tcp::disconnected);
        QObject::connect(mTcpSocket, &QTcpSocket::readyRead, this, &Tcp::readyRead);
        QObject::connect(mTcpSocket, &QTcpSocket::bytesWritten, this, &Tcp::bytesWritten);
        QObject::connect(mTcpSocket, &QTcpSocket::errorOccurred, this, &Tcp::error);
	}

    Tcp::Tcp(const QJSValue &parameters)
        : Tcp()
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

            if(it.name() == QLatin1String("onConnected"))
                mOnConnected = it.value();
            else if(it.name() == QLatin1String("onDisconnected"))
                mOnDisconnected = it.value();
            else if(it.name() == QLatin1String("onReadyRead"))
                mOnReadyRead = it.value();
            else if(it.name() == QLatin1String("onBytesWritten"))
                mOnBytesWritten = it.value();
            else if(it.name() == QLatin1String("onError"))
                mOnError = it.value();
        }
    }
	
	Tcp::Tcp(QTcpSocket *tcpSocket)
		: CodeClass(),
		mTcpSocket(tcpSocket)
	{
        QObject::connect(mTcpSocket, &QTcpSocket::connected, this, &Tcp::connected);
        QObject::connect(mTcpSocket, &QTcpSocket::disconnected, this, &Tcp::disconnected);
        QObject::connect(mTcpSocket, &QTcpSocket::readyRead, this, &Tcp::readyRead);
        QObject::connect(mTcpSocket, &QTcpSocket::bytesWritten, this, &Tcp::bytesWritten);
        QObject::connect(mTcpSocket, &QTcpSocket::errorOccurred, this, &Tcp::error);
    }
	
    Tcp::~Tcp() = default;
	
    Tcp *Tcp::connect(const QString &hostname, quint16 port, OpenMode openMode)
	{
		mTcpSocket->connectToHost(hostname, port, static_cast<QIODevice::OpenMode>(openMode));
		
        return this;
	}
	
    Tcp *Tcp::waitForConnected(int waitTime)
	{
		if(!mTcpSocket->waitForConnected(waitTime))
			throwError(QStringLiteral("ConnectionError"), tr("Cannot establish a connection to the host"));
		
        return this;
	}
	
    Tcp *Tcp::waitForBytesWritten(int waitTime)
	{
		if(!mTcpSocket->waitForBytesWritten(waitTime))
			throwError(QStringLiteral("BytesWrittenError"), tr("Waiting for bytes written failed"));
		
        return this;
	}
	
    Tcp *Tcp::waitForReadyRead(int waitTime)
	{
		if(!mTcpSocket->waitForReadyRead(waitTime))
			throwError(QStringLiteral("ReadyReadError"), tr("Waiting for ready read failed"));
		
        return this;
	}

    Tcp *Tcp::waitForDisconnected(int waitTime)
	{
		if(!mTcpSocket->waitForDisconnected(waitTime))
			throwError(QStringLiteral("DisconnectionError"), tr("Waiting for disconnection failed"));
		
        return this;
	}
	
    Tcp *Tcp::write(const QJSValue &data)
	{
		QObject *object = data.toQObject();
		if(auto rawData = qobject_cast<RawData*>(object))
		{
			if(mTcpSocket->write(rawData->byteArray()) == -1)
				throwError(QStringLiteral("WriteError"), tr("Write failed"));
		}
		else
		{
			if(mTcpSocket->write(data.toVariant().toByteArray()) == -1)
				throwError(QStringLiteral("WriteError"), tr("Write failed"));
		}
	
        return this;
	}
	
    Tcp *Tcp::writeText(const QString &data, Encoding encoding)
	{
		if(mTcpSocket->write(toEncoding(data, encoding)) == -1)
			throwError(QStringLiteral("WriteError"), tr("Write failed"));
		
        return this;
	}
	
	QJSValue Tcp::read()
	{
        return CodeClass::construct<RawData>(mTcpSocket->readAll());
	}
	
	QString Tcp::readText(Encoding encoding)
	{
		return fromEncoding(mTcpSocket->readAll(), encoding);
	}
	
    Tcp *Tcp::disconnect()
	{
		mTcpSocket->disconnectFromHost();
		
        return this;
	}

    void Tcp::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Tcp*>("const Tcp *");

        CodeClass::registerClass<Tcp>(QStringLiteral("Tcp"), scriptEngine);
    }
	
	void Tcp::connected()
	{
        if(!mOnConnected.isUndefined())
            mOnConnected.call();
	}

	void Tcp::disconnected()
	{
        if(!mOnDisconnected.isUndefined())
            mOnDisconnected.call();
	}

	void Tcp::readyRead()
	{
        if(!mOnReadyRead.isUndefined())
            mOnReadyRead.call();
	}

	void Tcp::bytesWritten(qint64 bytes)
	{
        if(!mOnBytesWritten.isUndefined())
            mOnBytesWritten.call(QJSValueList() << static_cast<qreal>(bytes));
	}

	void Tcp::error(QAbstractSocket::SocketError socketError)
	{
		Q_UNUSED(socketError)

        if(!mOnError.isUndefined())
            mOnError.call(QJSValueList() << mTcpSocket->errorString());
	}
}
