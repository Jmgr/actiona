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

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QProcess>

#include "code/codetools.h"
#include "code/codeclass.h"

namespace Code
{
	class Process : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(QScriptValue onError READ onError WRITE setOnError)
		Q_PROPERTY(QScriptValue onFinished READ onFinished WRITE setOnFinished)
		Q_PROPERTY(QScriptValue onReadyReadStandardError READ onReadyReadStandardError WRITE setOnReadyReadStandardError)
		Q_PROPERTY(QScriptValue onReadyReadStandardOutput READ onReadyReadStandardOutput WRITE setOnReadyReadStandardOutput)
		Q_PROPERTY(QScriptValue onStarted READ onStarted WRITE setOnStarted)
		Q_PROPERTY(QScriptValue onStateChanged READ onStateChanged WRITE setOnStateChanged)
		Q_ENUMS(ProcessError)
		Q_ENUMS(ExitStatus)
		Q_ENUMS(ProcessState)
		Q_ENUMS(ProcessChannel)
		Q_ENUMS(ProcessChannelMode)
		Q_FLAGS(OpenModeFlag OpenMode)

	public:
		enum ProcessError
		{
			FailedToStart = QProcess::FailedToStart,
			Crashed = QProcess::Crashed,
			Timedout = QProcess::Timedout,
			ReadError = QProcess::ReadError,
			WriteError = QProcess::WriteError,
			UnknownError = QProcess::UnknownError
		};
		enum ExitStatus
		{
			NormalExit = QProcess::NormalExit,
			CrashExit = QProcess::CrashExit
		};
		enum ProcessState
		{
			NotRunning = QProcess::NotRunning,
			Starting = QProcess::Starting,
			Running = QProcess::Running
		};
		enum ProcessChannel
		{
			StandardOutput = QProcess::StandardOutput,
			StandardError = QProcess::StandardError
		};
		enum ProcessChannelMode
		{
			SeparateChannels = QProcess::SeparateChannels,
			MergedChannels = QProcess::MergedChannels,
			ForwardedChannels = QProcess::ForwardedChannels
		};
		enum OpenModeFlag
		{
			NotOpen = QIODevice::NotOpen,
			ReadOnly = QIODevice::ReadOnly,
			WriteOnly = QIODevice::WriteOnly,
			ReadWrite = QIODevice::ReadWrite,
			Append = QIODevice::Append,
			Truncate = QIODevice::Truncate,
			Text = QIODevice::Text,
			Unbuffered = QIODevice::Unbuffered
		};
		Q_DECLARE_FLAGS(OpenMode, OpenModeFlag)

		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

		static QScriptValue list(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue startDetached(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue thisProcess(QScriptContext *context, QScriptEngine *engine);

		Process();

		void setOnError(const QScriptValue &onError)									{ mOnError = onError; }
		void setOnFinished(const QScriptValue &onFinished)								{ mOnFinished = onFinished; }
		void setOnReadyReadStandardError(const QScriptValue &onReadyReadStandardError)	{ mOnReadyReadStandardError = onReadyReadStandardError; }
		void setOnReadyReadStandardOutput(const QScriptValue &onReadyReadStandardOutput){ mOnReadyReadStandardOutput = onReadyReadStandardOutput; }
		void setOnStarted(const QScriptValue &onStarted)								{ mOnStarted = onStarted; }
		void setOnStateChanged(const QScriptValue &onStateChanged)						{ mOnStateChanged = onStateChanged; }

		QScriptValue onError() const													{ return mOnError; }
		QScriptValue onFinished() const													{ return mOnFinished; }
		QScriptValue onReadyReadStandardError() const									{ return mOnReadyReadStandardError; }
		QScriptValue onReadyReadStandardOutput() const									{ return mOnReadyReadStandardOutput; }
		QScriptValue onStarted() const													{ return mOnStarted; }
		QScriptValue onStateChanged() const												{ return mOnStateChanged; }

		QProcess *process() const														{ return mProcess; }

	public slots:
		QString toString() const override														{ return QStringLiteral("Process"); }
        bool equals(const QScriptValue &other) const override                            { return defaultEqualsImplementation<Process>(other); }
		QScriptValue handle() const;
		int id() const;
		QScriptValue start();
		ProcessState state() const;
		ProcessError error() const;
		int exitCode() const;
		ExitStatus exitStatus() const;
		QScriptValue readError() const;
		QScriptValue read() const;
		QString readErrorText(Encoding encoding = Native) const;
		QString readText(Encoding encoding = Native) const;
		bool atEnd() const;
		qint64 bytesAvailable() const;
		qint64 bytesToWrite() const;
		bool canReadLine() const;
		QScriptValue write(const QScriptValue &data);
		QScriptValue writeText(const QString &data, Encoding encoding = Native);
		QScriptValue setWorkingDirectory(const QString &workingDirectory);
		QScriptValue setProcessChannelMode(ProcessChannelMode channelMode);
		QScriptValue setEnvironment();
		QScriptValue updateEnvironment();
		QScriptValue setReadChannel(ProcessChannel channel);
		QScriptValue setStandardErrorFile(const QString &fileName, int openMode = Truncate);
		QScriptValue setStandardInputFile(const QString &fileName);
		QScriptValue setStandardOutputFile(const QString &fileName, int openMode = Truncate);
		QScriptValue setStandardOutputProcess(const QScriptValue &processValue);
		QScriptValue waitForFinished(int waitTime = 30000);
		QScriptValue waitForStarted(int waitTime = 30000);
		QScriptValue waitForBytesWritten(int waitTime = 30000);
		QScriptValue waitForReadyRead(int waitTime = 30000);
		QScriptValue close();
		QScriptValue kill();
		QScriptValue terminate();

	private slots:
        void onError(QProcess::ProcessError processError);
		void finished(int exitCode, QProcess::ExitStatus exitStatus);
		void readyReadStandardError();
		void readyReadStandardOutput();
		void started();
		void stateChanged(QProcess::ProcessState newState);

	private:
		QProcess *mProcess;
		QScriptValue mOnError;
		QScriptValue mOnFinished;
		QScriptValue mOnReadyReadStandardError;
		QScriptValue mOnReadyReadStandardOutput;
		QScriptValue mOnStarted;
		QScriptValue mOnStateChanged;
	};
}

