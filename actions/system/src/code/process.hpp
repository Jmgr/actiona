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

#include <QJSValue>
#include <QProcess>

#include "actiontools/code/codeclass.hpp"

namespace Code
{
	class Process : public CodeClass
	{
		Q_OBJECT
		Q_PROPERTY(QJSValue onError READ onError WRITE setOnError)
		Q_PROPERTY(QJSValue onFinished READ onFinished WRITE setOnFinished)
		Q_PROPERTY(QJSValue onReadyReadStandardError READ onReadyReadStandardError WRITE setOnReadyReadStandardError)
		Q_PROPERTY(QJSValue onReadyReadStandardOutput READ onReadyReadStandardOutput WRITE setOnReadyReadStandardOutput)
		Q_PROPERTY(QJSValue onStarted READ onStarted WRITE setOnStarted)
		Q_PROPERTY(QJSValue onStateChanged READ onStateChanged WRITE setOnStateChanged)

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
        Q_ENUM(ProcessError)
		enum ExitStatus
		{
			NormalExit = QProcess::NormalExit,
			CrashExit = QProcess::CrashExit
		};
        Q_ENUM(ExitStatus)
		enum ProcessState
		{
			NotRunning = QProcess::NotRunning,
			Starting = QProcess::Starting,
			Running = QProcess::Running
		};
        Q_ENUM(ProcessState)
		enum ProcessChannel
		{
			StandardOutput = QProcess::StandardOutput,
			StandardError = QProcess::StandardError
		};
        Q_ENUM(ProcessChannel)
		enum ProcessChannelMode
		{
			SeparateChannels = QProcess::SeparateChannels,
			MergedChannels = QProcess::MergedChannels,
			ForwardedChannels = QProcess::ForwardedChannels
		};
        Q_ENUM(ProcessChannelMode)
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
        Q_FLAG(OpenMode)

        Q_INVOKABLE Process();
        Q_INVOKABLE Process(const QJSValue &parameters);

		void setOnError(const QJSValue &onError)									{ mOnError = onError; }
		void setOnFinished(const QJSValue &onFinished)								{ mOnFinished = onFinished; }
		void setOnReadyReadStandardError(const QJSValue &onReadyReadStandardError)	{ mOnReadyReadStandardError = onReadyReadStandardError; }
		void setOnReadyReadStandardOutput(const QJSValue &onReadyReadStandardOutput){ mOnReadyReadStandardOutput = onReadyReadStandardOutput; }
		void setOnStarted(const QJSValue &onStarted)								{ mOnStarted = onStarted; }
		void setOnStateChanged(const QJSValue &onStateChanged)						{ mOnStateChanged = onStateChanged; }

		QJSValue onError() const													{ return mOnError; }
		QJSValue onFinished() const													{ return mOnFinished; }
		QJSValue onReadyReadStandardError() const									{ return mOnReadyReadStandardError; }
		QJSValue onReadyReadStandardOutput() const									{ return mOnReadyReadStandardOutput; }
		QJSValue onStarted() const													{ return mOnStarted; }
		QJSValue onStateChanged() const												{ return mOnStateChanged; }

		QProcess *process() const														{ return mProcess; }

        Q_INVOKABLE QString toString() const override								{ return QStringLiteral("Process"); }
        Q_INVOKABLE QJSValue handle() const;
        Q_INVOKABLE int id() const;
        Q_INVOKABLE Process *start(const QString &filename);
        Q_INVOKABLE Process *start(const QString &filename, const QStringList &parameters);
        Q_INVOKABLE Process *start(const QString &filename, const QStringList &parameters, OpenMode openMode);
        Q_INVOKABLE ProcessState state() const;
        Q_INVOKABLE ProcessError error() const;
        Q_INVOKABLE int exitCode() const;
        Q_INVOKABLE ExitStatus exitStatus() const;
        Q_INVOKABLE QJSValue readError() const;
        Q_INVOKABLE QJSValue read() const;
        Q_INVOKABLE QString readErrorText(Encoding encoding = Native) const;
        Q_INVOKABLE QString readText(Encoding encoding = Native) const;
        Q_INVOKABLE bool atEnd() const;
        Q_INVOKABLE qint64 bytesAvailable() const;
        Q_INVOKABLE qint64 bytesToWrite() const;
        Q_INVOKABLE bool canReadLine() const;
        Q_INVOKABLE Process *write(const QJSValue &data);
        Q_INVOKABLE Process *writeText(const QString &data, Encoding encoding = Native);
        Q_INVOKABLE Process *setWorkingDirectory(const QString &workingDirectory);
        Q_INVOKABLE Process *setProcessChannelMode(ProcessChannelMode channelMode);
        Q_INVOKABLE Process *setEnvironment(const QJSValue &environment);
        Q_INVOKABLE Process *updateEnvironment(const QJSValue &environment);
        Q_INVOKABLE Process *setReadChannel(ProcessChannel channel);
        Q_INVOKABLE Process *setStandardErrorFile(const QString &fileName, int openMode = Truncate);
        Q_INVOKABLE Process *setStandardInputFile(const QString &fileName);
        Q_INVOKABLE Process *setStandardOutputFile(const QString &fileName, int openMode = Truncate);
        Q_INVOKABLE Process *setStandardOutputProcess(const QJSValue &processValue);
        Q_INVOKABLE Process *waitForFinished(int waitTime = 30000);
        Q_INVOKABLE Process *waitForStarted(int waitTime = 30000);
        Q_INVOKABLE Process *waitForBytesWritten(int waitTime = 30000);
        Q_INVOKABLE Process *waitForReadyRead(int waitTime = 30000);
        Q_INVOKABLE Process *close();
        Q_INVOKABLE Process *kill();
        Q_INVOKABLE Process *terminate();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);

	private slots:
        void onError(QProcess::ProcessError processError);
		void finished(int exitCode, QProcess::ExitStatus exitStatus);
		void readyReadStandardError();
		void readyReadStandardOutput();
		void started();
		void stateChanged(QProcess::ProcessState newState);

	private:
		QProcess *mProcess;
		QJSValue mOnError;
		QJSValue mOnFinished;
		QJSValue mOnReadyReadStandardError;
		QJSValue mOnReadyReadStandardOutput;
		QJSValue mOnStarted;
		QJSValue mOnStateChanged;
	};

    class StaticProcess : public CodeClass
    {
        Q_OBJECT

    public:
        StaticProcess(QObject *parent): CodeClass(parent) {}

        Q_INVOKABLE QString toString() const override { return QStringLiteral("StaticProcess"); }
        Q_INVOKABLE QJSValue list();
        Q_INVOKABLE QJSValue startDetached(const QString &filename);
        Q_INVOKABLE QJSValue startDetached(const QString &filename, const QStringList &parameters);
        Q_INVOKABLE QJSValue startDetached(const QString &filename, const QStringList &parameters, const QString &workingDirectory);
        Q_INVOKABLE QJSValue thisProcess();
    };
}

