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

#include "codefile.h"

#include <QScriptValueIterator>
#include <QProcess>
#include <QDir>

QScriptValue CodeFile::constructor(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(context)

	return engine->newQObject(new CodeFile, QScriptEngine::ScriptOwnership);
}

QScriptValue CodeFile::open(const QString &filename, OpenMode mode)
{
	mFile.setFileName(filename);

	if(!mFile.open(static_cast<QIODevice::OpenMode>(mode)))
		context()->throwError(tr("Unable to open file"));

	return context()->thisObject();
}

QScriptValue CodeFile::write(const QScriptValue &value)
{
	if(mFile.write(value.toVariant().toByteArray()) == -1)
		context()->throwError(tr("Write failed"));

	return context()->thisObject();
}

QScriptValue CodeFile::writeText(const QString &value, Encoding encoding)
{
	QByteArray data;

	switch(encoding)
	{
	case Native:
		data = value.toLocal8Bit();
		break;
	case Ascii:
		data = value.toAscii();
		break;
	case Latin1:
		data = value.toLatin1();
		break;
	case UTF8:
		data = value.toUtf8();
		break;
	default:
		break;
	}

	if(mFile.write(data) == -1)
		context()->throwError(tr("Write failed"));

	return context()->thisObject();
}

QScriptValue CodeFile::read()
{
	return engine()->newVariant(mFile.readAll());
}

QScriptValue CodeFile::readText(Encoding encoding)
{
	QByteArray data = mFile.readAll();

	switch(encoding)
	{
	case Native:
		return engine()->newVariant(QString::fromLocal8Bit(data.data(), data.size()));
	case Ascii:
		return engine()->newVariant(QString::fromAscii(data.data(), data.size()));
	case Latin1:
		return engine()->newVariant(QString::fromLatin1(data.data(), data.size()));
	case UTF8:
		return engine()->newVariant(QString::fromUtf8(data.data(), data.size()));
	default:
		return QScriptValue();
	}
}

QScriptValue CodeFile::close()
{
	mFile.close();

	return context()->thisObject();
}

QScriptValue CodeFile::copy(QString source, QString destination, const QScriptValue &parameters)
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
			if(QProcess::execute(QString("sh -c \"mkdir -p %1\"").arg(QFile::encodeName(destination)))
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

	QString command = "sh -c \"cp -f";

	if(recursive)
		command += " -r";

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

QScriptValue CodeFile::copy(const QString &destination, const QScriptValue &parameters)
{
	return copy(mFile.fileName(), destination, parameters);
}

QScriptValue CodeFile::move(QString source, QString destination, const QScriptValue &parameters)
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
			if(QProcess::execute(QString("sh -c \"mkdir -p %1\"").arg(QFile::encodeName(destination)))
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

QScriptValue CodeFile::move(const QString &destination, const QScriptValue &parameters)
{
	mFile.close();

	return copy(mFile.fileName(), destination, parameters);
}

QScriptValue CodeFile::rename(QString source, QString destination, const QScriptValue &parameters)
{
	return move(source, destination, parameters);
}

QScriptValue CodeFile::rename(const QString &destination, const QScriptValue &parameters)
{
	return move(destination, parameters);
}

QScriptValue CodeFile::remove(QString filename)
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

	funkyWindowsCommand.start("cmd", args);//Yes, we have to run it *two* time...
	funkyWindowsCommand.waitForStarted();
	funkyWindowsCommand.waitForFinished();

	funkyWindowsCommand.start("cmd", args);
	funkyWindowsCommand.waitForStarted();
	funkyWindowsCommand.waitForFinished();
#endif

	return context()->thisObject();
}

QScriptValue CodeFile::remove()
{
	mFile.close();

	return remove(mFile.fileName());
}
