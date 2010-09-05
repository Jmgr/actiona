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

#ifndef CODEREGISTRY_H
#define CODEREGISTRY_H

#include <QObject>
#include <QScriptable>
#include <QScriptValue>
#include <QScriptEngine>
#include <QStringList>

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

class CodeRegistry : public QObject, public QScriptable
{
    Q_OBJECT
	Q_ENUMS(Key)
	
public:
	enum Key
	{
		ClassesRoot,
		CurrentConfig,
		CurrentUser,
		Users,
		LocalMachine
	};
	
    static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);

public slots:
	QScriptValue openKey(Key key, const QString &subKey) const;
	QScriptValue createKey(Key key, const QString &subKey) const;
	QScriptValue setValue(const QString &valueName, const QVariant &value) const;
	QVariant value(const QString &valueName = QString()) const;
	QStringList valueNames() const;
	QStringList keys() const;
	QScriptValue deleteKey(bool recursive = true) const;
	QScriptValue closeKey() const;
	
private:
#ifdef Q_WS_WIN
	HKEY mHKey;
#endif
};

#endif // CODEREGISTRY_H
