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

#pragma once

#include "code/codeclass.h"

#include <QScriptValue>
#include <QScriptEngine>

class QNetworkAccessManager;
class QNetworkReply;
class QAuthenticator;
class QFile;

namespace Code
{
	class Web : public CodeClass
	{
		Q_OBJECT
		Q_ENUMS(Method)
		Q_PROPERTY(QScriptValue onFinished READ onFinished WRITE setOnFinished)
		Q_PROPERTY(QScriptValue onDownloadProgress READ onDownloadProgress WRITE setOnDownloadProgress)
		Q_PROPERTY(QScriptValue onError READ onError WRITE setOnError)

	public:
		enum Method
		{
			Get,
			Post
		};

		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

		Web();
		~Web() override;

		void setOnFinished(const QScriptValue &onFinished)					{ mOnFinished = onFinished; }
		void setOnDownloadProgress(const QScriptValue &onDownloadProgress)	{ mOnDownloadProgress = onDownloadProgress; }
		void setOnError(const QScriptValue &onError)						{ mOnError = onError; }

		QScriptValue onFinished() const										{ return mOnFinished; }
		QScriptValue onDownloadProgress() const								{ return mOnDownloadProgress; }
		QScriptValue onError() const										{ return mOnError; }

	public slots:
		QString toString() const override                                            { return QStringLiteral("Web"); }
        bool equals(const QScriptValue &other) const override                { return defaultEqualsImplementation<Web>(other); }
		QScriptValue download(const QString &urlString, const QScriptValue &options = QScriptValue());
		bool isDownloading() const;
		QScriptValue toImage() const;
		QScriptValue toText(Encoding encoding = Native) const;
		QScriptValue toRawData() const;
		QScriptValue cancel();

	private slots:
		void finished();
		void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
		void error();
		void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
		void readyRead();

	private:
		QNetworkAccessManager *mNetworkAccessManager;
		QNetworkReply *mNetworkReply{nullptr};
		QScriptValue mOnFinished;
		QScriptValue mOnDownloadProgress;
		QScriptValue mOnError;
		QScriptValue mFileValue;
		QFile *mFile{nullptr};
		bool mCloseFile{false};
		QByteArray mData;
		bool mIsDownloading{false};
		QString mUser;
		QString mPassword;
	};
}

