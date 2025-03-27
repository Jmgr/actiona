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

#include "actiontools/code/codeclass.hpp"

#include <QJSValue>
#include <QFile>

namespace Code
{
	class File : public CodeClass
	{
        friend class StaticFile;

        Q_OBJECT
        Q_ENUM(Encoding)
	
	public:
		enum OpenMode
		{
			ReadOnly =		QIODevice::ReadOnly,
			WriteOnly =		QIODevice::WriteOnly,
			ReadWrite =		QIODevice::ReadWrite,
			Append =		QIODevice::Append,
			Truncate =		QIODevice::Truncate,
			Text =			QIODevice::Text,
			Unbuffered =	QIODevice::Unbuffered
		};
        Q_ENUM(OpenMode)
	
        Q_INVOKABLE File();
        ~File();

        QFile *file()                                           { return &mFile; }
	
        Q_INVOKABLE QString toString() const override                                { return QStringLiteral("File"); }
        Q_INVOKABLE File *open(const QString &filename, OpenMode mode);
        Q_INVOKABLE File *write(const QJSValue &data);
        Q_INVOKABLE File *writeText(const QString &value, Encoding encoding = Native);
        Q_INVOKABLE QJSValue read();
        Q_INVOKABLE QString readText(Encoding encoding = Native);
        Q_INVOKABLE File *close();
        Q_INVOKABLE File *copy(const QString &destination, const QJSValue &options = QJSValue());
        Q_INVOKABLE File *move(const QString &destination, const QJSValue &options = QJSValue());
        Q_INVOKABLE File *rename(const QString &destination, const QJSValue &options = QJSValue());
        Q_INVOKABLE File *remove(const QJSValue &options = QJSValue());

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
	
    private:
        static std::tuple<bool, bool, bool, bool, bool> getParameters(const QJSValue &options);
        static void copyPrivate(CodeClass *self, const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory);
        static void movePrivate(CodeClass *self, const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory);
        static void renamePrivate(CodeClass *self, const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory);
        static void removePrivate(CodeClass *self, const QString &filename, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo);
		static QString getErrorString(int error);

		QFile mFile;
	};

    class StaticFile : public CodeClass
    {
        Q_OBJECT

    public:
        StaticFile(QObject *parent): CodeClass(parent) {}

        Q_INVOKABLE QString toString() const override { return QStringLiteral("StaticFile"); }
        Q_INVOKABLE void copy(const QString &source, const QString &destination, const QJSValue &options);
        Q_INVOKABLE bool exists(const QString &filename);
        Q_INVOKABLE void move(const QString &source, const QString &destination, const QJSValue &options);
        Q_INVOKABLE void rename(const QString &source, const QString &destination, const QJSValue &options);
        Q_INVOKABLE void remove(const QString &filename, const QJSValue &options);
    };
}

