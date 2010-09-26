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

#include "system.h"

#include <QSystemInfo>
#include <QxtWindowSystem>
#include <QDesktopServices>
#include <QSystemStorageInfo>
#include <QSystemDisplayInfo>
#include <QSystemDeviceInfo>
#include <QDesktopWidget>
#include <QApplication>
#include <QUrl>
#include <QDir>
#include <QDateTime>
#include <cstdlib>

#ifdef Q_WS_WIN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <LMCons.h>
#endif

QTM_USE_NAMESPACE

namespace Code
{
	QScriptValue System::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		Q_UNUSED(context)

		return engine->newQObject(new System, QScriptEngine::ScriptOwnership);
	}

	System::System()
		: mSystemInfo(new QSystemInfo(this)),
		mSystemStorageInfo(new QSystemStorageInfo(this)),
		mSystemDisplayInfo(new QSystemDisplayInfo(this)),
		mSystemDeviceInfo(new QSystemDeviceInfo(this))
	{
	}

	QString System::storageLocationPath(StorageLocation location) const
	{
		return QDesktopServices::storageLocation(static_cast<QDesktopServices::StandardLocation>(location));
	}

	QString System::storageLocationName(StorageLocation location) const
	{
		return QDesktopServices::displayName(static_cast<QDesktopServices::StandardLocation>(location));
	}

	void System::openUrl(const QString &url) const
	{
		QDesktopServices::openUrl(QUrl(url));
	}

	int System::screenCount() const
	{
		return QApplication::desktop()->screenCount();
	}

	QScriptValue System::availableGeometry(int screen) const
	{
		const QRect &geometry = QApplication::desktop()->availableGeometry(screen);
		QScriptValue value = engine()->newObject();
		value.setProperty("x", engine()->newVariant(geometry.x()));
		value.setProperty("y", engine()->newVariant(geometry.y()));
		value.setProperty("width", engine()->newVariant(geometry.width()));
		value.setProperty("height", engine()->newVariant(geometry.height()));

		return value;
	}

	QScriptValue System::screenGeometry(int screen) const
	{
		const QRect &geometry = QApplication::desktop()->screenGeometry(screen);
		QScriptValue value = engine()->newObject();
		value.setProperty("x", engine()->newVariant(geometry.x()));
		value.setProperty("y", engine()->newVariant(geometry.y()));
		value.setProperty("width", engine()->newVariant(geometry.width()));
		value.setProperty("height", engine()->newVariant(geometry.height()));

		return value;
	}

	int System::primaryScreen() const
	{
		return QApplication::desktop()->primaryScreen();
	}

	bool System::isVirtualDesktop() const
	{
		return QApplication::desktop()->isVirtualDesktop();
	}

	QString System::currentDirectory() const
	{
		return QDir::currentPath();
	}

	QString System::username() const
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

	QString System::variable(const QString &name) const
	{
		return QString::fromAscii(std::getenv(name.toAscii()));
	}

	uint System::timestamp() const
	{
		return QDateTime::currentDateTime().toTime_t();
	}

	uint System::idleTime() const
	{
		return QxtWindowSystem::idleTime();
	}

	QString System::version() const
	{
		return mSystemInfo->version(QSystemInfo::Os);
	}

	QString System::countryCode() const
	{
		return mSystemInfo->currentCountryCode();
	}

	QString System::language() const
	{
		return mSystemInfo->currentLanguage();
	}

	QStringList System::logicalDrives() const
	{
		return QSystemStorageInfo::logicalDrives();
	}

	qlonglong System::availableDiskSpace(const QString &drive) const
	{
		return mSystemStorageInfo->availableDiskSpace(drive);
	}

	qlonglong System::totalDiskSpace(const QString &drive) const
	{
		return mSystemStorageInfo->totalDiskSpace(drive);
	}

	System::DriveType System::driveType(const QString &drive) const
	{
		return static_cast<DriveType>(mSystemStorageInfo->typeForDrive(drive));
	}

	int System::colorDepth(int screenId) const
	{
		int screen = (screenId == -1 ? QApplication::desktop()->primaryScreen() : screenId);
		return mSystemDisplayInfo->colorDepth(screen);
	}

	int System::displayBrightness(int screenId) const
	{
		int screen = (screenId == -1 ? QApplication::desktop()->primaryScreen() : screenId);
		return mSystemDisplayInfo->displayBrightness(screen);
	}

	int System::batteryLevel() const
	{
		return mSystemDeviceInfo->batteryLevel();
	}

	System::PowerState System::powerState() const
	{
		return static_cast<System::PowerState>(mSystemDeviceInfo->currentPowerState());
	}

	QString System::manufacturer() const
	{
		return mSystemDeviceInfo->manufacturer();
	}

	QString System::model() const
	{
		return mSystemDeviceInfo->model();
	}

	QString System::productName() const
	{
		return mSystemDeviceInfo->productName();
	}
}
