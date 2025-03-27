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

#include "file.hpp"
#include "actiontools/code/rawdata.hpp"

#include <QJSValueIterator>
#include <QProcess>
#include <QDir>
#include <QDebug>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <Shellapi.h>
#endif

namespace Code
{
    File::File()
        : CodeClass()
    {
    }

    File::~File()
    {
        mFile.close();
    }

    File *File::open(const QString &filename, OpenMode mode)
	{
		mFile.setFileName(filename);
	
		if(!mFile.open(static_cast<QIODevice::OpenMode>(mode)))
			throwError(QStringLiteral("CannotOpenFileError"), tr("Unable to open file"));
	
        return this;
	}
	
    File *File::write(const QJSValue &data)
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
	
        return this;
	}
	
    File *File::writeText(const QString &value, Encoding encoding)
	{
		if(mFile.write(toEncoding(value, encoding)) == -1)
			throwError(QStringLiteral("WriteFailedError"), tr("Write failed"));
	
        return this;
	}
	
	QJSValue File::read()
    {
        return CodeClass::construct<RawData>(mFile.readAll());
	}
	
	QString File::readText(Encoding encoding)
	{
		return fromEncoding(mFile.readAll(), encoding);
	}
	
    File *File::close()
	{
		mFile.close();
	
        return this;
	}
	
    File *File::copy(const QString &destination, const QJSValue &options)
	{
        auto [noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory] = File::getParameters(options);

        copyPrivate(this, mFile.fileName(), destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory);

        return this;
	}
	
    File *File::move(const QString &destination, const QJSValue &options)
	{
        mFile.close();

        auto [noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory] = File::getParameters(options);

        movePrivate(this, mFile.fileName(), destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory);

        return this;
	}
	
    File *File::rename(const QString &destination, const QJSValue &options)
	{
        auto [noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory] = File::getParameters(options);

        renamePrivate(this, mFile.fileName(), destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory);

        return this;
	}
	
    File *File::remove(const QJSValue &options)
	{
		mFile.close();

        auto [noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory] = File::getParameters(options);

        removePrivate(this, mFile.fileName(), noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo);

        return this;
	}

    void File::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<File*>("const File *");

        CodeClass::registerClassWithStaticFunctions<File, StaticFile>(QStringLiteral("File"), {QStringLiteral("copy"), QStringLiteral("move"), QStringLiteral("rename"), QStringLiteral("remove"), QStringLiteral("exists")}, scriptEngine);
    }

    std::tuple<bool, bool, bool, bool, bool> File::getParameters(const QJSValue &options)
	{
        QJSValueIterator it(options);

        bool noErrorDialog = false;
        bool noConfirmDialog = false;
        bool noProgressDialog = false;
        bool allowUndo = false;
        bool createDestinationDirectory = true;

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

        return {noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory};
	}

    void File::copyPrivate(CodeClass *self, const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory)
	{
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
                if(QProcess::execute(QStringLiteral("sh"), {QStringLiteral("-c \"mkdir -p %1\"").arg(QString::fromLocal8Bit(QFile::encodeName(destinationCopy)))}))
				{
                    self->throwError(QStringLiteral("DirectoryCreationError"), tr("Unable to create destination directory"));
                    return;
				}
			}
			else
			{
                self->throwError(QStringLiteral("DirectoryDoesntExistError"), tr("Destination directory doesn't exist"));
                return;
			}
		}

        if(QProcess::execute(QStringLiteral("sh"), {QStringLiteral("-c \"cp -fr %1 %2\"")
                             .arg(QString::fromLocal8Bit(QFile::encodeName(sourceCopy)))
                             .arg(QString::fromLocal8Bit(QFile::encodeName(destinationCopy)))
        }))
		{
            self->throwError(QStringLiteral("CopyError"), tr("Copy failed"));
            return;
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
            self->throwError(QStringLiteral("CopyError"), tr("Copy failed: %1").arg(getErrorString(result)));
            return;
		}

		if(shFileOpStruct.fAnyOperationsAborted)
		{
            self->throwError(QStringLiteral("CopyAbortedError"), tr("Copy failed: aborted"));
            return;
		}
