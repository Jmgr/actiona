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

#include "web.hpp"
#include "actiontools/code/image.hpp"
#include "actiontools/code/rawdata.hpp"
#include "file.hpp"

#include <QJSValueIterator>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QAuthenticator>
#include <QDebug>
#include <QUrlQuery>

namespace Code
{
	Web::Web()
		: CodeClass(),
		  mNetworkAccessManager(new QNetworkAccessManager(this))
		  
	{
        QObject::connect(mNetworkAccessManager, &QNetworkAccessManager::authenticationRequired, this, &Web::authenticationRequired);
	}

    Web::Web(const QJSValue &parameters)
        : Web()
    {
        if(!parameters.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return;
        }

        QJSValueIterator it(parameters);

        while(it.hasNext())
        {
            it.next();

            if(it.name() == QLatin1String("onFinished"))
                mOnFinished = it.value();
            else if(it.name() == QLatin1String("onDownloadProgress"))
                mOnDownloadProgress = it.value();
            else if(it.name() == QLatin1String("onError"))
                mOnError = it.value();
            else if(it.name() == QLatin1String("file"))
                mFileValue = it.value();
        }
    }

    Web::~Web() = default;

    Web *Web::download(const QString &urlString, const QJSValue &options)
	{
		mData.clear();

        if(!mFileValue.isUndefined())
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
                    return this;
				}

				mCloseFile = true;
			}
		}

		QUrl url(urlString);
		if(url.scheme() == QString())
			url = QUrl(QStringLiteral("http://") + urlString, QUrl::TolerantMode);

        QUrlQuery urlQuery;
		QNetworkRequest request;

		QJSValueIterator it(options);
		Method method = Get;
		QByteArray postData;

		while(it.hasNext())
		{
			it.next();

			if(it.name() == QLatin1String("rawHeaders"))
			{
				QJSValueIterator headerIt(it.value());

				while(headerIt.hasNext())
				{
					headerIt.next();

					request.setRawHeader(headerIt.name().toUtf8(), headerIt.value().toString().toUtf8());
				}
			}
			else if(it.name() == QLatin1String("method"))
			{
                method = static_cast<Method>(it.value().toInt());
			}
			else if(it.name() == QLatin1String("postData"))
			{
                QJSValueIterator postDataIt(it.value());
                QUrlQuery postDataParameters;

                while(postDataIt.hasNext())
                {
                    postDataIt.next();

                    postDataParameters.addQueryItem(postDataIt.name(), postDataIt.value().toString());
                }

                postData = postDataParameters.toString(QUrl::FullyEncoded).toLatin1();
			}
            else if(it.name() == QLatin1String("rawData"))
            {
                postData = it.value().toString().toUtf8();
            }
			else if(it.name() == QLatin1String("query"))
			{
				QJSValueIterator queryIt(it.value());

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
        QObject::connect(mNetworkReply, &QNetworkReply::errorOccurred, this, &Web::error);
        QObject::connect(mNetworkReply, &QNetworkReply::readyRead, this, &Web::readyRead);

		mIsDownloading = true;

        return this;
	}

	bool Web::isDownloading() const
	{
		return mIsDownloading;
	}

	QJSValue Web::toImage() const
    {
        return CodeClass::construct<Image>(QImage::fromData(mData));
	}

	QJSValue Web::toText(Encoding encoding) const
	{
		return fromEncoding(mData, encoding);
	}

	QJSValue Web::toRawData() const
	{
        return CodeClass::construct<RawData>(mData);
	}

    Web *Web::cancel()
	{
		if(!mNetworkReply)
            return this;

		mNetworkReply->abort();

        return this;
	}

    void Web::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Web*>("const Web *");

        CodeClass::registerClass<Web>(QStringLiteral("Web"), scriptEngine);
    }


	void Web::finished()
	{
		if(!mNetworkReply)
			return;

        if(!mFileValue.isUndefined() && mFile)
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

        if(!mOnFinished.isUndefined())
            mOnFinished.call();

		mNetworkReply->deleteLater();
		mNetworkReply = nullptr;

		mIsDownloading = false;
	}

	void Web::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
	{
		if(bytesTotal == -1)
			return;

        if(!mOnDownloadProgress.isUndefined())
            mOnDownloadProgress.call(QJSValueList() << static_cast<qreal>(bytesReceived) << static_cast<qreal>(bytesTotal));
	}

	void Web::error()
	{
		if(!mNetworkReply)
			return;

		if(mNetworkReply->error() == QNetworkReply::OperationCanceledError)
			return;

        if(!mOnError.isUndefined())
            mOnError.call(QJSValueList() << mNetworkReply->errorString());
	}

	void Web::authenticationRequired(QNetworkReply *, QAuthenticator *authenticator)
	{
		authenticator->setUser(mUser);
		authenticator->setPassword(mPassword);
	}

	void Web::readyRead()
	{
        if(!mFileValue.isUndefined() && mFile)
			mFile->write(mNetworkReply->readAll());
	}
}
