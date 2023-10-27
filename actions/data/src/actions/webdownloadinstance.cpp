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

#include "webdownloadinstance.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProgressDialog>

namespace Actions
{
    Tools::StringListPair WebDownloadInstance::destinations =
    {
        {
            QStringLiteral("variable"),
            QStringLiteral("file")
        },
        {
            QStringLiteral(QT_TRANSLATE_NOOP("WebDownloadInstance::destinations", "Variable")),
            QStringLiteral(QT_TRANSLATE_NOOP("WebDownloadInstance::destinations", "File"))
        }
    };

	WebDownloadInstance::WebDownloadInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mNetworkAccessManager(new QNetworkAccessManager(this)),
		  mReply(nullptr),
		  mDestination(Variable),
		  mProgressDialog(new QProgressDialog)
	{
        mProgressDialog->close(); // Prevent the progress dialog from displaying after 4 seconds

        connect(mProgressDialog, &QProgressDialog::canceled, this, &WebDownloadInstance::canceled);
	}

	WebDownloadInstance::~WebDownloadInstance()
	{
		delete mProgressDialog;
	}

	void WebDownloadInstance::startExecution()
	{
		bool ok = true;

		QString urlString = evaluateString(ok, QStringLiteral("url"));
		mDestination = evaluateListElement<Destination>(ok, destinations, QStringLiteral("destination"));
		mVariable = evaluateVariable(ok, QStringLiteral("variable"));
		QString file = evaluateString(ok, QStringLiteral("file"));


		if(!ok)
			return;

		QUrl url(urlString);
		if(url.scheme() == QString())
			url = QUrl(QStringLiteral("http://") + urlString, QUrl::TolerantMode);

		if(!url.isValid())
		{
			setCurrentParameter(QStringLiteral("url"));
			emit executionException(ActionTools::ActionException::InvalidParameterException, tr("Invalid URL"));
			return;
		}

		if(mDestination == File)
		{
			mFile.setFileName(file);
			if(!mFile.open(QIODevice::WriteOnly))
			{
				setCurrentParameter(QStringLiteral("file"));
				emit executionException(CannotOpenFileException, tr("Cannot write to file"));
				return;
			}
		}

		mReply = mNetworkAccessManager->get(QNetworkRequest(url));

        connect(mReply, &QNetworkReply::finished, this, &WebDownloadInstance::finished);
        connect(mReply, &QNetworkReply::downloadProgress, this, &WebDownloadInstance::downloadProgress);
        connect(mReply, &QNetworkReply::readyRead, this, &WebDownloadInstance::readyRead);

		mProgressDialog->setModal(false);
		mProgressDialog->setWindowTitle(tr("Downloading"));
		mProgressDialog->setLabelText(tr("Downloading..."));
		mProgressDialog->setMaximum(100);
        mProgressDialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
		mProgressDialog->show();
	}

	void WebDownloadInstance::stopExecution()
	{
		if(mReply)
			mReply->abort();
	}

	void WebDownloadInstance::finished()
	{
		mFile.close();

		switch(mReply->error())
		{
		case QNetworkReply::NoError:
			if(mDestination == Variable)
                setVariable(mVariable, QString::fromUtf8(mReply->readAll()));

            executionEnded();
			break;
		case QNetworkReply::OperationCanceledError:
			if(mDestination == File)
				mFile.remove();

            executionEnded();
			break;
		default:
			{
				if(mDestination == File)
					mFile.remove();

				setCurrentParameter(QStringLiteral("url"));

				emit executionException(DownloadException, tr("Download error: %1").arg(mReply->errorString()));
			}
			break;
		}

		mProgressDialog->close();

		mReply->deleteLater();
		mReply = nullptr;
	}

	void WebDownloadInstance::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
	{
		if(bytesTotal > 0)
			mProgressDialog->setValue((bytesReceived * 100) / bytesTotal);
	}

	void WebDownloadInstance::readyRead()
	{
		if(mDestination == File)
			mFile.write(mReply->readAll());
	}

	void WebDownloadInstance::canceled()
	{
		if(mReply)
			mReply->abort();
	}
}
