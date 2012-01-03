/*
	Actionaz
	Copyright (C) 2008-2012 Jonathan Mercier-Ganady

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

#include "process.h"
#include "code/processhandle.h"
#include "code/rawdata.h"

#include <QDir>
#include <QScriptValueIterator>

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef Q_WS_X11
#include <unistd.h>
#endif

namespace Code
{
	QScriptValue Process::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Process *process = new Process;

		QScriptValueIterator it(context->argument(0));

		while(it.hasNext())
		{
			it.next();

			if(it.name() == "workingDirectory")
				process->process()->setWorkingDirectory(it.value().toString());
			else if(it.name() == "processChannelMode")
				process->process()->setProcessChannelMode(static_cast<QProcess::ProcessChannelMode>(it.value().toInt32()));
			else if(it.name() == "readChannel")
				process->process()->setReadChannel(static_cast<QProcess::ProcessChannel>(it.value().toInt32()));
			else if(it.name() == "onError")
				process->mOnError = it.value();
			else if(it.name() == "onFinished")
				process->mOnFinished = it.value();
			else if(it.name() == "onReadyReadStandardError")
				process->mOnReadyReadStandardError = it.value();
			else if(it.name() == "onReadyReadStandardOutput")
				process->mOnReadyReadStandardOutput = it.value();
			else if(it.name() == "onStarted")
				process->mOnStarted = it.value();
			else if(it.name() == "onStateChanged")
				process->mOnStateChanged = it.value();
		}

		return CodeClass::constructor(process, context, engine);
	}

	QScriptValue Process::list(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)

		QList<int> processesList = ActionTools::CrossPlatform::runningProcesses();

		QScriptValue back = engine->newArray(processesList.count());

		for(int index = 0; index < processesList.count(); ++index)
			back.setProperty(index, ProcessHandle::constructor(processesList.at(index), engine));

		return back;
	}

	QScriptValue Process::startDetached(QScriptContext *context, QScriptEngine *engine)
	{
		QString filename = context->argument(0).toString();
		if(filename.isEmpty())
		{
			throwError(context, engine, "FilenameError", tr("Invalid filename"));
			return engine->undefinedValue();
		}

		QStringList parameters;
		if(context->argumentCount() > 1)
		{
			const QScriptValue &parametersScriptValue = context->argument(1);

			if(parametersScriptValue.isArray())
				parameters = arrayParameterToStringList(parametersScriptValue);
			else
				parameters.append(parametersScriptValue.toString());
		}

		QString workingDirectory = QDir::currentPath();
		if(context->argumentCount() > 2)
			workingDirectory = context->argument(2).toString();

		qint64 processId;
		if(!QProcess::startDetached(filename, parameters, workingDirectory, &processId))
		{
			throwError(context, engine, "StartProcessError", tr("Unable to start the process"));
			return engine->undefinedValue();
		}

		return ProcessHandle::constructor(processId, engine);
	}

	QScriptValue Process::thisProcess(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)

#ifdef Q_WS_WIN
		return ProcessHandle::constructor(GetCurrentProcessId(), engine);
#else
		return ProcessHandle::constructor(getpid(), engine);
#endif
	}

	Process::Process()
		: CodeClass(),
		mProcess(new QProcess(this))
	{
		connect(mProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(error(QProcess::ProcessError)));
		connect(mProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
		connect(mProcess, SIGNAL(readyReadStandardError()), SLOT(readyReadStandardError()));
		connect(mProcess, SIGNAL(readyReadStandardOutput()), SLOT(readyReadStandardOutput()));
		connect(mProcess, SIGNAL(started()), SLOT(started()));
		connect(mProcess, SIGNAL(stateChanged(QProcess::ProcessState)), SLOT(stateChanged(QProcess::ProcessState)));
	}

	QScriptValue Process::handle() const
	{
		return ProcessHandle::constructor(id(), engine());
	}

	int Process::id() const
	{
#ifdef Q_WS_WIN
            return mProcess->pid()->dwProcessId;
#else
            return mProcess->pid();
#endif
	}

	QScriptValue Process::start()
	{
		QString filename = context()->argument(0).toString();
		if(filename.isEmpty())
		{
			throwError("FilenameError", tr("Invalid filename"));
			return engine()->undefinedValue();
		}

		QStringList parameters;
		if(context()->argumentCount() > 1)
		{
			const QScriptValue &parametersScriptValue = context()->argument(1);

			if(parametersScriptValue.isArray())
				parameters = arrayParameterToStringList(parametersScriptValue);
			else
				parameters.append(parametersScriptValue.toString());
		}

		QIODevice::OpenMode openMode = QIODevice::ReadWrite;
		if(context()->argumentCount() > 2)
			openMode = static_cast<QIODevice::OpenMode>(context()->argument(2).toInt32());

		mProcess->start(filename, parameters, openMode);

		return thisObject();
	}

	QProcess::ProcessState Process::state() const
	{
		return mProcess->state();
	}

	QProcess::ProcessError Process::error() const
	{
		return mProcess->error();
	}

	int Process::exitCode() const
	{
		return mProcess->exitCode();
	}

	QProcess::ExitStatus Process::exitStatus() const
	{
		return mProcess->exitStatus();
	}

	QScriptValue Process::readError() const
	{
		return RawData::constructor(mProcess->readAllStandardError(), engine());
	}

	QScriptValue Process::read() const
	{
		return RawData::constructor(mProcess->readAllStandardOutput(), engine());
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

	QScriptValue Process::write(const QScriptValue &data)
	{
		QObject *object = data.toQObject();
		if(RawData *rawData = qobject_cast<RawData*>(object))
		{
			if(mProcess->write(rawData->byteArray()) == -1)
				throwError("WriteError", tr("Write failed"));
		}
		else
		{
			if(mProcess->write(data.toVariant().toByteArray()) == -1)
				throwError("WriteError", tr("Write failed"));
		}

		return thisObject();
	}

	QScriptValue Process::writeText(const QString &data, Encoding encoding)
	{
		if(mProcess->write(toEncoding(data, encoding)) == -1)
			throwError("WriteError", tr("Write failed"));

		return thisObject();
	}

	QScriptValue Process::setWorkingDirectory(const QString &workingDirectory)
	{
		mProcess->setWorkingDirectory(workingDirectory);

		return thisObject();
	}

	QScriptValue Process::setProcessChannelMode(QProcess::ProcessChannelMode channelMode)
	{
		mProcess->setProcessChannelMode(channelMode);

		return thisObject();
	}

	QScriptValue Process::setEnvironment()
	{
		QProcessEnvironment environment;
		QScriptValueIterator it(context()->argument(0));

		while(it.hasNext())
		{
			it.next();

			environment.insert(it.name(), it.value().toString());
		}

		mProcess->setProcessEnvironment(environment);

		return thisObject();
	}

	QScriptValue Process::updateEnvironment()
	{
		QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
		QScriptValueIterator it(context()->argument(0));

		while(it.hasNext())
		{
			it.next();

			environment.insert(it.name(), it.value().toString());
		}

		mProcess->setProcessEnvironment(environment);

		return thisObject();
	}

	QScriptValue Process::setReadChannel(QProcess::ProcessChannel channel)
	{
		mProcess->setReadChannel(channel);

		return thisObject();
	}

	QScriptValue Process::setStandardErrorFile(const QString &fileName, QIODevice::OpenMode openMode)
	{
		mProcess->setStandardErrorFile(fileName, openMode);

		return thisObject();
	}

	QScriptValue Process::setStandardInputFile(const QString &fileName)
	{
		mProcess->setStandardInputFile(fileName);

		return thisObject();
	}

	QScriptValue Process::setStandardOutputFile(const QString &fileName, QIODevice::OpenMode openMode)
	{
		mProcess->setStandardOutputFile(fileName, openMode);

		return thisObject();
	}

	QScriptValue Process::setStandardOutputProcess(const QScriptValue &processValue)
	{
		Process *otherProcess = qobject_cast<Process *>(processValue.toQObject());
		if(!otherProcess)
		{
			throwError("InvalidProcessError", tr("Invalid process"));
			return thisObject();
		}

		mProcess->setStandardOutputProcess(otherProcess->process());

		return thisObject();
	}

	QScriptValue Process::waitForFinished(int waitTime)
	{
		if(!mProcess->waitForFinished(waitTime))
			throwError("WaitForFinishedError", tr("Wait for finished failed"));

		return thisObject();
	}

	QScriptValue Process::waitForStarted(int waitTime)
	{
		if(!mProcess->waitForFinished(waitTime))
			throwError("WaitForStartedError", tr("Wait for started failed"));

		return thisObject();
	}

	QScriptValue Process::waitForBytesWritten(int waitTime)
	{
		if(!mProcess->waitForReadyRead(waitTime))
			throwError("WaitForBytesWrittenError", tr("Waiting for bytes written failed"));

		return thisObject();
	}

	QScriptValue Process::waitForReadyRead(int waitTime)
	{
		if(!mProcess->waitForReadyRead(waitTime))
			throwError("WaitForReadyReadError", tr("Waiting for ready read failed"));

		return thisObject();
	}

	QScriptValue Process::close()
	{
		mProcess->close();

		return thisObject();
	}

	QScriptValue Process::kill()
	{
		mProcess->kill();

		return thisObject();
	}

	QScriptValue Process::terminate()
	{
		mProcess->terminate();

		return thisObject();
	}

	void Process::error(QProcess::ProcessError processError)
	{
		if(mOnError.isValid())
			mOnError.call(thisObject(), QScriptValueList() << processError);
	}

	void Process::finished(int exitCode, QProcess::ExitStatus exitStatus)
	{
		if(mOnFinished.isValid())
			mOnFinished.call(thisObject(), QScriptValueList() << exitCode << exitStatus);
	}

	void Process::readyReadStandardError()
	{
		if(mOnReadyReadStandardError.isValid())
			mOnReadyReadStandardError.call(thisObject());
	}

	void Process::readyReadStandardOutput()
	{
		if(mOnReadyReadStandardOutput.isValid())
			mOnReadyReadStandardOutput.call(thisObject());
	}

	void Process::started()
	{
		if(mOnStarted.isValid())
			mOnStarted.call(thisObject());
	}

	void Process::stateChanged(QProcess::ProcessState newState)
	{
		if(mOnStateChanged.isValid())
			mOnStateChanged.call(thisObject(), QScriptValueList() << newState);
	}
}
