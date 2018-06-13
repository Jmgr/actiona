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

#pragma once

#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
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
		Q_ENUMS(DriveType)
		Q_ENUMS(PowerState)
		Q_ENUMS(StorageLocation)
	
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
		enum PowerState
		{
			UnknownState,
			BatteryPower,
			WallPower,
			WallPowerChargingBattery
		};
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
		
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	
		System();
		~System() override;
	
	public slots:
		QString toString() const override                                { return QStringLiteral("System"); }
        bool equals(const QScriptValue &other) const override    { return defaultEqualsImplementation<System>(other); }
		QString storageLocationPath(StorageLocation location) const;
		QString storageLocationName(StorageLocation location) const;
		QScriptValue openUrl(const QString &url) const;
		int screenCount() const;
		QScriptValue availableGeometry(int screen = -1) const;
		QScriptValue screenGeometry(int screen = -1) const;
		int primaryScreen() const;
		bool isVirtualDesktop() const;
		QString currentDirectory() const;
		QString username() const;
		QString variable(const QString &name) const;
		uint timestamp() const;
		QString osName() const;
		QString version() const;
		QString countryCode() const;
		QString language() const;
		QStringList logicalDrives() const;
		qlonglong availableDiskSpace(const QString &drive) const;
		qlonglong totalDiskSpace(const QString &drive) const;
		DriveType driveType(const QString &drive) const;
		int colorDepth(int screenId = -1) const;
		int displayBrightness(int screenId = -1) const;
		int batteryLevel() const;
		PowerState powerState() const;
		QString manufacturer() const;
		QString model() const;
		QString productName() const;
		QScriptValue logout(bool force) const;
		QScriptValue restart(bool force) const;
		QScriptValue shutdown(bool force) const;
		QScriptValue suspend(bool force) const;
		QScriptValue hibernate(bool force) const;
		QScriptValue lockScreen() const;
		QScriptValue startScreenSaver() const;
		
	private:
		SystemSession *mSystemSession;
        QDeviceInfo *mDeviceInfo;
        QBatteryInfo *mBatteryInfo;
        QStorageInfo_Custom *mStorageInfo;
	};
}

