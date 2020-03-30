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

#include "tools/updater.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDate>
#include <QTimer>
#include <QUrlQuery>
#include <QSysInfo>
#include <QVersionNumber>

namespace Tools
{
	Updater::Updater(QNetworkAccessManager *networkAccessManager, const QUrl &url, int timeout, QObject *parent)
		: QObject(parent),
		  mUrl(url),
		  mNetworkAccessManager(networkAccessManager),
		  mCurrentReply(nullptr),
		  mTimeoutTimer(new QTimer(this))
	{
        connect(mTimeoutTimer, &QTimer::timeout, this, &Updater::timeout);

		mTimeoutTimer->setSingleShot(true);
		mTimeoutTimer->setInterval(timeout);
	}
	
    Updater::~Updater() = default;
	
	void Updater::checkForUpdates(const QString &program,
							const QVersionNumber &programVersion,
                            int programBits,
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

		urlQuery.addQueryItem(QStringLiteral("request"), QStringLiteral("program"));
		urlQuery.addQueryItem(QStringLiteral("protocol"), QString::number(Protocol));
        switch(fileType)
        {
        case Source:
			urlQuery.addQueryItem(QStringLiteral("type"), QStringLiteral("source"));
            break;
        case Binary:
			urlQuery.addQueryItem(QStringLiteral("type"), QStringLiteral("binary"));
            break;
        }
        switch(containerType)
        {
        case Installer:
			urlQuery.addQueryItem(QStringLiteral("container"), QStringLiteral("installer"));
            break;
        case SevenZip:
			urlQuery.addQueryItem(QStringLiteral("container"), QStringLiteral("7z"));
            break;
        case Zip:
			urlQuery.addQueryItem(QStringLiteral("container"), QStringLiteral("zip"));
            break;
        case TarGz:
			urlQuery.addQueryItem(QStringLiteral("container"), QStringLiteral("targz"));
            break;
        case TarBz2:
			urlQuery.addQueryItem(QStringLiteral("container"), QStringLiteral("tarbz2"));
            break;
        case Deb:
			urlQuery.addQueryItem(QStringLiteral("container"), QStringLiteral("deb"));
            break;
        case Rpm:
			urlQuery.addQueryItem(QStringLiteral("container"), QStringLiteral("rpm"));
            break;
        }
        urlQuery.addQueryItem(QStringLiteral("osName"), operatingSystem);
        urlQuery.addQueryItem(QStringLiteral("osVariant"), QSysInfo::productType());
        urlQuery.addQueryItem(QStringLiteral("osVersion"), QSysInfo::productVersion());
        urlQuery.addQueryItem(QStringLiteral("osBits"), QString::number(operatingSystemBits));
        urlQuery.addQueryItem(QStringLiteral("language"), language);
        urlQuery.addQueryItem(QStringLiteral("program"), program);
        urlQuery.addQueryItem(QStringLiteral("programBits"), QString::number(programBits));

        url.setQuery(urlQuery);

        QString buildName = QStringLiteral(ACT_BUILD_NAME);

        if(buildName.isEmpty())
            buildName.clear();
        else
        {
            buildName = QStringLiteral("-") + QStringLiteral(ACT_BUILD_NAME);
        }

		QNetworkRequest request(url);
        request.setRawHeader("User-Agent", QStringLiteral("%1 %2%3").arg(program).arg(programVersion.toString()).arg(buildName).toLatin1());
		
		mCurrentReply = mNetworkAccessManager->get(request);

        connect(mCurrentReply, &QNetworkReply::finished, this, &Updater::replyFinished);
        connect(mCurrentReply, &QNetworkReply::downloadProgress, this, &Updater::downloadProgress);

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

		mCurrentReply = nullptr;
		
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
		
		QVersionNumber version;
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
			
			if(stream.name() == QLatin1String("error"))
			{
				emit error(stream.readElementText() + QStringLiteral("."));
				return;
			}
			else if(stream.name() == QLatin1String("noresult"))
			{
				emit noResult();
				return;
			}
			else if(stream.name() == QLatin1String("version"))
                version = QVersionNumber::fromString(stream.readElementText());
			else if(stream.name() == QLatin1String("releaseDate"))
				date = QDate::fromString(stream.readElementText(), Qt::ISODate);
			else if(stream.name() == QLatin1String("type"))
				type = stream.readElementText();
			else if(stream.name() == QLatin1String("changelog"))
				changelog = stream.readElementText();
			else if(stream.name() == QLatin1String("filename"))
				filename = stream.readElementText();
			else if(stream.name() == QLatin1String("size"))
				size = stream.readElementText().toInt();
			else if(stream.name() == QLatin1String("hash"))
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
