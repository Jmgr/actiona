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
#include "../systemsession.hpp"

#include <QStandardPaths>
#include "systeminfo/qdeviceinfo.h"
#include "systeminfo/qbatteryinfo.h"
#include "systeminfo/qstorageinfo.h"
#include <QLocale>
#include <QDesktopServices>
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

    System *System::openUrl(const QString &url)
	{
		if(!QDesktopServices::openUrl(QUrl(url)))
			throwError(QStringLiteral("OpenUrlError"), tr("Cannot open the url"));

        return this;
	}

	int System::screenCount() const
	{
        return QGuiApplication::screens().size();
	}

	QJSValue System::availableGeometry(int screen) const
	{
        auto screens = QGuiApplication::screens();
        if(screen < 0 || screen >= screens.size())
        {
            return CodeClass::construct<Rect>(QRect{});
        }
        return CodeClass::construct<Rect>(screens[screen]->availableGeometry());
	}

	QJSValue System::screenGeometry(int screen) const
	{
        auto screens = QGuiApplication::screens();
        if(screen < 0 || screen >= screens.size())
        {
            return CodeClass::construct<Rect>(QRect{});
        }
        return CodeClass::construct<Rect>(screens[screen]->geometry());
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
        return QString::fromLatin1(std::getenv("USER"));
	#endif
	}

	QString System::variable(const QString &name) const
	{
		return QString::fromLatin1(std::getenv(name.toLatin1().constData()));
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

    System *System::logout(bool force)
	{
		if(!mSystemSession->logout(force))
			throwError(QStringLiteral("LogoutError"), tr("Logout failed"));

        return this;
	}

    System *System::restart(bool force)
	{
		if(!mSystemSession->restart(force))
			throwError(QStringLiteral("RestartError"), tr("Restart failed"));

        return this;
	}

    System *System::shutdown(bool force)
	{
		if(!mSystemSession->shutdown(force))
			throwError(QStringLiteral("ShutdownError"), tr("Shutdown failed"));

        return this;
	}

    System *System::suspend(bool force)
	{
		if(!mSystemSession->suspend(force))
			throwError(QStringLiteral("SuspendError"), tr("Suspend failed"));

        return this;
	}

    System *System::hibernate(bool force)
	{
		if(!mSystemSession->hibernate(force))
			throwError(QStringLiteral("HibernateError"), tr("Hibernate failed"));

        return this;
	}

    System *System::lockScreen()
	{
		if(!mSystemSession->lockScreen())
			throwError(QStringLiteral("LockScreenError"), tr("Lock screen failed"));

        return this;
	}

    System *System::startScreenSaver()
	{
		if(!mSystemSession->startScreenSaver())
			throwError(QStringLiteral("StartScreenSaverError"), tr("Start screen saver failed"));

        return this;
	}

    void System::registerClass(ActionTools::ScriptEngine &scriptEngine)
    {
        qRegisterMetaType<System*>("const System *");

        CodeClass::registerClass<System>(QStringLiteral("System"), scriptEngine);
    }
}
