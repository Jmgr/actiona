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

#include "actiontools/actioninstance.hpp"
#include "actiontools/script.hpp"
#include "tools/stringlistpair.hpp"

#include <QFile>

class QNetworkAccessManager;
class QNetworkReply;
class QProgressDialog;

namespace Actions
{
	class WebDownloadInstance : public ActionTools::ActionInstance
	{
        Q_OBJECT

	public:
		enum Destination
		{
			Variable,
			File
		};
        Q_ENUM(Destination)
		enum Exceptions
		{
			CannotOpenFileException = ActionTools::ActionException::UserException,
			DownloadException
		};

		WebDownloadInstance(const ActionTools::ActionDefinition *definition, QObject *parent = nullptr);
		~WebDownloadInstance() override;

        static Tools::StringListPair destinations;

		void startExecution() override;
		void stopExecution() override;

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

