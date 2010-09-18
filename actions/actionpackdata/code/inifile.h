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

#ifndef IniFile_H
#define IniFile_H

#include "code/code.h"

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>

#include <config.h>

namespace Code
{
	class IniFile : public QObject, public QScriptable
	{
		Q_OBJECT
	
	public:
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
		
		IniFile();
	
	public slots:
		QScriptValue load(const QString &filename);
		QScriptValue save(const QString &filename = QString());
		QScriptValue clear();
		QScriptValue preserveDeletedData(bool preserve);
		QScriptValue setDelimiter(char delimiter);
		QScriptValue setCommentCharacter(char commentchar);
		QScriptValue setSection(const QString &sectionName, bool create = true);
		QScriptValue setEncoding(Code::Encoding encoding);
		QString sectionAt(int sectionIndex) const;
		QScriptValue deleteSection(const QString &sectionName);
		int sectionCount() const;
		bool keyExists(const QString &keyName) const;
		QString keyAt(int keyIndex) const;
		QString keyValue(const QString &keyName) const;
		QScriptValue setKeyValue(const QString &keyName, const QString &value);
		QScriptValue deleteKey(const QString &keyName);
		int keyCount() const;
	
	private:
		rude::Config mConfig;
		Code::Encoding mEncoding;
	};
}

#endif // IniFile_H
