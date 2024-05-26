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
#include <QTcpSocket>

namespace Code
{
	class Tcp : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(QJSValue onConnected READ onConnected WRITE setOnConnected)
		Q_PROPERTY(QJSValue onDisconnected READ onDisconnected WRITE setOnDisconnected)
		Q_PROPERTY(QJSValue onReadyRead READ onReadyRead WRITE setOnReadyRead)
		Q_PROPERTY(QJSValue onBytesWritten READ onBytesWritten WRITE setOnBytesWritten)
		Q_PROPERTY(QJSValue onError READ onError WRITE setOnError)
		
	public:
		enum OpenMode
		{
			ReadOnly =		QIODevice::ReadOnly,
			WriteOnly =		QIODevice::WriteOnly,
			ReadWrite =		QIODevice::ReadWrite
		};	
        Q_ENUM(OpenMode)

        Q_INVOKABLE Tcp();
        Q_INVOKABLE Tcp(const QJSValue &parameters);
		Tcp(QTcpSocket *tcpSocket);
		~Tcp() override;
		
		void setOnConnected(const QJSValue &onConnected)			{ mOnConnected = onConnected; }
		void setOnDisconnected(const QJSValue &onDisconnected)		{ mOnDisconnected = onDisconnected; }
		void setOnReadyRead(const QJSValue &onReadyRead)			{ mOnReadyRead = onReadyRead; }
		void setOnBytesWritten(const QJSValue &onBytesWritten)		{ mOnBytesWritten = onBytesWritten; }
		void setOnError(const QJSValue &onError)					{ mOnError = onError; }
		
		QJSValue onConnected() const								{ return mOnConnected; }
		QJSValue onDisconnected() const								{ return mOnDisconnected; }
		QJSValue onReadyRead() const								{ return mOnReadyRead; }
		QJSValue onBytesWritten() const								{ return mOnBytesWritten; }
		QJSValue onError() const									{ return mOnError; }
		
        Q_INVOKABLE QString toString() const override										{ return QStringLiteral("Tcp"); }
        Q_INVOKABLE Tcp *connect(const QString &hostname, quint16 port, OpenMode openMode = ReadWrite);
        Q_INVOKABLE Tcp *waitForConnected(int waitTime = 30000);
        Q_INVOKABLE Tcp *waitForBytesWritten(int waitTime = 30000);
        Q_INVOKABLE Tcp *waitForReadyRead(int waitTime = 30000);
        Q_INVOKABLE Tcp *waitForDisconnected(int waitTime = 30000);
        Q_INVOKABLE Tcp *write(const QJSValue &data);
        Q_INVOKABLE Tcp *writeText(const QString &data, Encoding encoding = Native);
        Q_INVOKABLE QJSValue read();
        Q_INVOKABLE QString readText(Encoding encoding = Native);
        Q_INVOKABLE Tcp *disconnect();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
		
	private slots:
		void connected();
		void disconnected();
		void readyRead();
		void bytesWritten(qint64 bytes);
		void error(QAbstractSocket::SocketError socketError);
	
	private:
		QTcpSocket *mTcpSocket;
		QJSValue mOnConnected;
		QJSValue mOnDisconnected;
		QJSValue mOnReadyRead;
		QJSValue mOnBytesWritten;
		QJSValue mOnError;
	};
}

