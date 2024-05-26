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
#include <QUdpSocket>

namespace Code
{
	class Udp : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(QJSValue onConnected READ onConnected WRITE setOnConnected)
		Q_PROPERTY(QJSValue onDisconnected READ onDisconnected WRITE setOnDisconnected)
		Q_PROPERTY(QJSValue onReadyRead READ onReadyRead WRITE setOnReadyRead)
		Q_PROPERTY(QJSValue onError READ onError WRITE setOnError)
		
	public:
		enum OpenMode
		{
			ReadOnly =		QIODevice::ReadOnly,
			WriteOnly =		QIODevice::WriteOnly,
			ReadWrite =		QIODevice::ReadWrite
		};
        Q_ENUM(OpenMode)

        Q_INVOKABLE Udp();
        Q_INVOKABLE Udp(const QJSValue &parameters);
		~Udp() override;
		
		void setOnConnected(const QJSValue &onConnected)			{ mOnConnected = onConnected; }
		void setOnDisconnected(const QJSValue &onDisconnected)		{ mOnDisconnected = onDisconnected; }
		void setOnReadyRead(const QJSValue &onReadyRead)			{ mOnReadyRead = onReadyRead; }
		void setOnError(const QJSValue &onError)					{ mOnError = onError; }

		QJSValue onConnected() const								{ return mOnConnected; }
		QJSValue onDisconnected() const								{ return mOnDisconnected; }
		QJSValue onReadyRead() const								{ return mOnReadyRead; }
		QJSValue onError() const									{ return mOnError; }

        Q_INVOKABLE QString toString() const override                                        { return QStringLiteral("Udp"); }
        Q_INVOKABLE Udp *connect(const QString &hostname, quint16 port, OpenMode openMode = ReadWrite);
        Q_INVOKABLE Udp *waitForConnected(int waitTime = 30000);
        Q_INVOKABLE Udp *waitForReadyRead(int waitTime = 30000);
        Q_INVOKABLE Udp *write(const QJSValue &data);
        Q_INVOKABLE Udp *writeText(const QString &data, Encoding encoding = Native);
        Q_INVOKABLE QJSValue read();
        Q_INVOKABLE QString readText(Encoding encoding = Native);
        Q_INVOKABLE Udp *disconnect();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
		
	private slots:
		void connected();
		void disconnected();
		void readyRead();
		void error(QAbstractSocket::SocketError socketError);
	
	private:
		QUdpSocket *mUdpSocket;
		QJSValue mOnConnected;
		QJSValue mOnDisconnected;
		QJSValue mOnReadyRead;
		QJSValue mOnError;
	};
}

