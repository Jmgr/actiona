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

#include "process.hpp"
#include "actiontools/code/processhandle.hpp"
#include "actiontools/code/rawdata.hpp"
#include "actiontools/scriptengine.hpp"

#include <QDir>
#include <QJSValueIterator>

#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#ifdef Q_OS_UNIX
#include <unistd.h>
#endif

namespace Code
{
    Process::Process()
        : CodeClass(),
        mProcess(new QProcess(this))
    {
        connect(mProcess, static_cast<void (QProcess::*)(QProcess::ProcessError)>(&QProcess::errorOccurred), this, static_cast<void (Process::*)(QProcess::ProcessError)>(&Process::onError));
        connect(mProcess, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &Process::finished);
        connect(mProcess, &QProcess::readyReadStandardError, this, &Process::readyReadStandardError);
        connect(mProcess, &QProcess::readyReadStandardOutput, this, &Process::readyReadStandardOutput);
        connect(mProcess, &QProcess::started, this, &Process::started);
        connect(mProcess, &QProcess::stateChanged, this, &Process::stateChanged);
    }

    Process::Process(const QJSValue &parameters)
        : Process()
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

            if(it.name() == QLatin1String("workingDirectory"))
                process()->setWorkingDirectory(it.value().toString());
            else if(it.name() == QLatin1String("processChannelMode"))
                process()->setProcessChannelMode(static_cast<QProcess::ProcessChannelMode>(it.value().toInt()));
            else if(it.name() == QLatin1String("readChannel"))
                process()->setReadChannel(static_cast<QProcess::ProcessChannel>(it.value().toInt()));
            else if(it.name() == QLatin1String("onError"))
                mOnError = it.value();
            else if(it.name() == QLatin1String("onFinished"))
                mOnFinished = it.value();
            else if(it.name() == QLatin1String("onReadyReadStandardError"))
                mOnReadyReadStandardError = it.value();
            else if(it.name() == QLatin1String("onReadyReadStandardOutput"))
                mOnReadyReadStandardOutput = it.value();
            else if(it.name() == QLatin1String("onStarted"))
                mOnStarted = it.value();
            else if(it.name() == QLatin1String("onStateChanged"))
                mOnStateChanged = it.value();
        }
    }

	QJSValue Process::handle() const
	{
        return CodeClass::construct<ProcessHandle>(id());
	}

	int Process::id() const
	{
        return mProcess->processId();
	}

    Process *Process::start(const QString &filename)
    {
        return start(filename, {}, ReadWrite);
    }

    Process *Process::start(const QString &filename, const QStringList &parameters)
    {
        return start(filename, parameters, ReadWrite);
    }

    Process *Process::start(const QString &filename, const QStringList &parameters, OpenMode openMode)
    {
        if(filename.isEmpty())
        {
            throwError(QStringLiteral("FilenameError"), tr("Invalid filename"));
            return this;
        }

        auto ioDeviceOpenMode = static_cast<QIODevice::OpenMode>(static_cast<int>(openMode));

        mProcess->start(filename, parameters, ioDeviceOpenMode);

        return this;
    }

	Process::ProcessState Process::state() const
	{
		return static_cast<ProcessState>(mProcess->state());
	}

	Process::ProcessError Process::error() const
	{
		return static_cast<ProcessError>(mProcess->error());
	}

	int Process::exitCode() const
	{
		return mProcess->exitCode();
	}

	Process::ExitStatus Process::exitStatus() const
	{
		return static_cast<ExitStatus>(mProcess->exitStatus());
	}

	QJSValue Process::readError() const
    {
        return CodeClass::construct<RawData>(mProcess->readAllStandardError());
	}

	QJSValue Process::read() const
    {
        return CodeClass::construct<RawData>(mProcess->readAllStandardOutput());
	}

	QString Process::readErrorText(Encoding encoding) const
	{
		return fromEncoding(mProcess->readAllStandardError(), encoding);
	}

	QString Process::readText(Encoding encoding) const
	{
		return fromEncoding(mProcess->readAllStandardOutput(), encoding);
	}

	bool Process::atEnd() const
	{
		return mProcess->atEnd();
	}

	qint64 Process::bytesAvailable() const
	{
		return mProcess->bytesAvailable();
	}

	qint64 Process::bytesToWrite() const
	{
		return mProcess->bytesToWrite();
	}

	bool Process::canReadLine() const
	{
		return mProcess->canReadLine();
	}

    Process *Process::write(const QJSValue &data)
	{
		QObject *object = data.toQObject();
		if(auto rawData = qobject_cast<RawData*>(object))
		{
			if(mProcess->write(rawData->byteArray()) == -1)
				throwError(QStringLiteral("WriteError"), tr("Write failed"));
		}
		else
		{
			if(mProcess->write(data.toVariant().toByteArray()) == -1)
				throwError(QStringLiteral("WriteError"), tr("Write failed"));
		}

        return this;
	}

    Process *Process::writeText(const QString &data, Encoding encoding)
	{
		if(mProcess->write(toEncoding(data, encoding)) == -1)
			throwError(QStringLiteral("WriteError"), tr("Write failed"));

        return this;
	}

    Process *Process::setWorkingDirectory(const QString &workingDirectory)
	{
		mProcess->setWorkingDirectory(workingDirectory);

        return this;
	}

    Process *Process::setProcessChannelMode(ProcessChannelMode channelMode)
	{
		mProcess->setProcessChannelMode(static_cast<QProcess::ProcessChannelMode>(channelMode));

        return this;
	}

    Process *Process::setEnvironment(const QJSValue &environment)
	{
        if(!environment.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return this;
        }

        QProcessEnvironment processEnvironment;
        QJSValueIterator it(environment);

		while(it.hasNext())
		{
			it.next();

            processEnvironment.insert(it.name(), it.value().toString());
		}

        mProcess->setProcessEnvironment(processEnvironment);

        return this;
	}

    Process *Process::updateEnvironment(const QJSValue &environment)
	{
        if(!environment.isObject())
        {
            throwError(QStringLiteral("ObjectParameter"), QStringLiteral("parameter has to be an object"));
            return this;
        }

        QProcessEnvironment processEnvironment = QProcessEnvironment::systemEnvironment();
        QJSValueIterator it(environment);

		while(it.hasNext())
		{
			it.next();

            processEnvironment.insert(it.name(), it.value().toString());
		}

        mProcess->setProcessEnvironment(processEnvironment);

        return this;
	}

    Process *Process::setReadChannel(ProcessChannel channel)
	{
		mProcess->setReadChannel(static_cast<QProcess::ProcessChannel>(channel));

        return this;
	}

    Process *Process::setStandardErrorFile(const QString &fileName, int openMode)
	{
		mProcess->setStandardErrorFile(fileName, static_cast<QIODevice::OpenModeFlag>(openMode));

        return this;
	}

    Process *Process::setStandardInputFile(const QString &fileName)
	{
		mProcess->setStandardInputFile(fileName);

        return this;
	}

    Process *Process::setStandardOutputFile(const QString &fileName, int openMode)
	{
		mProcess->setStandardOutputFile(fileName, static_cast<QIODevice::OpenModeFlag>(openMode));

        return this;
	}

    Process *Process::setStandardOutputProcess(const QJSValue &processValue)
	{
		auto otherProcess = qobject_cast<Process *>(processValue.toQObject());
		if(!otherProcess)
		{
			throwError(QStringLiteral("InvalidProcessError"), tr("Invalid process"));
            return this;
		}

		mProcess->setStandardOutputProcess(otherProcess->process());

        return this;
	}

    Process *Process::waitForFinished(int waitTime)
	{
		if(!mProcess->waitForFinished(waitTime))
			throwError(QStringLiteral("WaitForFinishedError"), tr("Wait for finished failed"));

        return this;
	}

    Process *Process::waitForStarted(int waitTime)
	{
		if(!mProcess->waitForStarted(waitTime))
			throwError(QStringLiteral("WaitForStartedError"), tr("Wait for started failed"));

        return this;
	}

    Process *Process::waitForBytesWritten(int waitTime)
	{
		if(!mProcess->waitForBytesWritten(waitTime))
			throwError(QStringLiteral("WaitForBytesWrittenError"), tr("Waiting for bytes written failed"));

        return this;
	}

    Process *Process::waitForReadyRead(int waitTime)
	{
		if(!mProcess->waitForReadyRead(waitTime))
			throwError(QStringLiteral("WaitForReadyReadError"), tr("Waiting for ready read failed"));

        return this;
	}

    Process *Process::close()
	{
		mProcess->close();

        return this;
	}

    Process *Process::kill()
	{
		mProcess->kill();

        return this;
	}

    Process *Process::terminate()
	{
		mProcess->terminate();

        return this;
	}

    void Process::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<Process*>("const Process *");

        CodeClass::registerClassWithStaticFunctions<Process, StaticProcess>(QStringLiteral("Process"),
                                                                            {QStringLiteral("list"),
                                                                             QStringLiteral("startDetached"),
                                                                             QStringLiteral("thisProcess")},
                                                                            scriptEngine);
    }

    void Process::onError(QProcess::ProcessError processError)
	{
        if(!mOnError.isUndefined())
            mOnError.call(QJSValueList() << static_cast<ProcessError>(processError));
	}

	void Process::finished(int exitCode, QProcess::ExitStatus exitStatus)
	{
        if(!mOnFinished.isUndefined())
            mOnFinished.call(QJSValueList() << exitCode << static_cast<ExitStatus>(exitStatus));
	}

	void Process::readyReadStandardError()
	{
        if(!mOnReadyReadStandardError.isUndefined())
            mOnReadyReadStandardError.call();
	}

	void Process::readyReadStandardOutput()
	{
        if(!mOnReadyReadStandardOutput.isUndefined())
            mOnReadyReadStandardOutput.call();
	}

	void Process::started()
	{
        if(!mOnStarted.isUndefined())
            mOnStarted.call();
	}

	void Process::stateChanged(QProcess::ProcessState newState)
	{
        if(!mOnStateChanged.isUndefined())
            mOnStateChanged.call(QJSValueList() << static_cast<ProcessState>(newState));
	}

    QJSValue StaticProcess::list()
    {
        QList<int> processesList = ActionTools::CrossPlatform::runningProcesses();

        QJSValue back = ActionTools::ScriptEngine::current()->newArray(processesList.count());

        for(int index = 0; index < processesList.count(); ++index)
            back.setProperty(index, CodeClass::construct<ProcessHandle>(processesList.at(index)));

        return back;
    }

    QJSValue StaticProcess::startDetached(const QString &filename)
    {
        return startDetached(filename, {}, QDir::currentPath());
    }

    QJSValue StaticProcess::startDetached(const QString &filename, const QStringList &parameters)
    {
        return startDetached(filename, parameters, QDir::currentPath());
    }

    QJSValue StaticProcess::startDetached(const QString &filename, const QStringList &parameters, const QString &workingDirectory)
    {
        if(filename.isEmpty())
        {
            throwError(QStringLiteral("FilenameError"), tr("Invalid filename"));
            return {};
        }

        qint64 processId;
        if(!QProcess::startDetached(filename, parameters, workingDirectory, &processId))
        {
            throwError(QStringLiteral("StartProcessError"), tr("Unable to start the process"));
            return {};
        }

        return CodeClass::construct<ProcessHandle>(processId);
    }

    QJSValue StaticProcess::thisProcess()
    {
#ifdef Q_OS_WIN
        return CodeClass::construct<ProcessHandle>(GetCurrentProcessId());
#else
        return CodeClass::construct<ProcessHandle>(getpid());
#endif
    }
}
