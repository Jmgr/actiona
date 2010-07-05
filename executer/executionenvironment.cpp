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

#include "executionenvironment.h"

#include <QDesktopServices>
#include <QDesktopWidget>
#include <QApplication>
#include <QUrl>
#include <QScriptEngine>
#include <QDir>
#include <cstdlib>

#ifdef Q_WS_WIN
#include <windows.h>
#include <LMCons.h>
#endif

ExecutionEnvironment::ExecutionEnvironment(QObject *parent)
	: QObject(parent)
{
}

QString ExecutionEnvironment::storageLocationPath(StorageLocation location) const
{
	return QDesktopServices::storageLocation(static_cast<QDesktopServices::StandardLocation>(location));
}

QString ExecutionEnvironment::storageLocationName(StorageLocation location) const
{
	return QDesktopServices::displayName(static_cast<QDesktopServices::StandardLocation>(location));
}

void ExecutionEnvironment::openUrl(const QString &url) const
{
	QDesktopServices::openUrl(QUrl(url));
}

int ExecutionEnvironment::screenCount() const
{
	return QApplication::desktop()->screenCount();
}

QScriptValue ExecutionEnvironment::availableGeometry(int screen) const
{
	const QRect &geometry = QApplication::desktop()->availableGeometry(screen);
	QScriptValue value = engine()->newObject();
	value.setProperty("x", engine()->newVariant(geometry.x()));
	value.setProperty("y", engine()->newVariant(geometry.y()));
	value.setProperty("width", engine()->newVariant(geometry.width()));
	value.setProperty("height", engine()->newVariant(geometry.height()));

	return value;
}

QScriptValue ExecutionEnvironment::screenGeometry(int screen) const
{
	const QRect &geometry = QApplication::desktop()->screenGeometry(screen);
	QScriptValue value = engine()->newObject();
	value.setProperty("x", engine()->newVariant(geometry.x()));
	value.setProperty("y", engine()->newVariant(geometry.y()));
	value.setProperty("width", engine()->newVariant(geometry.width()));
	value.setProperty("height", engine()->newVariant(geometry.height()));

	return value;
}

int ExecutionEnvironment::primaryScreen() const
{
	return QApplication::desktop()->primaryScreen();
}

bool ExecutionEnvironment::isVirtualDesktop() const
{
	return QApplication::desktop()->isVirtualDesktop();
}

QString ExecutionEnvironment::currentDirectory() const
{
	return QDir::currentPath();
}

QString ExecutionEnvironment::username() const
{
#ifdef Q_WS_WIN
	TCHAR buffer[UNLEN+1];
	DWORD size = sizeof(buffer);
	GetUserName(buffer, &size);

	return QString::fromWCharArray(buffer);
#else
	return QString::fromAscii(std::getenv("USER"));
#endif
}
