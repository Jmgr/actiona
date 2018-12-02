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

#include "system.h"
#include "code/rect.h"
#include "../systemsession.h"

#include <QStandardPaths>
#include "qdeviceinfo.h"
#include "qbatteryinfo.h"
#include "qstorageinfo.h"
#include <QLocale>
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

namespace Code
{
	QScriptValue System::constructor(QScriptContext *context, QScriptEngine *engine)
	{
		return CodeClass::constructor(new System, context, engine);
	}

	System::System()
		: CodeClass(),
        mSystemSession(new SystemSession)
        , mDeviceInfo(new QDeviceInfo(this)),
        mBatteryInfo(new QBatteryInfo(this)),
        mStorageInfo(new QStorageInfo_Custom(this))
	{
	}

	System::~System()
	{
		delete mSystemSession;
	}

	QString System::storageLocationPath(StorageLocation location) const
	{
        return QStandardPaths::locate(static_cast<QStandardPaths::StandardLocation>(location), QString(), QStandardPaths::LocateDirectory);
	}

	QString System::storageLocationName(StorageLocation location) const
	{
        return QStandardPaths::displayName(static_cast<QStandardPaths::StandardLocation>(location));
	}

	QScriptValue System::openUrl(const QString &url) const
	{
		if(!QDesktopServices::openUrl(QUrl(url)))
			throwError(QStringLiteral("OpenUrlError"), tr("Cannot open the url"));

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
		return QString::fromLatin1(std::getenv(name.toLatin1().constData()));
	}

	uint System::timestamp() const
	{
		return QDateTime::currentDateTime().toTime_t();
	}

	QString System::osName() const
	{
#if defined(Q_OS_LINUX)
		return QStringLiteral("GNU/Linux");
#elif defined(Q_OS_WIN)
		return QStringLiteral("Windows");
#else
        return QStringLiteral("Unknown");
#endif
	}

	QString System::version() const
	{
        return mDeviceInfo->version(QDeviceInfo::Os);
	}

	QString System::countryCode() const
	{
        QString localeName = QLocale::system().name();
		QStringList localeParts = localeName.split(QLatin1Char('_'));

        if(localeParts.size() >= 2)
            return localeParts[1];
        else
            return QString();
	}

	QString System::language() const
	{
        QString localeName = QLocale::system().name();
		QStringList localeParts = localeName.split(QLatin1Char('_'));

        if(localeParts.size() >= 2)
            return localeParts[0];
        else
            return QString();
	}

	QStringList System::logicalDrives() const
	{
        return mStorageInfo->allLogicalDrives();
	}

	qlonglong System::availableDiskSpace(const QString &drive) const
	{
        return mStorageInfo->availableDiskSpace(drive);
	}

	qlonglong System::totalDiskSpace(const QString &drive) const
	{
        return mStorageInfo->totalDiskSpace(drive);
	}

	System::DriveType System::driveType(const QString &drive) const
	{
        return static_cast<DriveType>(mStorageInfo->driveType(drive));
	}

	int System::colorDepth(int screenId) const
	{
        Q_UNUSED(screenId)

        return 0;// Sadly, no easy way to get this information

        //Qt 4: int screen = (screenId == -1 ? QApplication::desktop()->primaryScreen() : screenId);
        //Qt 4: return mSystemDisplayInfo->colorDepth(screen);
	}

	int System::displayBrightness(int screenId) const
	{
        Q_UNUSED(screenId)

        return 0;// Sadly, no easy way to get this information

        //Qt 4: int screen = (screenId == -1 ? QApplication::desktop()->primaryScreen() : screenId);
        //Qt 4: return mSystemDisplayInfo->displayBrightness(screen);
	}

	int System::batteryLevel() const
	{
        if(mBatteryInfo->batteryCount() == 0)
            return -1;

        if(mBatteryInfo->remainingCapacity() == -1 || mBatteryInfo->maximumCapacity() <= 0)
            return -1;

        return (mBatteryInfo->remainingCapacity() * 100) / mBatteryInfo->maximumCapacity();
	}

	System::PowerState System::powerState() const
	{
        if(mBatteryInfo->batteryCount() == 0)
            return UnknownState;

        switch(mBatteryInfo->chargingState())
        {
        case QBatteryInfo::Discharging:
            return BatteryPower;
        case QBatteryInfo::Charging:
            return WallPowerChargingBattery;
        case QBatteryInfo::IdleChargingState:
            return WallPower;
        default:
            return UnknownState;
        }
	}

	QString System::manufacturer() const
	{
        return mDeviceInfo->manufacturer();
	}

	QString System::model() const
	{
        return mDeviceInfo->model();
	}

	QString System::productName() const
	{
        return mDeviceInfo->productName();
	}

	QScriptValue System::logout(bool force) const
	{
		if(!mSystemSession->logout(force))
			throwError(QStringLiteral("LogoutError"), tr("Logout failed"));

		return thisObject();
	}

	QScriptValue System::restart(bool force) const
	{
		if(!mSystemSession->restart(force))
			throwError(QStringLiteral("RestartError"), tr("Restart failed"));

		return thisObject();
	}

	QScriptValue System::shutdown(bool force) const
	{
		if(!mSystemSession->shutdown(force))
			throwError(QStringLiteral("ShutdownError"), tr("Shutdown failed"));

		return thisObject();
	}

	QScriptValue System::suspend(bool force) const
	{
		if(!mSystemSession->suspend(force))
			throwError(QStringLiteral("SuspendError"), tr("Suspend failed"));

		return thisObject();
	}

	QScriptValue System::hibernate(bool force) const
	{
		if(!mSystemSession->hibernate(force))
			throwError(QStringLiteral("HibernateError"), tr("Hibernate failed"));

		return thisObject();
	}

	QScriptValue System::lockScreen() const
	{
		if(!mSystemSession->lockScreen())
			throwError(QStringLiteral("LockScreenError"), tr("Lock screen failed"));

		return thisObject();
	}

	QScriptValue System::startScreenSaver() const
	{
		if(!mSystemSession->startScreenSaver())
			throwError(QStringLiteral("StartScreenSaverError"), tr("Start screen saver failed"));

		return thisObject();
	}
}
