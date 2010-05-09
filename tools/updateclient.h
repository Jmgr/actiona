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

#ifndef UPDATECLIENT_H
#define UPDATECLIENT_H

#include "tools_global.h"
#include "actioninfo.h"
#include "version.h"

#include <QObject>
#include <QAbstractSocket>

class QxtRPCPeer;

namespace Tools
{
	class TOOLSSHARED_EXPORT UpdateClient : public QObject
	{
		Q_OBJECT

	public:
		static const int PROTOCOL_VERSION = 0;

		explicit UpdateClient(QObject *parent = 0);
		~UpdateClient();

		void start(const QString &ip, int port);
		void end();

		//Requests
		void askProgramInfo(const QString &program, const QString &language, const QString &type, const QString &operatingSystem);
		void askActionPackList(const Tools::Version &version, const QString &language, const QString &operatingSystem);

	signals:
		void errorOccured(const QString &message);
		void gotBanner(const QString &message);
		void gotProgramInfo(const Tools::Version &version, const QString &releaseDate, const QString &type, const QString &changelog, const QString &filename, int size, const QString &hash);
		void gotActionPackList(const Tools::ActionPackInfoList &actionPackInfoList);

	private slots:
		void transmitBanner(const QString &message);
		void transmitProgramInfo(const Tools::Version &version, const QString &releaseDate, const QString &type, const QString &changelog, const QString &filename, int size, const QString &hash);
		void transmitActionPackList(const Tools::ActionPackInfoList &actionPackInfoList);
		void connectedToServer();
		void disconnectedFromServer();

		void serverError(QAbstractSocket::SocketError socketError);
		void serverRequestError();

	private:
		QxtRPCPeer *mClient;
	};
}

#endif // UPDATECLIENT_H
