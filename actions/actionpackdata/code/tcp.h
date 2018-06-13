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
#include <QTcpSocket>

namespace Code
{
	class Tcp : public CodeClass
	{
		Q_OBJECT
		Q_ENUMS(OpenMode)
		Q_PROPERTY(QScriptValue onConnected READ onConnected WRITE setOnConnected)
		Q_PROPERTY(QScriptValue onDisconnected READ onDisconnected WRITE setOnDisconnected)
		Q_PROPERTY(QScriptValue onReadyRead READ onReadyRead WRITE setOnReadyRead)
		Q_PROPERTY(QScriptValue onBytesWritten READ onBytesWritten WRITE setOnBytesWritten)
		Q_PROPERTY(QScriptValue onError READ onError WRITE setOnError)
		
	public:
		enum OpenMode
		{
			ReadOnly =		QIODevice::ReadOnly,
			WriteOnly =		QIODevice::WriteOnly,
			ReadWrite =		QIODevice::ReadWrite
		};	
				
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue constructor(QTcpSocket *tcpSocket, QScriptEngine *engine);
		
		Tcp();
		Tcp(QTcpSocket *tcpSocket);
		~Tcp() override;
		
		void setOnConnected(const QScriptValue &onConnected)			{ mOnConnected = onConnected; }
		void setOnDisconnected(const QScriptValue &onDisconnected)		{ mOnDisconnected = onDisconnected; }
		void setOnReadyRead(const QScriptValue &onReadyRead)			{ mOnReadyRead = onReadyRead; }
		void setOnBytesWritten(const QScriptValue &onBytesWritten)		{ mOnBytesWritten = onBytesWritten; }
		void setOnError(const QScriptValue &onError)					{ mOnError = onError; }
		
		QScriptValue onConnected() const								{ return mOnConnected; }
		QScriptValue onDisconnected() const								{ return mOnDisconnected; }
		QScriptValue onReadyRead() const								{ return mOnReadyRead; }
		QScriptValue onBytesWritten() const								{ return mOnBytesWritten; }
		QScriptValue onError() const									{ return mOnError; }
		
	public slots:
		QString toString() const override										{ return QStringLiteral("Tcp"); }
        bool equals(const QScriptValue &other) const override            { return defaultEqualsImplementation<Tcp>(other); }
		QScriptValue connect(const QString &hostname, quint16 port, OpenMode openMode = ReadWrite);
		QScriptValue waitForConnected(int waitTime = 30000);
		QScriptValue waitForBytesWritten(int waitTime = 30000);
		QScriptValue waitForReadyRead(int waitTime = 30000);
		QScriptValue waitForDisconnected(int waitTime = 30000);
		QScriptValue write(const QScriptValue &data);
		QScriptValue writeText(const QString &data, Encoding encoding = Native);
		QScriptValue read();
		QString readText(Encoding encoding = Native);
		QScriptValue disconnect();
		
	private slots:
		void connected();
		void disconnected();
		void readyRead();
		void bytesWritten(qint64 bytes);
		void error(QAbstractSocket::SocketError socketError);
	
	private:
		QTcpSocket *mTcpSocket;
		QScriptValue mOnConnected;
		QScriptValue mOnDisconnected;
		QScriptValue mOnReadyRead;
		QScriptValue mOnBytesWritten;
		QScriptValue mOnError;
	};
}

