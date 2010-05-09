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

#include "updateclient.h"

#include <QxtRPCPeer>

namespace Tools
{
	UpdateClient::UpdateClient(QObject *parent)
		: QObject(parent),
		mClient(new QxtRPCPeer(this))
	{
		connect(mClient, SIGNAL(serverError(QAbstractSocket::SocketError)), this, SLOT(serverError(QAbstractSocket::SocketError)));
		connect(mClient, SIGNAL(connectedToServer()), this, SLOT(connectedToServer()));
		connect(this, SIGNAL(destroyed()), mClient, SLOT(disconnectServer()));

		mClient->attachSlot("requestError0", this, SLOT(serverRequestError()));

		mClient->attachSlot("banner0", this, SLOT(transmitBanner(QString)));
		mClient->attachSlot("programInfo0", this, SLOT(transmitProgramInfo(Tools::Version, QString, QString, QString, QString, int, QString)));
		mClient->attachSlot("actionPackList0", this, SLOT(transmitActionPackList(Tools::ActionPackInfoList)));
	}

	UpdateClient::~UpdateClient()
	{
	}

	void UpdateClient::start(const QString &ip, int port)
	{
		mClient->connect(ip, port);
	}

	void UpdateClient::end()
	{
		mClient->disconnectServer();
	}

	void UpdateClient::askProgramInfo(const QString &program, const QString &language, const QString &type, const QString &operatingSystem)
	{
		mClient->call("programInfo0", program, language, type, operatingSystem);
	}

	void UpdateClient::askActionPackList(const Tools::Version &version, const QString &language, const QString &operatingSystem)
	{
		mClient->call("actionPackList0", QVariant::fromValue<Tools::Version>(version), language, operatingSystem);
	}

	void UpdateClient::transmitBanner(const QString &message)
	{
		emit gotBanner(message);
	}

	void UpdateClient::transmitProgramInfo(const Tools::Version &version, const QString &releaseDate, const QString &type, const QString &changelog, const QString &filename, int size, const QString &hash)
	{
		emit gotProgramInfo(version, releaseDate, type, changelog, filename, size, hash);
	}

	void UpdateClient::transmitActionPackList(const Tools::ActionPackInfoList &actionPackInfoList)
	{
		emit gotActionPackList(actionPackInfoList);
	}

	void UpdateClient::connectedToServer()
	{
		mClient->call("protocol", PROTOCOL_VERSION);
	}

	void UpdateClient::disconnectedFromServer()
	{
	}

	void UpdateClient::serverError(QAbstractSocket::SocketError socketError)
	{
		Q_UNUSED(socketError)

		emit errorOccured(tr("Unable to reach the update server.\nPlease check your Internet connection."));
	}

	void UpdateClient::serverRequestError()
	{
		emit errorOccured(tr("Server error.\nPlease retry later."));
	}
}
