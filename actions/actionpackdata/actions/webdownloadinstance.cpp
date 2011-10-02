/*
		Actionaz
		Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#include "webdownloadinstance.h"
#include "actioninstanceexecutionhelper.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProgressDialog>

namespace Actions
{
	ActionTools::StringListPair WebDownloadInstance::destinations = qMakePair(
			QStringList() << "variable" << "file",
			QStringList() << QT_TRANSLATE_NOOP("WebDownloadInstance::destinations", "Variable")
						  << QT_TRANSLATE_NOOP("WebDownloadInstance::destinations", "File"));

	WebDownloadInstance::WebDownloadInstance(const ActionTools::ActionDefinition *definition, QObject *parent)
		: ActionTools::ActionInstance(definition, parent),
		  mNetworkAccessManager(new QNetworkAccessManager(this)),
		  mReply(0),
		  mDestination(Variable),
		  mProgressDialog(new QProgressDialog)
	{
		connect(mProgressDialog, SIGNAL(canceled()), this, SLOT(canceled()));
	}

	WebDownloadInstance::~WebDownloadInstance()
	{
		delete mProgressDialog;
	}

	void WebDownloadInstance::startExecution()
	{
		ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
		QString urlString;
		QString file;

		if(!actionInstanceExecutionHelper.evaluateString(urlString, "url") ||
		   !actionInstanceExecutionHelper.evaluateListElement(mDestination, destinations, "destination") ||
		   !actionInstanceExecutionHelper.evaluateVariable(mVariable, "variable") ||
		   !actionInstanceExecutionHelper.evaluateString(file, "file"))
			return;

		QUrl url(urlString);
		if(url.scheme() == QString())
			url = QUrl("http://" + urlString, QUrl::TolerantMode);

		if(!url.isValid())
		{
			actionInstanceExecutionHelper.setCurrentParameter("url");
			emit executionException(InvalidURLException, tr("Invalid URL"));
			return;
		}

		if(mDestination == File)
		{
			mFile.setFileName(file);
			if(!mFile.open(QIODevice::WriteOnly))
			{
				actionInstanceExecutionHelper.setCurrentParameter("file");
				emit executionException(CannotOpenFileException, tr("Cannot write to file"));
				return;
			}
		}

		mReply = mNetworkAccessManager->get(QNetworkRequest(url));

		connect(mReply, SIGNAL(finished()), this, SLOT(finished()));
		connect(mReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
		connect(mReply, SIGNAL(readyRead()), this, SLOT(readyRead()));

		mProgressDialog->setModal(false);
		mProgressDialog->setWindowTitle(tr("Downloading"));
		mProgressDialog->setLabelText(tr("Downloading..."));
		mProgressDialog->setMaximum(100);
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
			{
				ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());

				actionInstanceExecutionHelper.setVariable(mVariable, QString::fromUtf8(mReply->readAll()));
			}

			emit executionEnded();
			break;
		case QNetworkReply::OperationCanceledError:
			if(mDestination == File)
				mFile.remove();

			emit executionEnded();
			break;
		default:
			{
				if(mDestination == File)
					mFile.remove();

				ActionTools::ActionInstanceExecutionHelper actionInstanceExecutionHelper(this, script(), scriptEngine());
				actionInstanceExecutionHelper.setCurrentParameter("url");

				emit executionException(DownloadException, tr("Download error: %1").arg(mReply->errorString()));
			}
			break;
		}

		mProgressDialog->close();

		mReply->deleteLater();
		mReply = 0;
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
