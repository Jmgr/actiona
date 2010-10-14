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

#ifndef TCP_H
#define TCP_H

#include "code/code.h"

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>

class QTcpSocket;

namespace Code
{
	class Tcp : public QObject, public QScriptable
	{
		Q_OBJECT
		Q_CLASSINFO("type", "CodeClass")
		Q_ENUMS(OpenMode)
		Q_PROPERTY(QScriptValue onConnected READ onConnected WRITE setOnConnected)
		Q_PROPERTY(QScriptValue onDisconnected READ onDisconnected WRITE setOnDisconnected)
		Q_PROPERTY(QScriptValue onReadyRead READ onReadyRead WRITE setOnReadyRead)
		Q_PROPERTY(QScriptValue onBytesWritten READ onBytesWritten WRITE setOnBytesWritten)
		
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
		~Tcp();
		
		void setOnConnected(const QScriptValue &onConnected)			{ mOnConnected = onConnected; }
		void setOnDisconnected(const QScriptValue &onDisconnected)		{ mOnDisconnected = onDisconnected; }
		void setOnReadyRead(const QScriptValue &onReadyRead)			{ mOnReadyRead = onReadyRead; }
		void setOnBytesWritten(const QScriptValue &onBytesWritten)		{ mOnBytesWritten = onBytesWritten; }
		
		QScriptValue onConnected() const								{ return mOnConnected; }
		QScriptValue onDisconnected() const								{ return mOnDisconnected; }
		QScriptValue onReadyRead() const								{ return mOnReadyRead; }
		QScriptValue onBytesWritten() const								{ return mOnBytesWritten; }
		
	public slots:
		QString toString() const										{ return "Tcp"; }
		QScriptValue connect(const QString &hostname, quint16 port, OpenMode openMode = ReadWrite);
		QScriptValue waitForConnected(int waitTime = 30000);
		QScriptValue waitForBytesWritten(int waitTime = 30000);
		QScriptValue waitForReadyRead(int waitTime = 30000);
		QScriptValue waitForDisconnected(int waitTime = 30000);
		QScriptValue write(const QScriptValue &data);
		QScriptValue writeText(const QString &data, Code::Encoding encoding = Code::Native);
		QScriptValue read();
		QString readText(Code::Encoding encoding = Code::Native);
		QScriptValue disconnect();
		
	private slots:
		void connected();
		void disconnected();
		void readyRead();
		void bytesWritten(qint64 bytes);
	
	private:
		QTcpSocket *mTcpSocket;
		QScriptValue mOnConnected;
		QScriptValue mOnDisconnected;
		QScriptValue mOnReadyRead;
		QScriptValue mOnBytesWritten;
		QScriptValue mThisObject;
	};
}

#endif // TCP_H
