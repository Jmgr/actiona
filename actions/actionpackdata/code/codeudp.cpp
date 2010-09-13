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

#include "codeudp.h"
#include "coderawdata.h"

QScriptValue CodeUdp::constructor(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(context)
	
	return engine->newQObject(new CodeUdp, QScriptEngine::ScriptOwnership);
}

CodeUdp::~CodeUdp()
{
	mUdpSocket.disconnectFromHost();
}

QScriptValue CodeUdp::connect(const QString &hostname, quint16 port, OpenMode openMode)
{
	mUdpSocket.connectToHost(hostname, port, static_cast<QIODevice::OpenMode>(openMode));
	
	return context()->thisObject();
}

QScriptValue CodeUdp::waitForConnected(int waitTime)
{
	if(!mUdpSocket.waitForConnected(waitTime))
		context()->throwError(tr("Cannot establish a connection to the host"));
	
	return context()->thisObject();
}

QScriptValue CodeUdp::waitForReadyRead(int waitTime)
{
	if(!mUdpSocket.waitForReadyRead(waitTime))
		context()->throwError(tr("Waiting for ready read failed"));
	
	return context()->thisObject();
}

QScriptValue CodeUdp::write(const QScriptValue &data)
{
	QObject *object = data.toQObject();
	if(CodeRawData *codeRawData = qobject_cast<CodeRawData*>(object))
	{
		if(mUdpSocket.write(codeRawData->byteArray()) == -1)
			context()->throwError(tr("Write failed"));
	}
	else
	{
		if(mUdpSocket.write(data.toVariant().toByteArray()) == -1)
			context()->throwError(tr("Write failed"));
	}

	return context()->thisObject();
}

QScriptValue CodeUdp::writeText(const QString &data, Code::Encoding encoding)
{
	if(mUdpSocket.write(Code::toEncoding(data, encoding)) == -1)
		context()->throwError(tr("Write failed"));
	
	return context()->thisObject();
}

QScriptValue CodeUdp::read()
{
	return CodeRawData::constructor(mUdpSocket.readAll(), context(), engine());
}

QString CodeUdp::readText(Code::Encoding encoding)
{
	return Code::fromEncoding(mUdpSocket.readAll(), encoding);
}

QScriptValue CodeUdp::disconnect()
{
	mUdpSocket.disconnectFromHost();
	
	return context()->thisObject();
}
