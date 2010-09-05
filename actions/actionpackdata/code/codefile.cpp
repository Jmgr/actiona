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
	bool recursive = true;
	
	while(it.hasNext())
	{
		it.next();

		if(it.name() == "createDestinationDirectory")
			createDestinationDirectory = it.value().toBool();
		else if(it.name() == "recursive")
			recursive = it.value().toBool();
	}
	
#ifdef Q_WS_X11
	source.replace(" ", "\\ ");
	destination.replace(" ", "\\ ");
	
	QDir destinationDir(destination);
	if(!destinationDir.exists())
	{
		if(createDestinationDirectory)
		{
			if(QProcess::execute(QString("sh -c \"mkdir -p %1\"").arg(destination)))
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
	
	QString command = "sh -c \"cp -f";
	
	if(recursive)
		command += " -r";
	
	command += " ";
	command += source;
	command += " ";
	command += destination;
	command += "\"";

	if(QProcess::execute(command))
	{
		context()->throwError(tr("Copy failed"));
		return QScriptValue();
	}
#endif
#ifdef Q_WS_WIN
	//TODO
#endif
	
	return QScriptValue();
	
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
	source.replace(" ", "\\ ");
	destination.replace(" ", "\\ ");
	
	QDir destinationDir(destination);
	if(!destinationDir.exists())
	{
		if(createDestinationDirectory)
		{
			if(QProcess::execute(QString("sh -c \"mkdir -p %1\"").arg(destination)))
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
	command += source;
	command += " ";
	command += destination;
	command += "\"";

	if(QProcess::execute(command))
	{
		context()->throwError(tr("Move/rename failed"));
		return QScriptValue();
	}
#endif
#ifdef Q_WS_WIN
	//TODO
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

QScriptValue CodeFile::remove(QString file)
{
#ifdef Q_WS_X11
	file.replace(" ", "\\ ");
	
	QString command = "sh -c \"rm -fr";
	
	command += " ";
	command += file;
	command += "\"";

	if(QProcess::execute(command))
	{
		context()->throwError(tr("Remove failed"));
		return QScriptValue();
	}
#endif
#ifdef Q_WS_WIN
	//TODO
#endif
	
	return QScriptValue();
	
	return context()->thisObject();
}

QScriptValue CodeFile::remove()
{
	mFile.close();
	
	return remove(mFile.fileName());
}
