/*
	Actiona
	Copyright (C) 2005-2016 Jonathan Mercier-Ganady

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

#ifndef UPDATER_H
#define UPDATER_H

#include "tools_global.h"
#include "version.h"

#include <QObject>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;
class QTimer;

namespace Tools
{
	class TOOLSSHARED_EXPORT Updater : public QObject
	{
		Q_OBJECT
		
		static const int Protocol = 1;
		
	public:
		enum FileType
		{
			Binary,
			Source
		};
		enum ContainerType
		{
			Installer,
			SevenZip,
			Zip,
			TarGz,
			TarBz2,
			Deb,
			Rpm
		};
		
		Updater(QNetworkAccessManager *networkAccessManager, const QUrl &url, int timeout, QObject *parent = 0);
		~Updater();
		
		void checkForUpdates(const QString &program,
							 const Version &programVersion,
							 FileType fileType,
							 ContainerType containerType,
							 const QString &operatingSystem,
							 int operatingSystemBits,
							 const QString &language);
		void cancel();
	
	signals:
		void error(const QString &message);
		void noResult();
		void success(const Tools::Version &version,
					 const QDate &releaseDate,
					 const QString &type,
					 const QString &changelog,
					 const QString &filename,
					 int size,
					 const QString &hash);
	
	private slots:
		void replyFinished();
		void timeout();
		void downloadProgress();
		
	private:
		QUrl mUrl;
		QNetworkAccessManager *mNetworkAccessManager;
		QNetworkReply *mCurrentReply;
		QTimer *mTimeoutTimer;

		Q_DISABLE_COPY(Updater)
	};
}

#endif // UPDATER_H
