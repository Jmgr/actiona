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

#ifndef SYSTEM_H
#define SYSTEM_H

#include "code/codeclass.h"

#include <QObject>
#include <QScriptValue>
#include <QScriptEngine>
#include <QStringList>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <qmobilityglobal.h>

QTM_BEGIN_NAMESPACE
class QSystemInfo;
class QSystemStorageInfo;
class QSystemDisplayInfo;
class QSystemDeviceInfo;
QTM_END_NAMESPACE
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
class QDeviceInfo;
class QBatteryInfo;
class QStorageInfo;
#endif

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
			CdromDrive
#if (QT_VERSION >= 0x050000)//BUG: Cannot use QT_VERSION_CHECK here, or the MOC will consider the condition to be true
            , RamDrive
#endif
		};
		enum PowerState
		{
			UnknownState,
			BatteryPower,
			WallPower,
			WallPowerChargingBattery
		};
#if (QT_VERSION >= 0x050000)//BUG: Cannot use QT_VERSION_CHECK here, or the MOC will consider the condition to be true
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
#else
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
#endif
		
		static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	
		System();
		~System();
	
	public slots:
        QString toString() const                                { return "System"; }
        virtual bool equals(const QScriptValue &other) const    { return defaultEqualsImplementation<System>(other); }
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
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QDeviceInfo *mDeviceInfo;
        QBatteryInfo *mBatteryInfo;
        QStorageInfo *mStorageInfo;
#else
		QTM_PREPEND_NAMESPACE(QSystemInfo) *mSystemInfo;
		QTM_PREPEND_NAMESPACE(QSystemStorageInfo) *mSystemStorageInfo;
		QTM_PREPEND_NAMESPACE(QSystemDisplayInfo) *mSystemDisplayInfo;
		QTM_PREPEND_NAMESPACE(QSystemDeviceInfo) *mSystemDeviceInfo;
#endif
	};
}

#endif // SYSTEM_H
