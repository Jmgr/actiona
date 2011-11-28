/*
	Actionaz
	Copyright (C) 2008-2011 Jonathan Mercier-Ganady

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
#include "code/rect.h"
#include "../systemsession.h"

#include <QSystemInfo>
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
		return CodeClass::constructor(new System, context, engine);
	}

	System::System()
		: CodeClass(),
		mSystemSession(new SystemSession),
		mSystemInfo(new QSystemInfo(this)),
		mSystemStorageInfo(new QSystemStorageInfo(this)),
		mSystemDisplayInfo(new QSystemDisplayInfo(this)),
		mSystemDeviceInfo(new QSystemDeviceInfo(this))
	{
	}

	System::~System()
	{
		delete mSystemSession;
	}

	QString System::storageLocationPath(StorageLocation location) const
	{
		return QDesktopServices::storageLocation(static_cast<QDesktopServices::StandardLocation>(location));
	}

	QString System::storageLocationName(StorageLocation location) const
	{
		return QDesktopServices::displayName(static_cast<QDesktopServices::StandardLocation>(location));
	}

	QScriptValue System::openUrl(const QString &url) const
	{
		if(!QDesktopServices::openUrl(QUrl(url)))
			throwError("OpenUrlError", tr("Cannot open the url"));

		return thisObject();
	}

	int System::screenCount() const
	{
		return QApplication::desktop()->screenCount();
	}

	QScriptValue System::availableGeometry(int screen) const
	{
		return Rect::constructor(QApplication::desktop()->availableGeometry(screen), engine());
	}

	QScriptValue System::screenGeometry(int screen) const
	{
		return Rect::constructor(QApplication::desktop()->screenGeometry(screen), engine());
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

	QString System::osName() const
	{
#ifdef Q_WS_X11
		return "GNU/Linux";
#endif
#ifdef Q_WS_WIN
		return "Windows";
#endif
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

	QScriptValue System::logout(bool force) const
	{
		if(!mSystemSession->logout(force))
			throwError("LogoutError", tr("Logout failed"));

		return thisObject();
	}

	QScriptValue System::restart(bool force) const
	{
		if(!mSystemSession->restart(force))
			throwError("RestartError", tr("Restart failed"));

		return thisObject();
	}

	QScriptValue System::shutdown(bool force) const
	{
		if(!mSystemSession->shutdown(force))
			throwError("ShutdownError", tr("Shutdown failed"));

		return thisObject();
	}

	QScriptValue System::suspend(bool force) const
	{
		if(!mSystemSession->suspend(force))
			throwError("SuspendError", tr("Suspend failed"));

		return thisObject();
	}

	QScriptValue System::hibernate(bool force) const
	{
		if(!mSystemSession->hibernate(force))
			throwError("HibernateError", tr("Hibernate failed"));

		return thisObject();
	}

	QScriptValue System::lockScreen() const
	{
		if(!mSystemSession->lockScreen())
			throwError("LockScreenError", tr("Lock screen failed"));

		return thisObject();
	}

	QScriptValue System::startScreenSaver() const
	{
		if(!mSystemSession->startScreenSaver())
			throwError("StartScreenSaverError", tr("Start screen saver failed"));

		return thisObject();
	}
}
