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

#include "version.h"
#include "updateclient.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QCryptographicHash>

class QProgressDialog;
class QNetworkAccessManager;

namespace Tools
{
	class UpdateClient;
}

class Updater : public QObject
{
	Q_OBJECT

public:
	Updater(QWidget *parent);
	~Updater();

	void checkNewProgramVersion(bool showMessageIfNoNewVersion);

private slots:
	void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void downloadError(QNetworkReply::NetworkError code);
	void downloadFinished();
	void downloadDataAvailable();
	void postDownloadOperation();
	void stop();
	void errorOccured(const QString &message);
	void gotBanner(const QString &message);
	void gotProgramInfo(const Tools::Version &version,
						const QString &releaseDate,
						const QString &type,
						const QString &changelog,
						const QString &filename,
						int size,
						const QString &hash);

private:
	QString setupChangelog(const QString &changelog);

	Tools::UpdateClient mUpdateClient;
	QProgressDialog *mProgressDialog;
	QWidget *mParentWidget;
	QNetworkAccessManager mNetworkAccessManager;
	QNetworkReply *mNetworkReply;
	bool mShowMessageIfNoNewVersion;
	bool mIsChecking;
	QFile mDownloadedFile;
	QString mDownloadedFilename;
	QString mDownloadedFileHash;
	int mDownloadedFileSize;
	QCryptographicHash mHashCalculator;
	bool mInstallFile;
};

#endif // UPDATER_H

#endif
