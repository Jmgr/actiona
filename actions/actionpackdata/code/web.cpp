/*
	Actionaz
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

namespace Code
{
	QScriptValue Web::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Web *web = new Web;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();

			if(it.name() == "onFinished")
				web->mOnFinished = it.value();
			else if(it.name() == "onDownloadProgress")
				web->mOnDownloadProgress = it.value();
			else if(it.name() == "onError")
				web->mOnError = it.value();
			else if(it.name() == "file")
				web->mFileValue = it.value();
		}

		return CodeClass::constructor(web, context, engine);
	}

	Web::Web()
		: CodeClass(),
		  mNetworkAccessManager(new QNetworkAccessManager(this)),
		  mNetworkReply(0),
		  mFile(0),
		  mCloseFile(false),
		  mIsDownloading(false)
	{
		QObject::connect(mNetworkAccessManager, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)), this, SLOT(authenticationRequired(QNetworkReply *, QAuthenticator *)));
	}

	Web::~Web()
    {
    }

    bool Web::equals(const QScriptValue &other) const
    {
        if(other.isUndefined() || other.isNull())
            return false;

        QObject *object = other.toQObject();
        if(Web *otherWeb = qobject_cast<Web*>(object))
            return (otherWeb == this || otherWeb->mNetworkAccessManager == mNetworkAccessManager);

        return false;
    }

	QScriptValue Web::download(const QString &urlString, const QScriptValue &options)
	{
		mData.clear();

		if(mFileValue.isValid())
		{
			if(Code::File *file = qobject_cast<Code::File*>(mFileValue.toQObject()))
				mFile = file->file();
			else
				mFile = new QFile(mFileValue.toString(), this);

			mCloseFile = false;

			if(!mFile->isOpen())
			{
				if(!mFile->open(QIODevice::WriteOnly))
				{
					throwError("OpenFileError", tr("Unable to open the destination file"));
					return thisObject();
				}

				mCloseFile = true;
			}
		}

		QUrl url(urlString);
		if(url.scheme() == QString())
			url = QUrl("http://" + urlString, QUrl::TolerantMode);

		QNetworkRequest request;

		QScriptValueIterator it(options);
		Method method = Get;
		QByteArray postData;

		while(it.hasNext())
		{
			it.next();

			if(it.name() == "rawHeaders")
			{
				QScriptValueIterator headerIt(it.value());

				while(headerIt.hasNext())
				{
					headerIt.next();

					request.setRawHeader(headerIt.name().toUtf8(), headerIt.value().toString().toUtf8());
				}
			}
			else if(it.name() == "method")
			{
				method = static_cast<Method>(it.value().toInt32());
			}
			else if(it.name() == "postData")
			{
				QScriptValueIterator postDataIt(it.value());
				QUrl postDataParameters;

				while(postDataIt.hasNext())
				{
					postDataIt.next();

					postDataParameters.addQueryItem(postDataIt.name(), postDataIt.value().toString());
				}

				postData = postDataParameters.encodedQuery();
			}
			else if(it.name() == "query")
			{
				QScriptValueIterator queryIt(it.value());

				while(queryIt.hasNext())
				{
					queryIt.next();

					url.addQueryItem(queryIt.name(), queryIt.value().toString());
				}
			}
			else if(it.name() == "user")
			{
				mUser = it.value().toString();
			}
			else if(it.name() == "password")
			{
				mPassword = it.value().toString();
			}
		}

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

		QObject::connect(mNetworkReply, SIGNAL(finished()), this, SLOT(finished()));
		QObject::connect(mNetworkReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
		QObject::connect(mNetworkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error()));
		QObject::connect(mNetworkReply, SIGNAL(readyRead()), this, SLOT(readyRead()));

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

			mFile = 0;
		}
		else
			mData = mNetworkReply->readAll();

		if(mOnFinished.isValid())
			mOnFinished.call(thisObject());

		mNetworkReply->deleteLater();
		mNetworkReply = 0;

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
