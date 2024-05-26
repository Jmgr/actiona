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

#include "udp.hpp"
#include "actiontools/code/rawdata.hpp"

#include <QJSValueIterator>

namespace Code
{
	Udp::Udp()
		: CodeClass(),
		mUdpSocket(new QUdpSocket(this))
	{
        QObject::connect(mUdpSocket, &QUdpSocket::connected, this, &Udp::connected);
        QObject::connect(mUdpSocket, &QUdpSocket::disconnected, this, &Udp::disconnected);
        QObject::connect(mUdpSocket, &QUdpSocket::readyRead, this, &Udp::readyRead);
        QObject::connect(mUdpSocket, &QUdpSocket::errorOccurred, this, &Udp::error);
	}

    Udp::Udp(const QJSValue &parameters)
        : Udp()
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
            else if(it.name() == QLatin1String("onError"))
                mOnError = it.value();
        }
    }
	
    Udp::~Udp() = default;
	
    Udp *Udp::connect(const QString &hostname, quint16 port, OpenMode openMode)
	{
		mUdpSocket->connectToHost(hostname, port, static_cast<QIODevice::OpenMode>(openMode));
		
        return this;
	}
	
    Udp *Udp::waitForConnected(int waitTime)
	{
		if(!mUdpSocket->waitForConnected(waitTime))
			throwError(QStringLiteral("ConnectionError"), tr("Cannot establish a connection to the host"));
		
        return this;
	}
	
    Udp *Udp::waitForReadyRead(int waitTime)
	{
		if(!mUdpSocket->waitForReadyRead(waitTime))
			throwError(QStringLiteral("ReadyReadError"), tr("Waiting for ready read failed"));
		
        return this;
	}
	
    Udp *Udp::write(const QJSValue &data)
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
	
        return this;
	}
	
    Udp *Udp::writeText(const QString &data, Encoding encoding)
	{
		if(mUdpSocket->write(toEncoding(data, encoding)) == -1)
			throwError(QStringLiteral("WriteError"), tr("Write failed"));
		
        return this;
	}
	
	QJSValue Udp::read()
    {
        return CodeClass::construct<RawData>(mUdpSocket->readAll());
	}
	
	QString Udp::readText(Encoding encoding)
	{
		return fromEncoding(mUdpSocket->readAll(), encoding);
	}
	
    Udp *Udp::disconnect()
	{
		mUdpSocket->disconnectFromHost();
		
        return this;
	}
	
	void Udp::connected()
	{
        if(!mOnConnected.isUndefined())
            mOnConnected.call();
	}

	void Udp::disconnected()
	{
        if(!mOnDisconnected.isUndefined())
            mOnDisconnected.call();
	}

    void Udp::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Udp*>("const Udp *");

        CodeClass::registerClass<Udp>(QStringLiteral("Udp"), scriptEngine);
    }

	void Udp::readyRead()
	{
        if(!mOnReadyRead.isUndefined())
            mOnReadyRead.call();
	}

	void Udp::error(QAbstractSocket::SocketError socketError)
	{
		Q_UNUSED(socketError)

        if(!mOnError.isUndefined())
            mOnError.call(QJSValueList() << mUdpSocket->errorString());
	}
}
