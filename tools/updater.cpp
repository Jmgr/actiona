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

#include "updater.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDate>

namespace Tools
{
	Updater::Updater(QNetworkAccessManager *networkAccessManager, const QUrl &url, QObject *parent)
		: QObject(parent),
		mUrl(url),
		mNetworkAccessManager(networkAccessManager),
		mCurrentReply(0)
	{
	}
	
	Updater::~Updater()
	{
	}
	
	void Updater::checkForUpdates(const QString &program,
							const Version &programVersion,
							FileType fileType,
							const QString &operatingSystem,
							const QString &language)
	{
		if(mCurrentReply)
			return;
		
		connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
		
		QUrl url(mUrl);
		url.addQueryItem("request", "program");
		url.addQueryItem("protocol", "0");
		switch(fileType)
		{
		case Installer:
			url.addQueryItem("type", "installer");
			break;
		case Source:
			url.addQueryItem("type", "source");
			break;
		case Binary:
			url.addQueryItem("type", "binary");
			break;
		}
		url.addQueryItem("os", operatingSystem);
		url.addQueryItem("language", language);
		url.addQueryItem("program", program);
		
		QNetworkRequest request(url);
		request.setRawHeader("User-Agent", QString("%1 %2").arg(program).arg(programVersion.toString()).toAscii());
		
		mNetworkAccessManager->get(request);
	}
	
	void Updater::cancel()
	{
		if(!mCurrentReply)
			return;
		
		mCurrentReply->abort();
		mCurrentReply->deleteLater();
		mCurrentReply = 0;
		disconnect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	}
	
	void Updater::replyFinished(QNetworkReply *reply)
	{
		disconnect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
		mCurrentReply = 0;
		reply->deleteLater();
		
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
		int size;
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
				version.setFromString(stream.readElementText());
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
}

#endif
