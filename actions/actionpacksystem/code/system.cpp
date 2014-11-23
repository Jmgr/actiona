/*
	Actiona
	Copyright (C) 2008-2014 Jonathan Mercier-Ganady

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

#include "system.h"
#include "code/rect.h"
#include "../systemsession.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
#include <QDeviceInfo>
#include <QBatteryInfo>
#include <QStorageInfo>
#include <QLocale>
#else
#include <QSystemInfo>
#include <QSystemStorageInfo>
#include <QSystemDisplayInfo>
#include <QSystemDeviceInfo>
#endif
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QApplication>
#include <QUrl>
#include <QDir>
#include <QDateTime>
#include <cstdlib>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <LMCons.h>
#endif

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
QTM_USE_NAMESPACE
#endif

namespace Code
{
	QScriptValue System::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		return CodeClass::constructor(new System, context, engine);
	}

	System::System()
		: CodeClass(),
        mSystemSession(new SystemSession)
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        , mDeviceInfo(new QDeviceInfo(this)),
        mBatteryInfo(new QBatteryInfo(this)),
        mStorageInfo(new QStorageInfo(this))
#else
        , mSystemInfo(new QSystemInfo(this)),
		mSystemStorageInfo(new QSystemStorageInfo(this)),
		mSystemDisplayInfo(new QSystemDisplayInfo(this)),
		mSystemDeviceInfo(new QSystemDeviceInfo(this))
#endif
	{
	}

	System::~System()
	{
		delete mSystemSession;
	}

	QString System::storageLocationPath(StorageLocation location) const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return QStandardPaths::locate(static_cast<QStandardPaths::StandardLocation>(location), QString(), QStandardPaths::LocateDirectory);
#else
        return QDesktopServices::storageLocation(static_cast<QDesktopServices::StandardLocation>(location));
#endif
	}

	QString System::storageLocationName(StorageLocation location) const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return QStandardPaths::displayName(static_cast<QStandardPaths::StandardLocation>(location));
#else
        return QDesktopServices::displayName(static_cast<QDesktopServices::StandardLocation>(location));
#endif
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
    #ifdef Q_OS_WIN
		TCHAR buffer[UNLEN+1];
		DWORD size = sizeof(buffer);
		GetUserName(buffer, &size);

		return QString::fromWCharArray(buffer);
	#else
        return QString::fromLatin1(std::getenv("USER"));
	#endif
	}

	QString System::variable(const QString &name) const
	{
        return QString::fromLatin1(std::getenv(name.toLatin1()));
	}

	uint System::timestamp() const
	{
		return QDateTime::currentDateTime().toTime_t();
	}

	QString System::osName() const
	{
#ifdef Q_OS_LINUX
		return "GNU/Linux";
#endif
#ifdef Q_OS_WIN
		return "Windows";
#endif
	}

	QString System::version() const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return mDeviceInfo->version(QDeviceInfo::Os);
#else
        return mSystemInfo->version(QSystemInfo::Os);
#endif
	}

	QString System::countryCode() const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QString localeName = QLocale::system().name();
        QStringList localeParts = localeName.split(QChar('_'));

        if(localeParts.size() >= 2)
            return localeParts[1];
        else
            return QString();
#else
    return mSystemInfo->currentCountryCode();
#endif
	}

	QString System::language() const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QString localeName = QLocale::system().name();
        QStringList localeParts = localeName.split(QChar('_'));

        if(localeParts.size() >= 2)
            return localeParts[0];
        else
            return QString();
#else
        return mSystemInfo->currentLanguage();
#endif
	}

	QStringList System::logicalDrives() const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return mStorageInfo->allLogicalDrives();
#else
        return QSystemStorageInfo::logicalDrives();
#endif
	}

	qlonglong System::availableDiskSpace(const QString &drive) const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return mStorageInfo->availableDiskSpace(drive);
#else
        return mSystemStorageInfo->availableDiskSpace(drive);
#endif
	}

	qlonglong System::totalDiskSpace(const QString &drive) const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return mStorageInfo->totalDiskSpace(drive);
#else
        return mSystemStorageInfo->totalDiskSpace(drive);
#endif
	}

	System::DriveType System::driveType(const QString &drive) const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return static_cast<DriveType>(mStorageInfo->driveType(drive));
#else
        return static_cast<DriveType>(mSystemStorageInfo->typeForDrive(drive));
#endif
	}

	int System::colorDepth(int screenId) const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return 0;// Sadly, no easy way to get this information
#else
        int screen = (screenId == -1 ? QApplication::desktop()->primaryScreen() : screenId);
        return mSystemDisplayInfo->colorDepth(screen);
#endif
	}

	int System::displayBrightness(int screenId) const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return 0;// Sadly, no easy way to get this information
#else
        int screen = (screenId == -1 ? QApplication::desktop()->primaryScreen() : screenId);
        return mSystemDisplayInfo->displayBrightness(screen);
#endif
	}

	int System::batteryLevel() const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        if(mBatteryInfo->batteryCount() == 0)
            return -1;

        if(mBatteryInfo->remainingCapacity(0) == -1 || mBatteryInfo->maximumCapacity(0) <= 0)
            return -1;

        return (mBatteryInfo->remainingCapacity(0) * 100) / mBatteryInfo->maximumCapacity(0);
#else
        return mSystemDeviceInfo->batteryLevel();
#endif
	}

	System::PowerState System::powerState() const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        if(mBatteryInfo->batteryCount() == 0)
            return UnknownState;

        switch(mBatteryInfo->chargingState(0))
        {
        case QBatteryInfo::Discharging:
            return BatteryPower;
        case QBatteryInfo::Charging:
            return WallPowerChargingBattery;
        case QBatteryInfo::Full:
            return WallPower;
        default:
            return UnknownState;
        }
#else
        return static_cast<System::PowerState>(mSystemDeviceInfo->currentPowerState());
#endif
	}

	QString System::manufacturer() const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return mDeviceInfo->manufacturer();
#else
        return mSystemDeviceInfo->manufacturer();
#endif
	}

	QString System::model() const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return mDeviceInfo->model();
#else
        return mSystemDeviceInfo->model();
#endif
	}

	QString System::productName() const
	{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        return mDeviceInfo->productName();
#else
        return mSystemDeviceInfo->productName();
#endif
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
