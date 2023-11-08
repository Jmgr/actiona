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

#include "mainclass.hpp"
#include "codeexecuter.hpp"
#include "scriptexecuter.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QApplication>
#include <QBuffer>
#include <QTimer>

MainClass::MainClass()
	: QObject(nullptr),
	
	mNetworkAccessManager(new QNetworkAccessManager(this))
{
}

bool MainClass::start(ExecutionMode executionMode, QIODevice *device, const QString &filename)
{
	if(executionMode == Script)
		mExecuter = new ScriptExecuter(this);
	else
		mExecuter = new CodeExecuter(this);

	return mExecuter->start(device, filename);
}

bool MainClass::start(ExecutionMode executionMode, const QUrl &url)
{
	mExecutionMode = executionMode;
	mUrl = url;
	mUrl.setScheme(QStringLiteral("http"));

	mNetworkReply = mNetworkAccessManager->get(QNetworkRequest(mUrl));
    connect(mNetworkReply, &QNetworkReply::finished, this, &MainClass::downloadFinished);

	return true;
}

void MainClass::downloadFinished()
{
	if(mNetworkReply->error() != QNetworkReply::NoError)
	{
		QTextStream stream(stdout);
		stream << QObject::tr("Unable to download the requested file, \"%1\"").arg(mNetworkReply->errorString()) << "\n";
		stream.flush();
		QApplication::exit(-1);
	}
	else
	{
		QByteArray data = mNetworkReply->readAll();
		QBuffer buffer(&data);
		buffer.open(QIODevice::ReadOnly);

		if(!start(mExecutionMode, &buffer, mUrl.toString()))
			QApplication::exit(-1);
	}

	mNetworkReply->close();
	mNetworkReply->deleteLater();
	mNetworkReply = nullptr;
}
