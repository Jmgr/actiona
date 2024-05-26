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

#pragma once

#include "actiontools/code/codeclass.hpp"

#include <QJSValue>

class QNetworkAccessManager;
class QNetworkReply;
class QAuthenticator;
class QFile;

namespace Code
{
	class Web : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(QJSValue onFinished READ onFinished WRITE setOnFinished)
		Q_PROPERTY(QJSValue onDownloadProgress READ onDownloadProgress WRITE setOnDownloadProgress)
		Q_PROPERTY(QJSValue onError READ onError WRITE setOnError)

	public:
		enum Method
		{
			Get,
			Post
		};
        Q_ENUM(Method)

        Q_INVOKABLE Web();
        Q_INVOKABLE Web(const QJSValue &parameters);
		~Web() override;

		void setOnFinished(const QJSValue &onFinished)					{ mOnFinished = onFinished; }
		void setOnDownloadProgress(const QJSValue &onDownloadProgress)	{ mOnDownloadProgress = onDownloadProgress; }
		void setOnError(const QJSValue &onError)						{ mOnError = onError; }

		QJSValue onFinished() const										{ return mOnFinished; }
		QJSValue onDownloadProgress() const								{ return mOnDownloadProgress; }
		QJSValue onError() const										{ return mOnError; }

        Q_INVOKABLE QString toString() const override                                            { return QStringLiteral("Web"); }
        Q_INVOKABLE Web *download(const QString &urlString, const QJSValue &options = QJSValue());
        Q_INVOKABLE bool isDownloading() const;
        Q_INVOKABLE QJSValue toImage() const;
        Q_INVOKABLE QJSValue toText(Encoding encoding = Native) const;
        Q_INVOKABLE QJSValue toRawData() const;
        Q_INVOKABLE Web *cancel();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private slots:
		void finished();
		void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
		void error();
		void authenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
		void readyRead();

	private:
		QNetworkAccessManager *mNetworkAccessManager;
		QNetworkReply *mNetworkReply{nullptr};
		QJSValue mOnFinished;
		QJSValue mOnDownloadProgress;
		QJSValue mOnError;
		QJSValue mFileValue;
		QFile *mFile{nullptr};
		bool mCloseFile{false};
		QByteArray mData;
		bool mIsDownloading{false};
		QString mUser;
		QString mPassword;
	};
}

