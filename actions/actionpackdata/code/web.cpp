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

	Contact : jmgr@jmgr.info
*/

#include "web.h"
#include "code/image.h"
#include "code/rawdata.h"
#include "file.h"

#include <QScriptValueIterator>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QAuthenticator>
#include <QDebug>
#include <QUrlQuery>

namespace Code
{
	QScriptValue Web::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Web *web = new Web;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();

			if(it.name() == QLatin1String("onFinished"))
				web->mOnFinished = it.value();
			else if(it.name() == QLatin1String("onDownloadProgress"))
				web->mOnDownloadProgress = it.value();
			else if(it.name() == QLatin1String("onError"))
				web->mOnError = it.value();
			else if(it.name() == QLatin1String("file"))
				web->mFileValue = it.value();
		}

		return CodeClass::constructor(web, context, engine);
	}

	Web::Web()
		: CodeClass(),
		  mNetworkAccessManager(new QNetworkAccessManager(this))
		  
	{
        QObject::connect(mNetworkAccessManager, &QNetworkAccessManager::authenticationRequired, this, &Web::authenticationRequired);
	}

    Web::~Web() = default;

	QScriptValue Web::download(const QString &urlString, const QScriptValue &options)
	{
		mData.clear();

		if(mFileValue.isValid())
		{
			if(auto file = qobject_cast<Code::File*>(mFileValue.toQObject()))
				mFile = file->file();
			else
				mFile = new QFile(mFileValue.toString(), this);

			mCloseFile = false;

			if(!mFile->isOpen())
			{
				if(!mFile->open(QIODevice::WriteOnly))
				{
					throwError(QStringLiteral("OpenFileError"), tr("Unable to open the destination file"));
					return thisObject();
				}

				mCloseFile = true;
			}
		}

		QUrl url(urlString);
		if(url.scheme() == QString())
			url = QUrl(QStringLiteral("http://") + urlString, QUrl::TolerantMode);

        QUrlQuery urlQuery;
		QNetworkRequest request;

		QScriptValueIterator it(options);
		Method method = Get;
		QByteArray postData;

		while(it.hasNext())
		{
			it.next();

			if(it.name() == QLatin1String("rawHeaders"))
			{
				QScriptValueIterator headerIt(it.value());

				while(headerIt.hasNext())
				{
					headerIt.next();

					request.setRawHeader(headerIt.name().toUtf8(), headerIt.value().toString().toUtf8());
				}
			}
			else if(it.name() == QLatin1String("method"))
			{
				method = static_cast<Method>(it.value().toInt32());
			}
			else if(it.name() == QLatin1String("postData"))
			{
                QScriptValueIterator postDataIt(it.value());
                QUrlQuery postDataParameters;

                while(postDataIt.hasNext())
                {
                    postDataIt.next();

                    postDataParameters.addQueryItem(postDataIt.name(), postDataIt.value().toString());
                }

                postData = postDataParameters.toString(QUrl::FullyEncoded).toLatin1();
			}
			else if(it.name() == QLatin1String("query"))
			{
				QScriptValueIterator queryIt(it.value());

				while(queryIt.hasNext())
				{
					queryIt.next();

                    urlQuery.addQueryItem(queryIt.name(), queryIt.value().toString());
				}
			}
			else if(it.name() == QLatin1String("user"))
			{
				mUser = it.value().toString();
			}
			else if(it.name() == QLatin1String("password"))
			{
				mPassword = it.value().toString();
			}
		}

        url.setQuery(urlQuery);

        request.setUrl(url);

		switch(method)
		{
		case Post:
			mNetworkReply = mNetworkAccessManager->post(request, postData);
			break;
		case Get:
		default:
			mNetworkReply = mNetworkAccessManager->get(request);
			break;
		}

        QObject::connect(mNetworkReply, &QNetworkReply::finished, this, &Web::finished);
        QObject::connect(mNetworkReply, &QNetworkReply::downloadProgress, this, &Web::downloadProgress);
        QObject::connect(mNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &Web::error);
        QObject::connect(mNetworkReply, &QNetworkReply::readyRead, this, &Web::readyRead);

		mIsDownloading = true;

		return thisObject();
	}

	bool Web::isDownloading() const
	{
		return mIsDownloading;
	}

	QScriptValue Web::toImage() const
	{
		return Image::constructor(QImage::fromData(mData), engine());
	}

	QScriptValue Web::toText(Encoding encoding) const
	{
		return fromEncoding(mData, encoding);
	}

	QScriptValue Web::toRawData() const
	{
		return RawData::constructor(mData, engine());
	}

	QScriptValue Web::cancel()
	{
		if(!mNetworkReply)
			return thisObject();

		mNetworkReply->abort();

		return thisObject();
	}

	void Web::finished()
	{
		if(!mNetworkReply)
			return;

		if(mFileValue.isValid() && mFile)
		{
			if(mCloseFile)
			{
				mFile->close();
				mFile->deleteLater();
			}

			mFile = nullptr;
		}
		else
			mData = mNetworkReply->readAll();

		if(mOnFinished.isValid())
			mOnFinished.call(thisObject());

		mNetworkReply->deleteLater();
		mNetworkReply = nullptr;

		mIsDownloading = false;
	}

	void Web::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
	{
		if(bytesTotal == -1)
			return;

		if(mOnDownloadProgress.isValid())
			mOnDownloadProgress.call(thisObject(), QScriptValueList() << static_cast<qsreal>(bytesReceived) << static_cast<qsreal>(bytesTotal));
	}

	void Web::error()
	{
		if(!mNetworkReply)
			return;

		if(mNetworkReply->error() == QNetworkReply::OperationCanceledError)
			return;

		if(mOnError.isValid())
			mOnError.call(thisObject(), QScriptValueList() << mNetworkReply->errorString());
	}

	void Web::authenticationRequired(QNetworkReply *, QAuthenticator *authenticator)
	{
		authenticator->setUser(mUser);
		authenticator->setPassword(mPassword);
	}

	void Web::readyRead()
	{
		if(mFileValue.isValid() && mFile)
			mFile->write(mNetworkReply->readAll());
	}
}
