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

#ifndef EXECUTIONENVIRONMENT_H
#define EXECUTIONENVIRONMENT_H

#include "executer_global.h"

#include <QObject>
#include <QScriptable>
#include <QScriptValue>

class EXECUTERSHARED_EXPORT ExecutionEnvironment : public QObject, protected QScriptable
{
    Q_OBJECT
	Q_ENUMS(StorageLocation)
	
public:
	enum StorageLocation
	{
		Desktop,
		Documents,
		Fonts,
		Applications,
		Music,
		Movies,
		Pictures,
		Temp,
		Home,
		Data,
		Cache
	};
	
    explicit ExecutionEnvironment(QObject *parent = 0);

public slots:
	QString storageLocationPath(StorageLocation location) const;
	QString storageLocationName(StorageLocation location) const;
	void openUrl(const QString &url) const;
	int screenCount() const;
	QScriptValue availableGeometry(int screen = -1) const;
	QScriptValue screenGeometry(int screen = -1) const;
	int primaryScreen() const;
	bool isVirtualDesktop() const;
	QString currentDirectory() const;
	QString username() const;
	QString variable(const QString &name) const;
	uint timestamp() const;
};

#endif // EXECUTIONENVIRONMENT_H
