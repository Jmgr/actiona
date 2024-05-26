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

#pragma once

#include "actiontools/code/codeclass.hpp"

#include <QJSValue>
#include <QStringList>

class QDeviceInfo;
class QBatteryInfo;
class QStorageInfo_Custom;

class SystemSession;

namespace Code
{
	class System : public CodeClass
	{
		Q_OBJECT

	public:
		enum DriveType
		{
			UnknownDrive,
			InternalDrive,
			RemovableDrive,
			RemoteDrive,
            CdromDrive,
            RamDrive
		};
        Q_ENUM(DriveType)
		enum PowerState
		{
			UnknownState,
			BatteryPower,
			WallPower,
			WallPowerChargingBattery
		};
        Q_ENUM(PowerState)
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
            Cache,
            GenericData,
            Runtime,
            Config,
            Download,
            GenericCache,
            GenericConfig
        };
        Q_ENUM(StorageLocation)
		
        Q_INVOKABLE System();
		~System() override;
	
        Q_INVOKABLE QString toString() const override                                { return QStringLiteral("System"); }
        Q_INVOKABLE QString storageLocationPath(StorageLocation location) const;
        Q_INVOKABLE QString storageLocationName(StorageLocation location) const;
        Q_INVOKABLE System *openUrl(const QString &url);
        Q_INVOKABLE int screenCount() const;
        Q_INVOKABLE QJSValue availableGeometry(int screen = -1) const;
        Q_INVOKABLE QJSValue screenGeometry(int screen = -1) const;
        Q_INVOKABLE int primaryScreen() const;
        Q_INVOKABLE bool isVirtualDesktop() const;
        Q_INVOKABLE QString currentDirectory() const;
        Q_INVOKABLE QString username() const;
        Q_INVOKABLE QString variable(const QString &name) const;
        Q_INVOKABLE qint64 timestamp() const;
        Q_INVOKABLE QString osName() const;
        Q_INVOKABLE QString version() const;
        Q_INVOKABLE QString countryCode() const;
        Q_INVOKABLE QString language() const;
        Q_INVOKABLE QStringList logicalDrives() const;
        Q_INVOKABLE qlonglong availableDiskSpace(const QString &drive) const;
        Q_INVOKABLE qlonglong totalDiskSpace(const QString &drive) const;
        Q_INVOKABLE DriveType driveType(const QString &drive) const;
        Q_INVOKABLE int colorDepth(int screenId = -1) const;
        Q_INVOKABLE int displayBrightness(int screenId = -1) const;
        Q_INVOKABLE int batteryLevel() const;
        Q_INVOKABLE PowerState powerState() const;
        Q_INVOKABLE QString manufacturer() const;
        Q_INVOKABLE QString model() const;
        Q_INVOKABLE QString productName() const;
        Q_INVOKABLE System *logout(bool force);
        Q_INVOKABLE System *restart(bool force);
        Q_INVOKABLE System *shutdown(bool force);
        Q_INVOKABLE System *suspend(bool force);
        Q_INVOKABLE System *hibernate(bool force);
        Q_INVOKABLE System *lockScreen();
        Q_INVOKABLE System *startScreenSaver();

        static void registerClass(ActionTools::ScriptEngine &scriptEngine);
		
	private:
		SystemSession *mSystemSession;
        QDeviceInfo *mDeviceInfo;
        QBatteryInfo *mBatteryInfo;
        QStorageInfo_Custom *mStorageInfo;
	};
}

