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

#ifndef ACT_NO_UPDATER

#ifndef UPDATER_H
#define UPDATER_H

#include "tools_global.h"
#include "version.h"

#include <QObject>
#include <QUrl>

class QNetworkAccessManager;
class QNetworkReply;

namespace Tools
{
	class TOOLSSHARED_EXPORT Updater : public QObject
	{
		Q_OBJECT
		
		static const int Protocol = 0;
		
	public:
		enum FileType
		{
			Installer,
			Source,
			Binary
		};
		
		Updater(QNetworkAccessManager *networkAccessManager, const QUrl &url, QObject *parent = 0);
		~Updater();
		
		void checkForUpdates(const QString &program,
							 const Version &programVersion,
							 FileType fileType,
							 const QString &operatingSystem,
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
		void replyFinished(QNetworkReply *reply);
		
	private:
		QUrl mUrl;
		QNetworkAccessManager *mNetworkAccessManager;
		QNetworkReply *mCurrentReply;

		Q_DISABLE_COPY(Updater)
	};
}

#endif // UPDATER_H

#endif
