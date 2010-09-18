/*
	Actionaz
	Copyright (C) 2008-2010 Jonathan Mercier-Ganady

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
		Q_UNUSED(context)
	
		return engine->newQObject(new File, QScriptEngine::ScriptOwnership);
	}
	
	QScriptValue File::open(const QString &filename, OpenMode mode)
	{
		mFile.setFileName(filename);
	
		if(!mFile.open(static_cast<QIODevice::OpenMode>(mode)))
			context()->throwError(tr("Unable to open file"));
	
		return context()->thisObject();
	}
	
	QScriptValue File::write(const QScriptValue &data)
	{
		QObject *object = data.toQObject();
		if(RawData *codeRawData = qobject_cast<RawData*>(object))
		{
			if(mFile.write(codeRawData->byteArray()) == -1)
				context()->throwError(tr("Write failed"));
		}
		else
		{
			if(mFile.write(data.toVariant().toByteArray()) == -1)
				context()->throwError(tr("Write failed"));
		}
	
		return context()->thisObject();
	}
	
	QScriptValue File::writeText(const QString &value, Code::Encoding encoding)
	{
		if(mFile.write(Code::toEncoding(value, encoding)) == -1)
			context()->throwError(tr("Write failed"));
	
		return context()->thisObject();
	}
	
	QScriptValue File::read()
	{
		return RawData::constructor(mFile.readAll(), context(), engine());
	}
	
	QScriptValue File::readText(Code::Encoding encoding)
	{
		return engine()->newVariant(Code::fromEncoding(mFile.readAll(), encoding));
	}
	
	QScriptValue File::close()
	{
		mFile.close();
	
		return context()->thisObject();
	}
	
	QScriptValue File::copy(QString source, QString destination, const QScriptValue &parameters) const
	{
		QScriptValueIterator it(parameters);
		bool createDestinationDirectory = true;
	
		while(it.hasNext())
		{
			it.next();
	
			if(it.name() == "createDestinationDirectory")
				createDestinationDirectory = it.value().toBool();
		}
	
	#ifdef Q_WS_X11
		QDir destinationDir(destination);
	
		source.replace(" ", "\\ ");
		destination.replace(" ", "\\ ");
	
		if(!destinationDir.exists())
		{
			if(createDestinationDirectory)
			{
				if(QProcess::execute("sh -c \"mkdir -p " + QFile::encodeName(destination) + "\""))
				{
					context()->throwError(tr("Unable to create destination directory"));
					return context()->thisObject();
				}
			}
			else
			{
				context()->throwError(tr("Destination directory doesn't exist"));
				return context()->thisObject();
			}
		}
	
		QString command = "sh -c \"cp -fr";
	
		command += " ";
		command += QFile::encodeName(source);
		command += " ";
		command += QFile::encodeName(destination);
		command += "\"";
	
		if(QProcess::execute(command))
		{
			context()->throwError(tr("Copy failed"));
			return context()->thisObject();
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
					context()->throwError(tr("Unable to create destination directory"));
					return QScriptValue();
				}
			}
			else
			{
				context()->throwError(tr("Destination directory doesn't exist"));
				return QScriptValue();
			}
		}
	
		QStringList args = QStringList() << "/c" << "xcopy" << QFile::encodeName(source) << QFile::encodeName(destination) << "/i /y /e /r /k /a /q /h /c /m /x";
	
		if(QProcess::execute("cmd", args) > 1)
			context()->throwError(tr("Copy failed"));
	#endif
	
		return context()->thisObject();
	}
	
	QScriptValue File::copy(const QString &destination, const QScriptValue &parameters) const
	{
		return copy(mFile.fileName(), destination, parameters);
	}
	
	QScriptValue File::move(QString source, QString destination, const QScriptValue &parameters)
	{
		QScriptValueIterator it(parameters);
		bool createDestinationDirectory = true;
	
		while(it.hasNext())
		{
			it.next();
	
			if(it.name() == "createDestinationDirectory")
				createDestinationDirectory = it.value().toBool();
		}
	
	#ifdef Q_WS_X11
		QDir destinationDir(destination);
	
		source.replace(" ", "\\ ");
		destination.replace(" ", "\\ ");
	
		if(!destinationDir.exists())
		{
			if(createDestinationDirectory)
			{
				if(QProcess::execute("sh -c \"mkdir -p " + QFile::encodeName(destination) + "\""))
				{
					context()->throwError(tr("Unable to create destination directory"));
					return QScriptValue();
				}
			}
			else
			{
				context()->throwError(tr("Destination directory doesn't exist"));
				return QScriptValue();
			}
		}
	
		QString command = "sh -c \"mv -f";
	
		command += " ";
		command += QFile::encodeName(source);
		command += " ";
		command += QFile::encodeName(destination);
		command += "\"";
	
		if(QProcess::execute(command))
		{
			context()->throwError(tr("Move/rename failed"));
			return QScriptValue();
		}
	#endif
	#ifdef Q_WS_WIN
		QStringList args = QStringList() << "/c" << "move /y" << QFile::encodeName(source) << QFile::encodeName(destination);
	
		if(QProcess::execute("cmd", args))
			context()->throwError(tr("Move/rename failed"));
	#endif
	
		return QScriptValue();
	
		return context()->thisObject();
	}
	
	QScriptValue File::move(const QString &destination, const QScriptValue &parameters)
	{
		mFile.close();
	
		return copy(mFile.fileName(), destination, parameters);
	}
	
	QScriptValue File::rename(QString source, QString destination, const QScriptValue &parameters)
	{
		return move(source, destination, parameters);
	}
	
	QScriptValue File::rename(const QString &destination, const QScriptValue &parameters)
	{
		return move(destination, parameters);
	}
	
	QScriptValue File::remove(QString filename) const
	{
	#ifdef Q_WS_X11
		filename.replace(" ", "\\ ");
	
		QString command = "sh -c \"rm -fr";
	
		command += " ";
		command += QFile::encodeName(filename);
		command += "\"";
	
		if(QProcess::execute(command))
		{
			context()->throwError(tr("Remove failed"));
			return context()->thisObject();
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
	
		return context()->thisObject();
	}
	
	QScriptValue File::remove()
	{
		mFile.close();
	
		return remove(mFile.fileName());
	}
}
