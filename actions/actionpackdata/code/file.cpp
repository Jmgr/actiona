/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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

#include "file.h"
#include "code/rawdata.h"

#include <QScriptValueIterator>
#include <QProcess>
#include <QDir>

namespace Code
{
	QScriptValue File::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		return CodeClass::constructor(new File, context, engine);
	}

	QScriptValue File::copy(QScriptContext *context, QScriptEngine *engine)
	{
		QString source, destination;
		bool createDestinationDirectory;

		if(getParameters(source, destination, createDestinationDirectory, context, engine))
			copyPrivate(source, destination, createDestinationDirectory, context, engine);

		return engine->undefinedValue();
	}

	QScriptValue File::move(QScriptContext *context, QScriptEngine *engine)
	{
		QString source, destination;
		bool createDestinationDirectory;

		if(getParameters(source, destination, createDestinationDirectory, context, engine))
			movePrivate(source, destination, createDestinationDirectory, context, engine);

		return engine->undefinedValue();
	}

	QScriptValue File::rename(QScriptContext *context, QScriptEngine *engine)
	{
		QString source, destination;
		bool createDestinationDirectory;

		if(getParameters(source, destination, createDestinationDirectory, context, engine))
			movePrivate(source, destination, createDestinationDirectory, context, engine);

		return engine->undefinedValue();
	}

	QScriptValue File::remove(QScriptContext *context, QScriptEngine *engine)
	{
		QString filename;

		if(context->argumentCount() < 1)
		{
			throwError(context, engine, "ParameterCountError", tr("Incorrect parameter count"));
			return false;
		}

		filename = context->argument(0).toString();

		removePrivate(filename, context, engine);

		return engine->undefinedValue();
	}

	void File::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&copy, "copy", scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&move, "move", scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&rename, "rename", scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&remove, "remove", scriptEngine);
	}
	
	QScriptValue File::open(const QString &filename, OpenMode mode)
	{
		mFile.setFileName(filename);
	
		if(!mFile.open(static_cast<QIODevice::OpenMode>(mode)))
			throwError("CannotOpenFileError", tr("Unable to open file"));
	
		return context()->thisObject();
	}
	
	QScriptValue File::write(const QScriptValue &data)
	{
		QObject *object = data.toQObject();
		if(RawData *rawData = qobject_cast<RawData*>(object))
		{
			if(mFile.write(rawData->byteArray()) == -1)
				throwError("WriteFailedError", tr("Write failed"));
		}
		else
		{
			if(mFile.write(data.toVariant().toByteArray()) == -1)
				throwError("WriteFailedError", tr("Write failed"));
		}
	
		return context()->thisObject();
	}
	
	QScriptValue File::writeText(const QString &value, Encoding encoding)
	{
		if(mFile.write(toEncoding(value, encoding)) == -1)
			throwError("WriteFailedError", tr("Write failed"));
	
		return context()->thisObject();
	}
	
	QScriptValue File::read()
	{
		return RawData::constructor(mFile.readAll(), context(), engine());
	}
	
	QString File::readText(Encoding encoding)
	{
		return fromEncoding(mFile.readAll(), encoding);
	}
	
	QScriptValue File::close()
	{
		mFile.close();
	
		return context()->thisObject();
	}
	
	QScriptValue File::copy(const QString &destination, bool createDestinationDirectory) const
	{
		return copyPrivate(mFile.fileName(), destination, createDestinationDirectory, context(), engine());
	}
	
	QScriptValue File::move(const QString &destination, bool createDestinationDirectory)
	{
		mFile.close();
	
		return movePrivate(mFile.fileName(), destination, createDestinationDirectory, context(), engine());
	}
	
	QScriptValue File::rename(const QString &destination, bool createDestinationDirectory)
	{
		return movePrivate(mFile.fileName(), destination, createDestinationDirectory, context(), engine());
	}
	
	QScriptValue File::remove()
	{
		mFile.close();
	
		return removePrivate(mFile.fileName(), context(), engine());
	}

	bool File::getParameters(QString &source, QString &destination, bool &createDestinationDirectory, QScriptContext *context, QScriptEngine *engine)
	{
		if(context->argumentCount() < 2)
		{
			throwError(context, engine, "ParameterCountError", tr("Incorrect parameter count"));
			return false;
		}

		source = context->argument(0).toString();
		destination = context->argument(1).toString();
		createDestinationDirectory = true;

		if(context->argumentCount() >= 3)
			createDestinationDirectory = context->argument(2).toBool();

		return true;
	}

	QScriptValue File::copyPrivate(const QString &source, const QString &destination, bool createDestinationDirectory, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

#ifdef Q_WS_X11
		QDir destinationDir(destination);
		QString sourceCopy(source);
		QString destinationCopy(destination);

		sourceCopy.replace(" ", "\\ ");
		destinationCopy.replace(" ", "\\ ");

		if(!destinationDir.exists())
		{
			if(createDestinationDirectory)
			{
				if(QProcess::execute("sh -c \"mkdir -p " + QFile::encodeName(destinationCopy) + "\""))
				{
					throwError(context, engine, "DirectoryCreationError", tr("Unable to create destination directory"));
					return context->thisObject();
				}
			}
			else
			{
				throwError(context, engine, "DirectoryDoesntExistError", tr("Destination directory doesn't exist"));
				return context->thisObject();
			}
		}

		QString command = "sh -c \"cp -fr";

		command += " ";
		command += QFile::encodeName(sourceCopy);
		command += " ";
		command += QFile::encodeName(destinationCopy);
		command += "\"";

		if(QProcess::execute(command))
		{
			throwError(context, engine, "CopyError", tr("Copy failed"));
			return context->thisObject();
		}
#endif
#ifdef Q_WS_WIN
		QDir destinationDir(destination);
		if(!destinationDir.exists())
		{
			if(createDestinationDirectory)
			{
				if(QProcess::execute("cmd", QStringList() << "/c" << "mkdir" << QFile::encodeName(destination)))
				{
					throwError(context, engine, "DirectoryCreationError", tr("Unable to create destination directory"));
					return context->thisObject();
				}
			}
			else
			{
				throwError(context, engine, "DirectoryDoesntExistError", tr("Destination directory doesn't exist"));
				return context->thisObject();
			}
		}

		QStringList args = QStringList() << "/c" << "xcopy" << QFile::encodeName(source) << QFile::encodeName(destination) << "/i /y /e /r /k /a /q /h /c /m /x";

		if(QProcess::execute("cmd", args) > 1)
			throwError(context, engine, "CopyError", tr("Copy failed"));
#endif

		return context->thisObject();
	}

	QScriptValue File::movePrivate(const QString &source, const QString &destination, bool createDestinationDirectory, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

#ifdef Q_WS_X11
		QDir destinationDir(destination);
		QString sourceCopy(source);
		QString destinationCopy(destination);

		sourceCopy.replace(" ", "\\ ");
		destinationCopy.replace(" ", "\\ ");

		if(!destinationDir.exists())
		{
			if(createDestinationDirectory)
			{
				if(QProcess::execute("sh -c \"mkdir -p " + QFile::encodeName(destinationCopy) + "\""))
				{
					throwError(context, engine, "DirectoryCreationError", tr("Unable to create destination directory"));
					return context->thisObject();
				}
			}
			else
			{
				throwError(context, engine, "DirectoryDoesntExistError", tr("Destination directory doesn't exist"));
				return context->thisObject();
			}
		}

		QString command = "sh -c \"mv -f";

		command += " ";
		command += QFile::encodeName(sourceCopy);
		command += " ";
		command += QFile::encodeName(destinationCopy);
		command += "\"";

		if(QProcess::execute(command))
		{
			throwError(context, engine, "MoveRenameError", tr("Move/rename failed"));
			return context->thisObject();
		}
#endif
#ifdef Q_WS_WIN
		Q_UNUSED(createDestinationDirectory)

		QStringList args = QStringList() << "/c" << "move /y" << QFile::encodeName(source) << QFile::encodeName(destination);

		if(QProcess::execute("cmd", args))
			throwError(context, engine, "MoveRenameError", tr("Move/rename failed"));
#endif

		return context->thisObject();
	}

	QScriptValue File::removePrivate(const QString &filename, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

#ifdef Q_WS_X11
		QString filenameCopy(filename);
		filenameCopy.replace(" ", "\\ ");

		QString command = "sh -c \"rm -fr";

		command += " ";
		command += QFile::encodeName(filenameCopy);
		command += "\"";

		if(QProcess::execute(command))
		{
			throwError(context, engine, "RemoveError", tr("Remove failed"));
			return context->thisObject();
		}
#endif
#ifdef Q_WS_WIN
		QProcess funkyWindowsCommand;

		QStringList args = QStringList() << "/c" << "del /q" << QFile::encodeName(filename) << "/f /s /q /as /ah /ar /aa";
		//Yes, the /q option has to be *before* AND *after* the filename :D

		funkyWindowsCommand.start("cmd", args);
		funkyWindowsCommand.waitForStarted();
		funkyWindowsCommand.waitForFinished();

		args = QStringList() << "/c" << "rmdir /s /q" << QFile::encodeName(filename);

		funkyWindowsCommand.start("cmd", args);//Yes, we have to run it *two* times...
		funkyWindowsCommand.waitForStarted();
		funkyWindowsCommand.waitForFinished();

		funkyWindowsCommand.start("cmd", args);
		funkyWindowsCommand.waitForStarted();
		funkyWindowsCommand.waitForFinished();
#endif

		return context->thisObject();
	}
}
