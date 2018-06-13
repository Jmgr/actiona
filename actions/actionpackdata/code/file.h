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

#pragma once

#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QFile>

namespace Code
{
	class File : public CodeClass
	{
		Q_OBJECT
		Q_ENUMS(OpenMode)
		Q_ENUMS(Encoding)
	
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
	
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue copy(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue exists(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue move(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue rename(QScriptContext *context, QScriptEngine *engine);
		static QScriptValue remove(QScriptContext *context, QScriptEngine *engine);

		static void registerClass(QScriptEngine *scriptEngine);
	
        ~File() override                                                 { mFile.close(); }

        QFile *file()                                           { return &mFile; }
	
	public slots:
		QString toString() const override                                { return QStringLiteral("File"); }
        bool equals(const QScriptValue &other) const override    { return defaultEqualsImplementation<File>(other); }
		QScriptValue open(const QString &filename, OpenMode mode);
		QScriptValue write(const QScriptValue &data);
		QScriptValue writeText(const QString &value, Encoding encoding = Native);
		QScriptValue read();
		QString readText(Encoding encoding = Native);
		QScriptValue close();
		QScriptValue copy(const QString &destination, const QScriptValue &options = QScriptValue()) const;
		QScriptValue move(const QString &destination, const QScriptValue &options = QScriptValue());
		QScriptValue rename(const QString &destination, const QScriptValue &options = QScriptValue());
		QScriptValue remove(const QScriptValue &options = QScriptValue());
	
	private:
		static bool getParameters(QString &source, QString &destination, const QScriptValue &options, bool &noErrorDialog, bool &noConfirmDialog, bool &noProgressDialog, bool &allowUndo, bool &createDestinationDirectory, QScriptContext *context, QScriptEngine *engine);
		static bool getParameters(const QScriptValue &options, bool &noErrorDialog, bool &noConfirmDialog, bool &noProgressDialog, bool &allowUndo, bool &createDestinationDirectory);
		static QScriptValue copyPrivate(const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory, QScriptContext *context, QScriptEngine *engine);
		static QScriptValue movePrivate(const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory, QScriptContext *context, QScriptEngine *engine);
		static QScriptValue renamePrivate(const QString &source, const QString &destination, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, bool createDestinationDirectory, QScriptContext *context, QScriptEngine *engine);
		static QScriptValue removePrivate(const QString &filename, bool noErrorDialog, bool noConfirmDialog, bool noProgressDialog, bool allowUndo, QScriptContext *context, QScriptEngine *engine);
		static QString getErrorString(int error);

		QFile mFile;
	};
}

