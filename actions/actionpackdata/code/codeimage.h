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

#ifndef CODEIMAGE_H
#define CODEIMAGE_H

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>
#include <QImage>

class CodeImage : public QObject, public QScriptable
{
    Q_OBJECT
	
public:
    static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	static QScriptValue constructor(const QImage &image, QScriptContext *context, QScriptEngine *engine);
	
	CodeImage();
	CodeImage(const CodeImage &other);
	CodeImage(const QImage &image);
	
	CodeImage &operator=(CodeImage other);
	CodeImage &operator=(QImage image);
	
	void swap(CodeImage &other);
	void swap(QImage &image);
	
	QImage image() const;

public slots:
	QScriptValue setData(const QScriptValue &data);
	QScriptValue data() const;
	QScriptValue loadFromFile(const QString &filename);
	QScriptValue saveToFile(const QString &filename) const;

private:
	QImage mImage;
};

#endif // CODEIMAGE_H
