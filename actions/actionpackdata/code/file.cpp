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

#include "file.h"
#include "code/rawdata.h"

#include <QScriptValueIterator>
#include <QProcess>
#include <QDir>
#include <QDebug>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <Shellapi.h>
#endif

namespace Code
{
	QScriptValue File::constructor(QScriptContext *context, QScriptEngine *engine)
	{
			return CodeClass::constructor(new File, context, engine);
	}

	QScriptValue File::copy(QScriptContext *context, QScriptEngine *engine)
	{
		QString source, destination;
		bool noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory;

		if(getParameters(source, destination, context->argument(2), noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory, context, engine))
			copyPrivate(source, destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory, context, engine);

		return engine->undefinedValue();
	}

	QScriptValue File::exists(QScriptContext *context, QScriptEngine *engine)
	{
		if(context->argumentCount() != 1)
		{
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			return false;
		}

		return QFile::exists(context->argument(0).toString());
	}

	QScriptValue File::move(QScriptContext *context, QScriptEngine *engine)
	{
		QString source, destination;
		bool noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory;

		if(getParameters(source, destination, context->argument(2), noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory, context, engine))
			movePrivate(source, destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory, context, engine);

		return engine->undefinedValue();
	}

	QScriptValue File::rename(QScriptContext *context, QScriptEngine *engine)
	{
		QString source, destination;
		bool noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory;

		if(getParameters(source, destination, context->argument(2), noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory, context, engine))
			renamePrivate(source, destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory, context, engine);

		return engine->undefinedValue();
	}

	QScriptValue File::remove(QScriptContext *context, QScriptEngine *engine)
	{
		QString filename;

		if(context->argumentCount() < 1)
		{
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			return false;
		}

		filename = context->argument(0).toString();

		bool noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory;

		if(getParameters(context->argument(1), noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory))
			removePrivate(filename, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, context, engine);

		return engine->undefinedValue();
	}

