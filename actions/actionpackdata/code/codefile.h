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

#ifndef CODEFILE_H
#define CODEFILE_H

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>
#include <QFile>

class CodeFile : public QObject, public QScriptable
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
	enum Encoding
	{
		Native,
		Ascii,
		Latin1,
		UTF8,
	};

	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

	~CodeFile()						{ mFile.close(); }

public slots:
	QScriptValue open(const QString &filename, OpenMode mode = static_cast<OpenMode>(ReadWrite | Append));
	QScriptValue write(const QScriptValue &value);
	QScriptValue writeText(const QString &value, Encoding encoding = Native);
	QScriptValue read();
	QScriptValue readText(Encoding encoding = Native);
	QScriptValue close();
	QScriptValue copy(QString source, QString destination, const QScriptValue &parameters = QScriptValue());
	QScriptValue copy(const QString &destination, const QScriptValue &parameters = QScriptValue());
	QScriptValue move(QString source, QString destination, const QScriptValue &parameters = QScriptValue());
	QScriptValue move(const QString &destination, const QScriptValue &parameters = QScriptValue());
	QScriptValue rename(QString source, QString destination, const QScriptValue &parameters = QScriptValue());
	QScriptValue rename(const QString &destination, const QScriptValue &parameters = QScriptValue());
	QScriptValue remove(QString filename);
	QScriptValue remove();

private:
	QFile mFile;
};

#endif // CODEFILE_H
