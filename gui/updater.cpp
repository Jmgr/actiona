#include "updater.h"
#include "global.h"
#include "changelogdialog.h"

#include <QProgressDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDate>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QApplication>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTimer>

Updater::Updater(QWidget *parent)
	: QObject(parent),
	mProgressDialog(new QProgressDialog(parent)),
	mParentWidget(parent),
	mNetworkReply(0),
	mShowMessageIfNoNewVersion(true),
	mIsChecking(false),
	mHashCalculator(QCryptographicHash::Md5),
	mInstallFile(false)
{
	connect(&mUpdateClient, SIGNAL(errorOccured(QString)), this, SLOT(errorOccured(QString)));
	connect(&mUpdateClient, SIGNAL(gotBanner(QString)), this, SLOT(gotBanner(QString)));
	connect(&mUpdateClient, SIGNAL(gotProgramInfo(Tools::Version,QString,QString,QString,QString,int,QString)), this, SLOT(gotProgramInfo(Tools::Version,QString,QString,QString,QString,int,QString)));
	connect(mProgressDialog, SIGNAL(canceled()), this, SLOT(stop()));

	mProgressDialog->setMinimumDuration(0);
	mProgressDialog->setAutoClose(false);
	mProgressDialog->setModal(true);
}

Updater::~Updater()
{
	delete mNetworkReply;
	delete mProgressDialog;
}

void Updater::checkNewProgramVersion(bool showMessageIfNoNewVersion)
{
	if(mIsChecking)
	{
		deleteLater();
		return;
	}

	mIsChecking = true;

	mShowMessageIfNoNewVersion = showMessageIfNoNewVersion;

	mProgressDialog->setMinimum(0);
	mProgressDialog->setMaximum(0);
	mProgressDialog->setValue(0);
	mProgressDialog->setLabelText(tr("Checking for updates..."));
	mProgressDialog->setWindowTitle(tr("Update check"));
	mProgressDialog->show();

	mUpdateClient.start(Global::UPDATE_IP, Global::UPDATE_PORT);
}

void Updater::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	mProgressDialog->setValue(bytesReceived);
	mProgressDialog->setMaximum(bytesTotal);
}

void Updater::downloadError(QNetworkReply::NetworkError code)
{
	Q_UNUSED(code);

	if(mNetworkReply)
		QMessageBox::warning(mParentWidget, tr("Update download"), tr("An error occured while downloading the file.\nError message : %1").arg(mNetworkReply->errorString()));
}

void Updater::downloadFinished()
{
	mNetworkReply->deleteLater();
	mNetworkReply = 0;

	mDownloadedFile.close();

	mIsChecking = false;
	mProgressDialog->close();

	if(QFileInfo(mDownloadedFilename).size() != mDownloadedFileSize || mHashCalculator.result().toHex() != mDownloadedFileHash)
	{
		QMessageBox::warning(mParentWidget, tr("Update download"), tr("The downloaded file is corrupted. Try again later."));
		deleteLater();
	}
	else
		QTimer::singleShot(1, this, SLOT(postDownloadOperation()));
}

void Updater::downloadDataAvailable()
{
	const QByteArray &data = mNetworkReply->readAll();
	mDownloadedFile.write(data);
	mHashCalculator.addData(data);
}

void Updater::postDownloadOperation()
{
	if(mInstallFile)
	{
		QProcess::startDetached(mDownloadedFilename);
		QApplication::quit();
	}
	else
		QDesktopServices::openUrl(QUrl("file:///" + QFileInfo(mDownloadedFilename).dir().path(), QUrl::TolerantMode));

	deleteLater();
}

void Updater::stop()
{
	if(mIsChecking)
	{
		mUpdateClient.end();

		mIsChecking = false;
	}

	if(mNetworkReply && mNetworkReply->isRunning())
		mNetworkReply->abort();
}

void Updater::errorOccured(const QString &message)
{
	mUpdateClient.end();

	mProgressDialog->close();

	QMessageBox::warning(mParentWidget, tr("Update check"), tr("An error occured while checking for Actionaz updates.\nError message : %1").arg(message));

	mIsChecking = false;

	deleteLater();
}

void Updater::gotBanner(const QString &message)
{
	Q_UNUSED(message);

	mUpdateClient.askProgramInfo("actionaz", Global::currentLanguage(), "installer", Global::currentOS());
}

void Updater::gotProgramInfo(const Tools::Version &version, const QString &releaseDate, const QString &type, const QString &changelog, const QString &filename, int size, const QString &hash)
{
	mUpdateClient.end();

	mProgressDialog->close();

	if(Global::ACTIONAZ_VERSION < version)
	{
		QString translatedType;

		if(type == "major")
			translatedType = tr("Major");
		else if(type == "release")
			translatedType = tr("Release");
		else
			translatedType = tr("Bugfix");

		QDate translatedReleaseDate = QDate::fromString(releaseDate, Qt::ISODate);

		ChangelogDialog changelogDialog(mParentWidget);
		changelogDialog.setVersion(version);
		changelogDialog.setReleaseDate(translatedReleaseDate.toString(Qt::DefaultLocaleLongDate));
		changelogDialog.setType(translatedType);
		changelogDialog.setChangelog(changelog);

		if(changelogDialog.exec() == QDialog::Accepted)
		{
			if(changelogDialog.changelogAction() == ChangelogDialog::DownloadOnly)
			{
				mInstallFile = false;

				mDownloadedFilename = QFileDialog::getSaveFileName(mParentWidget,
																   tr("Select where to save the installation file"),
																   QDir(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation)).filePath(QFileInfo(filename).fileName()));
			}
			else
			{
				mInstallFile = true;
				mDownloadedFilename = QDir::temp().filePath(QFileInfo(filename).fileName());
			}

			if(mDownloadedFilename.isEmpty())
			{
				mIsChecking = false;

				return;
			}

			mDownloadedFile.setFileName(mDownloadedFilename);
			if(!mDownloadedFile.open(QIODevice::WriteOnly))
			{
				mIsChecking = false;

				QMessageBox::warning(mParentWidget, tr("Download update"), tr("Unable to save the update file."));

				return;
			}

			mDownloadedFileHash = hash;
			mDownloadedFileSize = size;

			mNetworkReply = mNetworkAccessManager.get(QNetworkRequest(filename));

			mHashCalculator.reset();

			connect(mNetworkReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
			connect(mNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(downloadError(QNetworkReply::NetworkError)));
			connect(mNetworkReply, SIGNAL(finished()), this, SLOT(downloadFinished()));
			connect(mNetworkReply, SIGNAL(readyRead()), this, SLOT(downloadDataAvailable()));

			mProgressDialog->setLabelText(tr("Downloading file..."));
			mProgressDialog->setWindowTitle(tr("Update download"));
			mProgressDialog->show();

			return;
		}
	}
	else
	{
		if(mShowMessageIfNoNewVersion)
			QMessageBox::information(mParentWidget, tr("Update check"), tr("No new version is available."));
	}

	deleteLater();

	mIsChecking = false;
}