	void File::registerClass(QScriptEngine *scriptEngine)
	{
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&copy, QStringLiteral("copy"), scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&move, QStringLiteral("move"), scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&rename, QStringLiteral("rename"), scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&remove, QStringLiteral("remove"), scriptEngine);
		CodeTools::addClassGlobalFunctionToScriptEngine<File>(&exists, QStringLiteral("exists"), scriptEngine);
	}

	QScriptValue File::open(const QString &filename, OpenMode mode)
	{
		mFile.setFileName(filename);
	
		if(!mFile.open(static_cast<QIODevice::OpenMode>(mode)))
			throwError(QStringLiteral("CannotOpenFileError"), tr("Unable to open file"));
	
		return thisObject();
	}
	
	QScriptValue File::write(const QScriptValue &data)
	{
		QObject *object = data.toQObject();
		if(auto rawData = qobject_cast<RawData*>(object))
		{
			if(mFile.write(rawData->byteArray()) == -1)
				throwError(QStringLiteral("WriteFailedError"), tr("Write failed"));
		}
		else
		{
			if(mFile.write(data.toVariant().toByteArray()) == -1)
				throwError(QStringLiteral("WriteFailedError"), tr("Write failed"));
		}
	
		return thisObject();
	}
	
	QScriptValue File::writeText(const QString &value, Encoding encoding)
	{
		if(mFile.write(toEncoding(value, encoding)) == -1)
			throwError(QStringLiteral("WriteFailedError"), tr("Write failed"));
	
		return thisObject();
	}
	
	QScriptValue File::read()
	{
		return RawData::constructor(mFile.readAll(), engine());
	}
	
	QString File::readText(Encoding encoding)
	{
		return fromEncoding(mFile.readAll(), encoding);
	}
	
	QScriptValue File::close()
	{
		mFile.close();
	
		return thisObject();
	}
	
	QScriptValue File::copy(const QString &destination, const QScriptValue &options) const
	{
		bool noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory;

		if(getParameters(options, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory))
			return copyPrivate(mFile.fileName(), destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory, context(), engine());
		else
			return false;
	}
	
	QScriptValue File::move(const QString &destination, const QScriptValue &options)
	{
		mFile.close();
	
		bool noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory;

		if(getParameters(options, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory))
			return movePrivate(mFile.fileName(), destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory, context(), engine());
		else
			return false;
	}
	
	QScriptValue File::rename(const QString &destination, const QScriptValue &options)
	{
		bool noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory;

		if(getParameters(options, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory))
			return renamePrivate(mFile.fileName(), destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory, context(), engine());
		else
			return false;
	}
	
	QScriptValue File::remove(const QScriptValue &options)
	{
		mFile.close();

		bool noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory;

		if(getParameters(options, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory))
			return removePrivate(mFile.fileName(), noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, context(), engine());
		else
			return false;
	}

	bool File::getParameters(QString &source, QString &destination, const QScriptValue &options, bool &noErrorDialog, bool &noConfirmDialog, bool &noProgressDialog, bool &allowUndo, bool &createDestinationDirectory, QScriptContext *context, QScriptEngine *engine)
	{
		if(context->argumentCount() < 2)
		{
			throwError(context, engine, QStringLiteral("ParameterCountError"), tr("Incorrect parameter count"));
			return false;
		}

		source = context->argument(0).toString();
		destination = context->argument(1).toString();

		return getParameters(options, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory);
	}

	bool File::getParameters(const QScriptValue &options, bool &noErrorDialog, bool &noConfirmDialog, bool &noProgressDialog, bool &allowUndo, bool &createDestinationDirectory)
	{
		QScriptValueIterator it(options);

		noErrorDialog = false;
		noConfirmDialog = false;
		noProgressDialog = false;
		allowUndo = false;
		createDestinationDirectory = true;

		while(it.hasNext())
		{
			it.next();

			if(it.name() == QLatin1String("noErrorDialog"))
				noErrorDialog = it.value().toBool();
			else if(it.name() == QLatin1String("noConfirmDialog"))
				noConfirmDialog = it.value().toBool();
			else if(it.name() == QLatin1String("noProgressDialog"))
				noProgressDialog = it.value().toBool();
			else if(it.name() == QLatin1String("allowUndo"))
				allowUndo = it.value().toBool();
			else if(it.name() == QLatin1String("createDestinationDirectory"))
				createDestinationDirectory = it.value().toBool();
		}

		return true;
	}

	QScriptValue File::copyPrivate(const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

#ifdef Q_OS_UNIX
		Q_UNUSED(noErrorDialog)
		Q_UNUSED(noConfirmDialog)
		Q_UNUSED(noProgressDialog)
		Q_UNUSED(allowUndo)

		QDir destinationDir(destination);
		QString sourceCopy(source);
		QString destinationCopy(destination);

		sourceCopy.replace(QStringLiteral(" "), QStringLiteral("\\ "));
		destinationCopy.replace(QStringLiteral(" "), QStringLiteral("\\ "));

		if(!destinationDir.exists())
		{
			if(createDestinationDirectory)
			{
				if(QProcess::execute(QStringLiteral("sh -c \"mkdir -p ") + QString::fromLocal8Bit(QFile::encodeName(destinationCopy)) + QStringLiteral("\"")))
				{
					throwError(context, engine, QStringLiteral("DirectoryCreationError"), tr("Unable to create destination directory"));
					return context->thisObject();
				}
			}
			else
			{
				throwError(context, engine, QStringLiteral("DirectoryDoesntExistError"), tr("Destination directory doesn't exist"));
				return context->thisObject();
			}
		}

		QString command = QStringLiteral("sh -c \"cp -fr");

		command += QStringLiteral(" ");
		command += QString::fromLocal8Bit(QFile::encodeName(sourceCopy));
		command += QStringLiteral(" ");
		command += QString::fromLocal8Bit(QFile::encodeName(destinationCopy));
		command += QStringLiteral("\"");

		if(QProcess::execute(command))
		{
			throwError(context, engine, QStringLiteral("CopyError"), tr("Copy failed"));
			return context->thisObject();
		}
#endif
#ifdef Q_OS_WIN
		Q_UNUSED(createDestinationDirectory)

		QDir sourceDir(source);
		QDir destinationDir(destination);

		std::wstring wideSource = QDir::toNativeSeparators(sourceDir.absolutePath()).toStdWString();
		wideSource += L'\0';

		std::wstring wideDestination = QDir::toNativeSeparators(destinationDir.absolutePath()).toStdWString();
		wideDestination += L'\0';

		SHFILEOPSTRUCT shFileOpStruct;
		shFileOpStruct.hwnd = 0;
		shFileOpStruct.wFunc = FO_COPY;
		shFileOpStruct.pFrom = wideSource.c_str();
		shFileOpStruct.pTo = wideDestination.c_str();
		shFileOpStruct.fFlags = 0;
		shFileOpStruct.fAnyOperationsAborted = false;
		shFileOpStruct.lpszProgressTitle = 0;
		shFileOpStruct.hNameMappings = 0;

		if(noErrorDialog)
			shFileOpStruct.fFlags |= FOF_NOERRORUI;
		if(noConfirmDialog)
			shFileOpStruct.fFlags |= (FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR);
		if(noProgressDialog)
			shFileOpStruct.fFlags |= FOF_SILENT;
		if(allowUndo)
			shFileOpStruct.fFlags |= FOF_ALLOWUNDO;

		int result = SHFileOperation(&shFileOpStruct);
		if(result != 0)
		{
			throwError(context, engine, QStringLiteral("CopyError"), tr("Copy failed: %1").arg(getErrorString(result)));
			return context->thisObject();
		}

		if(shFileOpStruct.fAnyOperationsAborted)
		{
			throwError(context, engine, QStringLiteral("CopyAbortedError"), tr("Copy failed: aborted"));
			return context->thisObject();
		}
#endif

		return context->thisObject();
	}

	QScriptValue File::movePrivate(const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

#ifdef Q_OS_UNIX
		Q_UNUSED(noErrorDialog)
		Q_UNUSED(noConfirmDialog)
		Q_UNUSED(noProgressDialog)
		Q_UNUSED(allowUndo)

		QDir destinationDir(destination);
		QString sourceCopy(source);
		QString destinationCopy(destination);

		sourceCopy.replace(QStringLiteral(" "), QStringLiteral("\\ "));
		destinationCopy.replace(QStringLiteral(" "), QStringLiteral("\\ "));

		if(!destinationDir.exists())
		{
			if(createDestinationDirectory)
			{
				if(QProcess::execute(QStringLiteral("sh -c \"mkdir -p ") + QString::fromLocal8Bit(QFile::encodeName(destinationCopy)) + QStringLiteral("\"")))
				{
					throwError(context, engine, QStringLiteral("DirectoryCreationError"), tr("Unable to create destination directory"));
					return context->thisObject();
				}
			}
			else
			{
				throwError(context, engine, QStringLiteral("DirectoryDoesntExistError"), tr("Destination directory doesn't exist"));
				return context->thisObject();
			}
		}

		QString command = QStringLiteral("sh -c \"mv -f");

		command += QStringLiteral(" ");
		command += QString::fromLocal8Bit(QFile::encodeName(sourceCopy));
		command += QStringLiteral(" ");
		command += QString::fromLocal8Bit(QFile::encodeName(destinationCopy));
		command += QStringLiteral("\"");

		if(QProcess::execute(command))
		{
			throwError(context, engine, QStringLiteral("MoveRenameError"), tr("Move/rename failed"));
			return context->thisObject();
		}
#endif
#ifdef Q_OS_WIN
		Q_UNUSED(createDestinationDirectory)

		QDir sourceDir(source);
		QDir destinationDir(destination);

		std::wstring wideSource = QDir::toNativeSeparators(sourceDir.absolutePath()).toStdWString();
		wideSource += L'\0';

		std::wstring wideDestination = QDir::toNativeSeparators(destinationDir.absolutePath()).toStdWString();
		wideDestination += L'\0';

		SHFILEOPSTRUCT shFileOpStruct;
		shFileOpStruct.hwnd = 0;
		shFileOpStruct.wFunc = FO_MOVE;
		shFileOpStruct.pFrom = wideSource.c_str();
		shFileOpStruct.pTo = wideDestination.c_str();
		shFileOpStruct.fFlags = 0;
		shFileOpStruct.fAnyOperationsAborted = false;
		shFileOpStruct.lpszProgressTitle = 0;
		shFileOpStruct.hNameMappings = 0;

		if(noErrorDialog)
			shFileOpStruct.fFlags |= FOF_NOERRORUI;
		if(noConfirmDialog)
			shFileOpStruct.fFlags |= (FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR);
		if(noProgressDialog)
			shFileOpStruct.fFlags |= FOF_SILENT;
		if(allowUndo)
			shFileOpStruct.fFlags |= FOF_ALLOWUNDO;

		int result = SHFileOperation(&shFileOpStruct);
		if(result != 0)
		{
			throwError(context, engine, QStringLiteral("MoveError"), tr("Move failed: %1").arg(getErrorString(result)));
			return context->thisObject();
		}

		if(shFileOpStruct.fAnyOperationsAborted)
		{
			throwError(context, engine, QStringLiteral("MoveAbortedError"), tr("Move failed: aborted"));
			return context->thisObject();
		}
#endif

		return context->thisObject();
	}

	QScriptValue File::renamePrivate(const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory, QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(engine)

#ifdef Q_OS_UNIX
		movePrivate(source, destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory, context, engine);
#endif
#ifdef Q_OS_WIN
		Q_UNUSED(createDestinationDirectory)

		QDir sourceDir(source);
		QDir destinationDir(destination);

		std::wstring wideSource = QDir::toNativeSeparators(sourceDir.absolutePath()).toStdWString();
		wideSource += L'\0';

		std::wstring wideDestination = QDir::toNativeSeparators(destinationDir.absolutePath()).toStdWString();
		wideDestination += L'\0';

		SHFILEOPSTRUCT shFileOpStruct;
		shFileOpStruct.hwnd = 0;
		shFileOpStruct.wFunc = FO_RENAME;
		shFileOpStruct.pFrom = wideSource.c_str();
		shFileOpStruct.pTo = wideDestination.c_str();
		shFileOpStruct.fFlags = 0;
		shFileOpStruct.fAnyOperationsAborted = false;
		shFileOpStruct.lpszProgressTitle = 0;
		shFileOpStruct.hNameMappings = 0;

		if(noErrorDialog)
			shFileOpStruct.fFlags |= FOF_NOERRORUI;
		if(noConfirmDialog)
			shFileOpStruct.fFlags |= (FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR);
		if(noProgressDialog)
			shFileOpStruct.fFlags |= FOF_SILENT;
		if(allowUndo)
			shFileOpStruct.fFlags |= FOF_ALLOWUNDO;

		int result = SHFileOperation(&shFileOpStruct);
		if(result != 0)
		{
			throwError(context, engine, QStringLiteral("RenameError"), tr("Rename failed: %1").arg(getErrorString(result)));
			return context->thisObject();
		}

		if(shFileOpStruct.fAnyOperationsAborted)
		{
			throwError(context, engine, QStringLiteral("RenameAbortedError"), tr("Rename failed: aborted"));
			return context->thisObject();
		}
#endif

		return context->thisObject();
	}

	QScriptValue File::removePrivate(const QString &filename, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, QScriptContext *context, QScriptEngine *engine)
	{
#ifdef Q_OS_UNIX
		Q_UNUSED(noErrorDialog)
		Q_UNUSED(noConfirmDialog)
		Q_UNUSED(noProgressDialog)
		Q_UNUSED(allowUndo)

		QString filenameCopy(filename);
		filenameCopy.replace(QStringLiteral(" "), QStringLiteral("\\ "));

		QString command = QStringLiteral("sh -c \"rm -fr");

		command += QStringLiteral(" ");
		command += QString::fromLocal8Bit(QFile::encodeName(filenameCopy));
		command += QStringLiteral("\"");

		if(QProcess::execute(command))
		{
			throwError(context, engine, QStringLiteral("RemoveError"), tr("Remove failed"));
			return context->thisObject();
		}
#endif
#ifdef Q_OS_WIN
		QDir filenameDir(filename);

		std::wstring wideFilename = QDir::toNativeSeparators(filenameDir.absolutePath()).toStdWString();
		wideFilename += L'\0';

		SHFILEOPSTRUCT shFileOpStruct;
		shFileOpStruct.hwnd = 0;
		shFileOpStruct.wFunc = FO_DELETE;
		shFileOpStruct.pFrom = wideFilename.c_str();
		shFileOpStruct.pTo = 0;
		shFileOpStruct.fFlags = 0;
		shFileOpStruct.fAnyOperationsAborted = false;
		shFileOpStruct.lpszProgressTitle = 0;
		shFileOpStruct.hNameMappings = 0;

		if(noErrorDialog)
			shFileOpStruct.fFlags |= FOF_NOERRORUI;
		if(noConfirmDialog)
			shFileOpStruct.fFlags |= (FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR);
		if(noProgressDialog)
			shFileOpStruct.fFlags |= FOF_SILENT;
		if(allowUndo)
			shFileOpStruct.fFlags |= FOF_ALLOWUNDO;

		int result = SHFileOperation(&shFileOpStruct);
		if(result != 0)
		{
			throwError(context, engine, QStringLiteral("RemoveError"), tr("Remove failed: %1").arg(getErrorString(result)));
			return context->thisObject();
		}

		if(shFileOpStruct.fAnyOperationsAborted)
		{
			throwError(context, engine, QStringLiteral("RemoveAbortedError"), tr("Remove failed: aborted"));
			return context->thisObject();
		}
#endif

		return context->thisObject();
	}

	QString File::getErrorString(int error)
	{
#ifdef Q_OS_UNIX
		Q_UNUSED(error)

		return QString();
#endif
#ifdef Q_OS_WIN
		switch(error)
		{
		case ERROR_SUCCESS:
			return QString();
		case ERROR_FILE_NOT_FOUND:
			return tr("File not found");
		case ERROR_PATH_NOT_FOUND:
			return tr("Path not found");
		case ERROR_ACCESS_DENIED:
			return tr("Access denied");
		case ERROR_SHARING_VIOLATION:
			return tr("This file is used by another process");
		case ERROR_DISK_FULL:
			return tr("The disk is full");
		case ERROR_FILE_EXISTS:
		case ERROR_ALREADY_EXISTS:
			return tr("The file already exists");
		case ERROR_INVALID_NAME:
			return tr("Invalid name");
		case ERROR_CANCELLED:
			return tr("Canceled");
		default:
			return tr("Unknown error (%1)").arg(error);
		}

#endif
	}
}
