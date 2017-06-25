/*
    Actiona
	Copyright (C) 2005-2017 Jonathan Mercier-Ganady

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

#include "updater.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDate>
#include <QTimer>
#include <QUrlQuery>

namespace Tools
{
	Updater::Updater(QNetworkAccessManager *networkAccessManager, const QUrl &url, int timeout, QObject *parent)
		: QObject(parent),
		  mUrl(url),
		  mNetworkAccessManager(networkAccessManager),
		  mCurrentReply(0),
		  mTimeoutTimer(new QTimer(this))
	{
		connect(mTimeoutTimer, SIGNAL(timeout()), this, SLOT(timeout()));

		mTimeoutTimer->setSingleShot(true);
		mTimeoutTimer->setInterval(timeout);
	}
	
	Updater::~Updater()
	{
	}
	
	void Updater::checkForUpdates(const QString &program,
							const Version &programVersion,
							FileType fileType,
							ContainerType containerType,
							const QString &operatingSystem,
							int operatingSystemBits,
							const QString &language)
	{
		if(mCurrentReply)
			return;
		
		QUrl url(mUrl);
        QUrlQuery urlQuery;

        urlQuery.addQueryItem("request", "program");
        urlQuery.addQueryItem("protocol", QString::number(Protocol));
        switch(fileType)
        {
        case Source:
            urlQuery.addQueryItem("type", "source");
            break;
        case Binary:
            urlQuery.addQueryItem("type", "binary");
            break;
        }
        switch(containerType)
        {
        case Installer:
            urlQuery.addQueryItem("container", "installer");
            break;
        case SevenZip:
            urlQuery.addQueryItem("container", "7z");
            break;
        case Zip:
            urlQuery.addQueryItem("container", "zip");
            break;
        case TarGz:
            urlQuery.addQueryItem("container", "targz");
            break;
        case TarBz2:
            urlQuery.addQueryItem("container", "tarbz2");
            break;
        case Deb:
            urlQuery.addQueryItem("container", "deb");
            break;
        case Rpm:
            urlQuery.addQueryItem("container", "rpm");
            break;
        }
        urlQuery.addQueryItem("osName", operatingSystem);
        urlQuery.addQueryItem("osBits", QString::number(operatingSystemBits));
        urlQuery.addQueryItem("language", language);
        urlQuery.addQueryItem("program", program);

        url.setQuery(urlQuery);

		QNetworkRequest request(url);
        request.setRawHeader("User-Agent", QString("%1 %2").arg(program).arg(programVersion.toString()).toLatin1());
		
		mCurrentReply = mNetworkAccessManager->get(request);

		connect(mCurrentReply, SIGNAL(finished()), this, SLOT(replyFinished()));
		connect(mCurrentReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress()));

		mTimeoutTimer->start();
	}
	
	void Updater::cancel()
	{
		mTimeoutTimer->stop();

		if(!mCurrentReply)
			return;
		
		mCurrentReply->abort();
	}
	
	void Updater::replyFinished()
	{
		mTimeoutTimer->stop();

		mCurrentReply->deleteLater();

		QNetworkReply *reply = mCurrentReply;

		mCurrentReply = 0;
		
		if(reply->error() != QNetworkReply::NoError)
		{
			switch(reply->error())
			{
			case QNetworkReply::ConnectionRefusedError:
				emit error(tr("Connection to the server was refused."));
				return;
			case QNetworkReply::HostNotFoundError:
				emit error(tr("Unable to establish a connection to the server."));
				return;
			case QNetworkReply::TimeoutError:
				emit error(tr("Connection timeout."));
				return;
			case QNetworkReply::OperationCanceledError:
				return;
			case QNetworkReply::ContentNotFoundError:
				emit error(tr("Serverside error."));
				return;
			default:
				emit error(tr("Connection error."));
				return;
			}
		}
		
		QXmlStreamReader stream(reply);
		stream.readNextStartElement();
		stream.readNextStartElement();
		
		Version version;
		QDate date;
		QString type;
		QString changelog;
		QString filename;
		int size = 0;
		QString hash;
		
		for(;!stream.atEnd();stream.readNext())
		{
			if(!stream.isStartElement())
				continue;
			
			if(stream.name() == "error")
			{
				emit error(stream.readElementText() + ".");
				return;
			}
			else if(stream.name() == "noresult")
			{
				emit noResult();
				return;
			}
			else if(stream.name() == "version")
#if (QT_VERSION >= 0x050600)
                version = QVersionNumber::fromString(stream.readElementText());
#else
				version.setFromString(stream.readElementText());
#endif
			else if(stream.name() == "releaseDate")
				date = QDate::fromString(stream.readElementText(), Qt::ISODate);
			else if(stream.name() == "type")
				type = stream.readElementText();
			else if(stream.name() == "changelog")
				changelog = stream.readElementText();
			else if(stream.name() == "filename")
				filename = stream.readElementText();
			else if(stream.name() == "size")
				size = stream.readElementText().toInt();
			else if(stream.name() == "hash")
				hash = stream.readElementText();
		}
		
		emit success(version, date, type, changelog, filename, size, hash);
	}

	void Updater::timeout()
	{
		cancel();

		emit error(tr("Connection timeout."));
	}

	void Updater::downloadProgress()
	{
		//Restart the timer
		if(mTimeoutTimer->isActive())
			mTimeoutTimer->start();
	}
}