#endif

        return;
	}

    void File::movePrivate(CodeClass *self, const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory)
	{
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
                if(QProcess::execute(QStringLiteral("sh"), {QStringLiteral("-c \"mkdir -p %1\"").arg(QString::fromLocal8Bit(QFile::encodeName(destinationCopy)))}))
                {
                    self->throwError(QStringLiteral("DirectoryCreationError"), tr("Unable to create destination directory"));
                    return;
				}
			}
			else
			{
                self->throwError(QStringLiteral("DirectoryDoesntExistError"), tr("Destination directory doesn't exist"));
                return;
			}
		}

        if(QProcess::execute(QStringLiteral("sh"), {QStringLiteral("-c \"mv -f %1 %2\"")
                             .arg(QString::fromLocal8Bit(QFile::encodeName(sourceCopy)))
                             .arg(QString::fromLocal8Bit(QFile::encodeName(destinationCopy)))
        }))
		{
            self->throwError(QStringLiteral("MoveRenameError"), tr("Move/rename failed"));
            return;
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
            self->throwError(QStringLiteral("MoveError"), tr("Move failed: %1").arg(getErrorString(result)));
            return;
		}

		if(shFileOpStruct.fAnyOperationsAborted)
		{
            self->throwError(QStringLiteral("MoveAbortedError"), tr("Move failed: aborted"));
            return;
		}
#endif
	}

    void File::renamePrivate(CodeClass *self, const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory)
	{
#ifdef Q_OS_UNIX
        movePrivate(self, source, destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory);
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
            self->throwError(QStringLiteral("RenameError"), tr("Rename failed: %1").arg(getErrorString(result)));
            return;
		}

		if(shFileOpStruct.fAnyOperationsAborted)
		{
            self->throwError(QStringLiteral("RenameAbortedError"), tr("Rename failed: aborted"));
            return;
		}
#endif
	}

    void File::removePrivate(CodeClass *self, const QString &filename, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo)
	{
#ifdef Q_OS_UNIX
		Q_UNUSED(noErrorDialog)
		Q_UNUSED(noConfirmDialog)
		Q_UNUSED(noProgressDialog)
		Q_UNUSED(allowUndo)

		QString filenameCopy(filename);
		filenameCopy.replace(QStringLiteral(" "), QStringLiteral("\\ "));

        if(QProcess::execute(QStringLiteral("sh"), {QStringLiteral("-c \"rm -fr %1\"").arg(QString::fromLocal8Bit(QFile::encodeName(filenameCopy)))}))
		{
            self->throwError(QStringLiteral("RemoveError"), tr("Remove failed"));
            return;
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
            self->throwError(QStringLiteral("RemoveError"), tr("Remove failed: %1").arg(getErrorString(result)));
            return;
		}

		if(shFileOpStruct.fAnyOperationsAborted)
		{
            self->throwError(QStringLiteral("RemoveAbortedError"), tr("Remove failed: aborted"));
            return;
		}
#endif

        return;
	}

	QString File::getErrorString(int error)
	{
#ifdef Q_OS_UNIX
		Q_UNUSED(error)

        return {};
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

    void StaticFile::copy(const QString &source, const QString &destination, const QJSValue &options)
    {
        auto [noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory] = File::getParameters(options);

        File::copyPrivate(this, source, destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory);
    }

    bool StaticFile::exists(const QString &filename)
    {
        return QFile::exists(filename);
    }

    void StaticFile::move(const QString &source, const QString &destination, const QJSValue &options)
    {
        auto [noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory] = File::getParameters(options);

        File::movePrivate(this, source, destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory);
    }

    void StaticFile::rename(const QString &source, const QString &destination, const QJSValue &options)
    {
        auto [noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory] = File::getParameters(options);

        File::renamePrivate(this, source, destination, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory);
    }

    void StaticFile::remove(const QString &filename, const QJSValue &options)
    {
        auto [noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo, createDestinationDirectory] = File::getParameters(options);

        File::removePrivate(this, filename, noErrorDialog, noConfirmDialog, noProgressDialog, allowUndo);
    }
}
