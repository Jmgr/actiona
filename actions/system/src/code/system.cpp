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

	Contact: jmgr@jmgr.info
*/

#include "system.hpp"
#include "actiontools/code/rect.hpp"
#include "backend/system.hpp"

#include <QStandardPaths>
#include "systeminfo/qdeviceinfo.h"
#include "systeminfo/qbatteryinfo.h"
#include "systeminfo/qstorageinfo.h"
#include <QLocale>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QApplication>
#include <QUrl>
#include <QDir>
#include <QDateTime>
#include <cstdlib>
#include <QScreen>

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
        mDeviceInfo(new QDeviceInfo(this)),
        mBatteryInfo(new QBatteryInfo(this)),
        mStorageInfo(new QStorageInfo_Custom(this))
	{
	}

	System::~System()
    {
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
        return QGuiApplication::screens().size();
	}

	QScriptValue System::availableGeometry(int screen) const
	{
        auto screens = QGuiApplication::screens();
        if(screen < 0 || screen >= screens.size())
        {
            return Rect::constructor({}, engine());
        }
        return Rect::constructor(screens[screen]->availableGeometry(), engine());
	}

	QScriptValue System::screenGeometry(int screen) const
	{
        auto screens = QGuiApplication::screens();
        if(screen < 0 || screen >= screens.size())
        {
            return Rect::constructor({}, engine());
        }
        return Rect::constructor(screens[screen]->geometry(), engine());
	}

	int System::primaryScreen() const
	{
        auto screens = QGuiApplication::screens();
        for(int i = 0; i < screens.size(); i++)
        {
            if(screens[i] == QGuiApplication::primaryScreen())
            {
                return i;
            }
        }
        return 0;
	}

	bool System::isVirtualDesktop() const
	{
        auto primaryScreen = QGuiApplication::primaryScreen();
        return !primaryScreen->virtualSiblings().empty();
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
        return qEnvironmentVariable("USER");
	#endif
	}

	QString System::variable(const QString &name) const
	{
        return qEnvironmentVariable(name.toLocal8Bit().constData());
	}

    qint64 System::timestamp() const
	{
        return QDateTime::currentDateTimeUtc().currentSecsSinceEpoch();
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
        try
        {
            Backend::Instance::system().logout(force);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("LogoutError"), tr("Logout failed: %1").arg(e.what()));
        }

		return thisObject();
	}

	QScriptValue System::restart(bool force) const
	{
        try
        {
            Backend::Instance::system().restart(force);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("RestartError"), tr("Restart failed: %1").arg(e.what()));
        }

		return thisObject();
	}

	QScriptValue System::shutdown(bool force) const
	{
        try
        {
            Backend::Instance::system().shutdown(force);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("ShutdownError"), tr("Shutdown failed: %1").arg(e.what()));
        }

		return thisObject();
	}

	QScriptValue System::suspend(bool force) const
	{
        try
        {
            Backend::Instance::system().suspend(force);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("SuspendError"), tr("Suspend failed: %1").arg(e.what()));
        }

		return thisObject();
	}

	QScriptValue System::hibernate(bool force) const
	{
        try
        {
            Backend::Instance::system().hibernate(force);
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("HibernateError"), tr("Hibernate failed: %1").arg(e.what()));
        }

		return thisObject();
	}

	QScriptValue System::lockScreen() const
	{
        try
        {
            Backend::Instance::system().lockScreen();
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("LockScreenError"), tr("Lock screen failed: %1").arg(e.what()));
        }

		return thisObject();
	}

	QScriptValue System::startScreenSaver() const
	{
        try
        {
            Backend::Instance::system().startScreenSaver();
        }
        catch(const Backend::BackendError &e)
        {
            throwError(QStringLiteral("StartScreenSaverError"), tr("Start screen saver failed: %1").arg(e.what()));
        }

		return thisObject();
	}
}
