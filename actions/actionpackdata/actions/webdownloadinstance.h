/*
	Actionaz
	Copyright (C) 2008-2013 Jonathan Mercier-Ganady

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

#ifndef WEBDOWNLOADINSTANCE_H
#define WEBDOWNLOADINSTANCE_H

#include "actioninstance.h"
#include "script.h"
#include "stringlistpair.h"

#include <QFile>

class QNetworkAccessManager;
class QNetworkReply;
class QProgressDialog;

namespace Actions
{
	class WebDownloadInstance : public ActionTools::ActionInstance
	{
		Q_OBJECT
		Q_ENUMS(Destination)

	public:
		enum Destination
		{
			Variable,
			File
		};
		enum Exceptions
		{
			CannotOpenFileException = ActionTools::ActionException::UserException,
			DownloadException
		};

		WebDownloadInstance(const ActionTools::ActionDefinition *definition, QObject *parent = 0);
		~WebDownloadInstance();

		static ActionTools::StringListPair destinations;

		void startExecution();
		void stopExecution();

	private slots:
		void finished();
		void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
		void readyRead();
		void canceled();

	private:
		Q_DISABLE_COPY(WebDownloadInstance)

		QNetworkAccessManager *mNetworkAccessManager;
		QNetworkReply *mReply;
		QString mVariable;
		Destination mDestination;
		QFile mFile;
		QProgressDialog *mProgressDialog;
	};
}

#endif // WEBDOWNLOADINSTANCE_H
