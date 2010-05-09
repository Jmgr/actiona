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
