/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSystems module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qnetworkinfo_mac_p.h"

#include <QtCore/private/qcore_mac_p.h>
#include <QStringList>
#include <QTimer>
#include <QHostInfo>
#include <QtCore/qmetaobject.h>


#include <CoreWLAN/CoreWLAN.h>
#include <SystemConfiguration/SystemConfiguration.h>
#include <IOBluetooth/IOBluetooth.h>

#include <net/if.h>
#include <net/if_types.h>
#include <net/if_media.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

inline QString nsstringToQString(const NSString *nsstr)
{ return QLatin1String([nsstr UTF8String]);}

inline QStringList nsarrayToQStringList(void *nsarray)
{
    NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];
    QStringList result;
    NSArray *array = static_cast<NSArray *>(nsarray);
    for (NSUInteger i=0; i<[array count]; ++i)
        result << nsstringToQString([array objectAtIndex:i]);
    [autoreleasepool release];
    return result;
}


@interface QtMNSListener : NSObject
{
    NSNotificationCenter *center;
    CWInterface * currentInterface;
}
- (void)notificationHandler:(NSNotification *)notification;
- (void)remove;
@end

@implementation QtMNSListener
- (id) init
{
    [super init];
    center = [NSNotificationCenter defaultCenter];
    currentInterface = [CWInterface interfaceWithName:nil];

    [center addObserver:self selector:@selector(notificationHandler:) name:kCWModeDidChangeNotification object:nil];
    [center addObserver:self selector:@selector(notificationHandler:) name:kCWSSIDDidChangeNotification object:nil];
    [center addObserver:self selector:@selector(notificationHandler:) name:kCWBSSIDDidChangeNotification object:nil];
    [center addObserver:self selector:@selector(notificationHandler:) name:kCWCountryCodeDidChangeNotification object:nil];
    [center addObserver:self selector:@selector(notificationHandler:) name:kCWLinkDidChangeNotification object:nil];
    [center addObserver:self selector:@selector(notificationHandler:) name:kCWPowerDidChangeNotification object:nil];

    return self;
}

-(void)dealloc
{
    [center release];
    [super dealloc];
}

-(void)remove
{
    [center removeObserver:self];
}

- (void)notificationHandler:(NSNotification *)notification
{
    QNetworkInfoPrivate::instance()->wifiNetworkChanged(QCFString::toQString([notification name]), QCFString::toQString([[notification object]name]));
}
@end
@interface RemoteDeviceRSSIHostControllerDelegate : NSObject
{
}
// See IOBluetoothHostControllerDelegate
- (void)readRSSIForDeviceComplete:(id)controller device:(IOBluetoothDevice*)device info:(BluetoothHCIRSSIInfo*)info error:(IOReturn)error;
@end

@implementation RemoteDeviceRSSIHostControllerDelegate
- (id) init
{
   [super init];
   return self;
}

- (void)readRSSIForDeviceComplete:(id)controller device:(IOBluetoothDevice*)device info:(BluetoothHCIRSSIInfo*)info error:(IOReturn)error
{
    Q_UNUSED(controller);
    Q_UNUSED(device);

    if ((error != kIOReturnSuccess) || (info == NULL)) {
        qDebug() << "ERROR: readRSSIForDeviceComplete return error";
    } else if (info->handle == kBluetoothConnectionHandleNone) {
        qDebug() << "ERROR: readRSSIForDeviceComplete no connection";
    } else {
    }
}
@end

NSObject* delegate;
@interface QtBtConnectListener : NSObject
{
}
@end

QNetworkInfoPrivate *QNetworkInfoPrivate::self = 0;

#ifndef MAC_SDK_10_5
QtMNSListener *listener;
#endif

QRunLoopThread::QRunLoopThread(QObject *parent)
    :QObject(parent)
{
    moveToThread(&t);
    t.start();
}

QRunLoopThread::~QRunLoopThread()
{
}

void QRunLoopThread::stop()
{
    QMutexLocker locker(&mutex);
    CFRunLoopStop(CFRunLoopGetCurrent());
    keepRunning = false;
    //#ifndef MAC_SDK_10_5
    //    [listener release];
    //    [delegate release];
    //#endif
    t.quit();
    t.wait();
}


void QRunLoopThread::doWork()
{
#ifndef MAC_SDK_10_5
    if (QThread::currentThread() != &t) {
        QMetaObject::invokeMethod(this, "doWork",
                                  Qt::QueuedConnection);
    } else {
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        startNetworkChangeLoop();
        delegate = [[[RemoteDeviceRSSIHostControllerDelegate alloc] init] autorelease];

        mutex.lock();
        keepRunning = true;
        mutex.unlock();

        listener = [[[QtMNSListener alloc] init] autorelease];

        SInt32 result;
        while (keepRunning &&
               (result = CFRunLoopRunInMode(kCFRunLoopDefaultMode ,1, YES))) {
        }
        [pool release];
    }
#endif
}


void networkChangeCallback(SCDynamicStoreRef, CFArrayRef changedKeys, void *)
{
    // NSLog(@"changed keys %@", changedKeys);
    QStringList keyList =  nsarrayToQStringList((void*)changedKeys);
    if (keyList.contains("State:/Network/Global/DNS")) {
    }
    if (keyList.contains("State:/Network/Global/IPv4")) {
        QNetworkInfoPrivate::instance()->ethernetChanged();
        QNetworkInfoPrivate::instance()->getDefaultInterface();
    }

    return;
}

void QRunLoopThread::startNetworkChangeLoop()
{
    storeSession = NULL;

    SCDynamicStoreContext dynStoreContext = { 0, this /*(void *)storeSession*/, NULL, NULL, NULL };
    storeSession = SCDynamicStoreCreate(NULL,
                                        CFSTR("networkChangeCallback"),
                                        networkChangeCallback,
                                        &dynStoreContext);
    if (!storeSession ) {
        qDebug()<< "could not open dynamic store: error:" << SCErrorString(SCError());
        return;
    }

    CFMutableArrayRef notificationKeys;
    notificationKeys = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);
    CFMutableArrayRef patternsArray;
    patternsArray = CFArrayCreateMutable(NULL, 0, &kCFTypeArrayCallBacks);

    CFStringRef storeKey;

    storeKey = SCDynamicStoreKeyCreateNetworkGlobalEntity(NULL,
                                                          kSCDynamicStoreDomainState,
                                                          kSCEntNetLink);
    CFArrayAppendValue(notificationKeys, storeKey);
    CFRelease(storeKey);

    storeKey = SCDynamicStoreKeyCreateNetworkServiceEntity(NULL,
                                                           kSCDynamicStoreDomainState,
                                                           kSCCompAnyRegex,
                                                           kSCEntNetLink);
    CFArrayAppendValue(patternsArray, storeKey);
    CFRelease(storeKey);

    storeKey = SCDynamicStoreKeyCreateNetworkGlobalEntity(NULL,
                                                          kSCDynamicStoreDomainState,
                                                          kSCEntNetDNS);
    CFArrayAppendValue(notificationKeys, storeKey);
    CFRelease(storeKey);

    storeKey = SCDynamicStoreKeyCreateNetworkServiceEntity(NULL,
                                                           kSCDynamicStoreDomainState,
                                                           kSCCompAnyRegex,
                                                           kSCEntNetDNS);
    CFArrayAppendValue(patternsArray, storeKey);
    CFRelease(storeKey);

    storeKey = SCDynamicStoreKeyCreateNetworkGlobalEntity(NULL,
                                                          kSCDynamicStoreDomainState,
                                                          kSCEntNetIPv4);
    CFArrayAppendValue(notificationKeys, storeKey);
    CFRelease(storeKey);

    storeKey = SCDynamicStoreKeyCreateNetworkServiceEntity(NULL,
                                                           kSCDynamicStoreDomainState,
                                                           kSCCompAnyRegex,
                                                           kSCEntNetIPv4);
    CFArrayAppendValue(patternsArray, storeKey);
    CFRelease(storeKey);

    if (!SCDynamicStoreSetNotificationKeys(storeSession , notificationKeys, patternsArray)) {
        qDebug()<< "register notification error:"<< SCErrorString(SCError());
        CFRelease(storeSession );
        CFRelease(notificationKeys);
        CFRelease(patternsArray);
        return;
    }
    CFRelease(notificationKeys);
    CFRelease(patternsArray);

    runloopSource = SCDynamicStoreCreateRunLoopSource(NULL, storeSession , 0);
    if (!runloopSource) {
        qDebug()<< "runloop source error:"<< SCErrorString(SCError());
        CFRelease(storeSession);
        return;
    }

    CFRunLoopAddSource(CFRunLoopGetCurrent(), runloopSource, kCFRunLoopDefaultMode);
    CFRelease(runloopSource);
    CFRelease(storeSession);
    return;
}


QT_BEGIN_NAMESPACE


QNetworkInfoPrivate::QNetworkInfoPrivate(QNetworkInfo *parent)
    : QObject(parent)
    , q_ptr(parent)
{
#ifndef MAC_SDK_10_5
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    if ([[CWInterface supportedInterfaces] count] > 0 ) {
        hasWifi = true;
    } else {
        hasWifi = false;
    }
    [pool release];
#endif
    rssiTimer = new QTimer(this);

    if (!self)
        self = this;
    resetInterfaceCount();
    QTimer::singleShot(200, this, SLOT(primaryInterface()));
}


QNetworkInfoPrivate::~QNetworkInfoPrivate()
{
#ifndef MAC_SDK_10_5
    if (hasWifi && networkThreadOk && runloopThread->keepRunning) {
        runloopThread->stop();
        delete runloopThread;
    }
#endif
}

void QNetworkInfoPrivate::primaryInterface()
{
    defaultInterface = getDefaultInterface();
}

void QNetworkInfoPrivate::resetInterfaceCount()
{
    networkInterfaceCounts.clear();
    networkInterfaceCounts.insert(QNetworkInfo::GsmMode,0);
    networkInterfaceCounts.insert(QNetworkInfo::CdmaMode,0);
    networkInterfaceCounts.insert(QNetworkInfo::WcdmaMode,0);
//    networkInterfaceCounts.insert(QNetworkInfo::WlanMode,0);
//    networkInterfaceCounts.insert(QNetworkInfo::EthernetMode,0);
//    networkInterfaceCounts.insert(QNetworkInfo::BluetoothMode,0);
    networkInterfaceCounts.insert(QNetworkInfo::WimaxMode,0);
    networkInterfaceCounts.insert(QNetworkInfo::LteMode,0);
    networkInterfaceCounts.insert(QNetworkInfo::TdscdmaMode,0);

    CFArrayRef interfaceArray = SCNetworkInterfaceCopyAll(); //10.4
    CFStringRef iName;
    CFStringRef type;

    for ( long i = 0; i < CFArrayGetCount(interfaceArray); i++) {
        SCNetworkInterfaceRef thisInterface =  (SCNetworkInterfaceRef ) CFArrayGetValueAtIndex(interfaceArray, i);
        type = SCNetworkInterfaceGetInterfaceType(thisInterface);
        iName = SCNetworkInterfaceGetBSDName(thisInterface);
        if (type != NULL) {
            if (CFEqual(type, kSCNetworkInterfaceTypeBluetooth)) {
                ++networkInterfaceCounts[QNetworkInfo::BluetoothMode];
            } else if (CFEqual(type, kSCNetworkInterfaceTypeEthernet)) {
                ++networkInterfaceCounts[QNetworkInfo::EthernetMode];
            } else if (CFEqual(type, kSCNetworkInterfaceTypeIEEE80211)) {
                ++networkInterfaceCounts[QNetworkInfo::WlanMode];
            }
        }
    }
    CFRelease(interfaceArray);
}


int QNetworkInfoPrivate::networkInterfaceCount(QNetworkInfo::NetworkMode mode)
{
   return networkInterfaceCounts.value(mode);
}

int QNetworkInfoPrivate::networkSignalStrength(QNetworkInfo::NetworkMode mode, int interfaceNumber)
{
    switch (mode) {
    case QNetworkInfo::GsmMode:
        break;
    case QNetworkInfo::CdmaMode:
        break;
    case QNetworkInfo::WcdmaMode:
        break;
    case QNetworkInfo::WlanMode:
    {
        int signalQuality = 0;
        if (hasWifi) {
#ifndef MAC_SDK_10_5
            NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];
            QString name = interfaceForMode(mode, interfaceNumber).name();
            CWInterface *wifiInterface = [CWInterface interfaceWithName:QCFString::toNSString(name)];

            if ([wifiInterface power]) {
                if (!rssiTimer->isActive())
                    rssiTimer->start(5000);
            }  else {
                if (rssiTimer->isActive())
                    rssiTimer->stop();
            }

            int rssiSignal = [[wifiInterface rssi] intValue];

            if (rssiSignal !=0 ) {
                int maxRssi = -40;
                int minRssi = [[wifiInterface noise] intValue];
                signalQuality = ( 100 * (maxRssi - minRssi) * (maxRssi - minRssi) - (maxRssi - rssiSignal) *
                                  (15 * (maxRssi - minRssi) + 62 * (maxRssi - rssiSignal)) ) /
                        ((maxRssi - minRssi) * (maxRssi - minRssi));

            } else {
                signalQuality = 0;
            }

            if (signalStrengthCache != signalQuality) {
                if (signalStrengthCache == 0) {
                    networkStatus(QNetworkInfo::WlanMode, 0);
                }
                signalStrengthCache = signalQuality;
                Q_EMIT networkSignalStrengthChanged(mode, 0, signalQuality);
            }
            [autoreleasepool release];
#endif
        }
        return signalQuality;
    }
        break;
    case QNetworkInfo::EthernetMode:
    {
        int percent = (isInterfaceActive(interfaceForMode(mode, interfaceNumber).name().toLatin1())) ? 100 : 0;
        return percent;
    }
        break;
    case QNetworkInfo::BluetoothMode:
    {
#ifdef  MAC_SDK_10_6
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
        IOBluetoothHostController* controller = IOBluetoothHostController.defaultController;

        if (controller != NULL) {

            NSArray *devices = [IOBluetoothDevice recentDevices:0];
            for ( IOBluetoothDevice *btDevice in devices ) {
                if ([btDevice isConnected]) {
                    qDebug()<<"IOBluetoothDevice connected"<< QCFString::toString([btDevice getName]);
                    //                        delegate = [[RemoteDeviceRSSIHostControllerDelegate alloc] init];
                    [delegate retain];
                    [controller setDelegate:delegate];
                    IOReturn rc = [controller readRSSIForDevice:btDevice];
                    if (rc != noErr) {
                        qDebug()<< "ERROR: call readRSSIForDevice failed";
                    }
                    //[delegate release];
                }
            }
            //              [devices release];
            //  [controller release];
        }
        [pool release];
#endif
    }
    case QNetworkInfo::WimaxMode:
        break;
    default:
        break;
    };
    return -1;
}

QNetworkInfo::CellDataTechnology QNetworkInfoPrivate::currentCellDataTechnology(int)
{
    return QNetworkInfo::UnknownDataTechnology;
}

QNetworkInfo::NetworkMode QNetworkInfoPrivate::currentNetworkMode()
{
    return modeForInterface(getDefaultInterface());
    return QNetworkInfo::UnknownMode;
}

QNetworkInfo::NetworkStatus QNetworkInfoPrivate::networkStatus(QNetworkInfo::NetworkMode mode, int interfaceNumber)
{
    QNetworkInfo::NetworkStatus status = QNetworkInfo::NoNetworkAvailable;
    switch (mode) {
    case QNetworkInfo::GsmMode:
        break;
    case QNetworkInfo::CdmaMode:
        break;
    case QNetworkInfo::WcdmaMode:
        break;
    case QNetworkInfo::WlanMode:
    {
#ifndef MAC_SDK_10_5
        if (hasWifi) {
            NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];
            CWInterface *wifiInterface = [CWInterface interfaceWithName: QCFString::toNSString(interfaceForMode(mode,interfaceNumber).name())];

            if ([wifiInterface power]) {
                if (!rssiTimer->isActive())
                    rssiTimer->start(5000);
            }  else {
                if (rssiTimer->isActive())
                    rssiTimer->stop();
            }

            switch ([[wifiInterface interfaceState]intValue]) {
            case  kCWInterfaceStateInactive:
                status = QNetworkInfo::NoNetworkAvailable;
                break;
            case kCWInterfaceStateScanning:
            case kCWInterfaceStateAuthenticating:
            case kCWInterfaceStateAssociating:
                status = QNetworkInfo::Searching;
                break;
            case kCWInterfaceStateRunning:
                status = QNetworkInfo::HomeNetwork; //Connected
                break;
            };
            [autoreleasepool release];
        }

#else
        if (isInterfaceActive(interfaceForMode(mode, interfaceNumber).name().toLatin1())) {
            status = QNetworkInfo::HomeNetwork;
        }
#endif
    }
        break;
    case QNetworkInfo::EthernetMode:
    {
        if (isInterfaceActive(interfaceForMode(mode, interfaceNumber).name().toLatin1())) {
            return QNetworkInfo::HomeNetwork; //Connected
        } else {
            return QNetworkInfo::NoNetworkAvailable;
        }
    }
        break;
    case QNetworkInfo::BluetoothMode:
    {

    }
        break;
    case QNetworkInfo::WimaxMode:
        break;
    default:
        break;
    };
    return status;
    return QNetworkInfo::UnknownStatus;
}

QString QNetworkInfoPrivate::cellId(int)
{
    return QString();
}

QString QNetworkInfoPrivate::currentMobileCountryCode(int)
{
    return QString();
}

QString QNetworkInfoPrivate::currentMobileNetworkCode(int)
{
    return QString();
}

QString QNetworkInfoPrivate::homeMobileCountryCode(int)
{
    return QString();
}

QString QNetworkInfoPrivate::homeMobileNetworkCode(int)
{
    return QString();
}

QString QNetworkInfoPrivate::imsi(int)
{
    return QString();
}

QString QNetworkInfoPrivate::locationAreaCode(int)
{
    return QString();
}

QString QNetworkInfoPrivate::macAddress(QNetworkInfo::NetworkMode mode, int interfaceNumber)
{
    return interfaceForMode(mode,interfaceNumber).hardwareAddress();
}

QString QNetworkInfoPrivate::networkName(QNetworkInfo::NetworkMode mode, int interfaceNumber)
{
    if (networkStatus(mode, interfaceNumber) == QNetworkInfo::NoNetworkAvailable) {
        return QString();
    }
    switch (mode) {
    case QNetworkInfo::GsmMode:
        break;
    case QNetworkInfo::CdmaMode:
        break;
    case QNetworkInfo::WcdmaMode:
        break;
    case QNetworkInfo::WlanMode:
    {
        QString name = interfaceForMode(mode,interfaceNumber).name();
        NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
#ifndef MAC_SDK_10_5
        if (hasWifi) {
            CWInterface *wifiInterface = [CWInterface interfaceWithName:QCFString::toNSString(name)];
            QString netname = nsstringToQString([wifiInterface ssid]);
            [pool release];
            return netname;
        }
#else
        SCDynamicStoreRef theDynamicStore;
        theDynamicStore = SCDynamicStoreCreate(nil, CFSTR("FindCurrentInterfaceAndIP"), nil, nil);

        NSMutableString *interfaceName = [NSMutableString string];
        NSString *airportPath = [NSString stringWithFormat:@"State:/Network/Interface/%@/AirPort", QCFString::toNSString(name)];

        CFDictionaryRef airportPlist = (const __CFDictionary*)SCDynamicStoreCopyValue(theDynamicStore, (CFStringRef)airportPath);

        CFRelease(theDynamicStore);
        QString netname = QCFString::ToQString([(NSDictionary *)airportPlist valueForKey:@"SSID_STR"]);
        [pool release];
        return netname;
#endif
    }
        break;
    case QNetworkInfo::EthernetMode:
    {
        QString netname;
        if (isInterfaceActive(interfaceForMode(mode,interfaceNumber).name().toLocal8Bit())) {
            int inNumber = 0;
            //            QString netname = interfaceForMode(mode,interfaceNumber).humanReadableName();
            CFArrayRef interfaceArray = SCNetworkInterfaceCopyAll(); //10.4
            CFStringRef iName;
            CFStringRef type;

            for ( long i = 0; i < CFArrayGetCount(interfaceArray); i++) {
                SCNetworkInterfaceRef thisInterface =  (SCNetworkInterfaceRef ) CFArrayGetValueAtIndex(interfaceArray, i);
                type = SCNetworkInterfaceGetInterfaceType(thisInterface);
                iName = SCNetworkInterfaceGetBSDName(thisInterface);
              //  if (interfaceName == QCFString::toQString(iName)) {
                    if (type != NULL) {
                        if (CFEqual(type, kSCNetworkInterfaceTypeEthernet)) {
                            if (inNumber == interfaceNumber) {
                                netname = QCFString::toQString(SCNetworkInterfaceGetLocalizedDisplayName(thisInterface));
                                break;
                            }  else {
                                inNumber++;
                            }
                        }
              //      }
                }
            }
        }
        return netname;//QHostInfo::localDomainName(); //FIXME
    }
        break;
    case QNetworkInfo::BluetoothMode:
        break;
    case QNetworkInfo::WimaxMode:
        break;
    default:
        break;
    };
    return QString();
}

void QNetworkInfoPrivate::connectNotify(const QMetaMethod &signal)
{
    static const QMetaMethod networkSignalStrengthChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoPrivate::networkSignalStrengthChanged);
    static const QMetaMethod networkNameChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoPrivate::networkNameChanged);
    static const QMetaMethod networkStatusChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoPrivate::networkStatusChanged);

    if (signal == networkSignalStrengthChangedSignal) {
        connect(rssiTimer, SIGNAL(timeout()), this, SLOT(rssiTimeout()));
        rssiTimer->start(5000);
    }
    if (signal == networkNameChangedSignal
            || signal == networkStatusChangedSignal) {
#ifndef MAC_SDK_10_5
        if (hasWifi) {
            runloopThread = new QRunLoopThread();
            runloopThread->doWork();
            networkThreadOk = true;
        }
#endif
    }
}

void QNetworkInfoPrivate::disconnectNotify(const QMetaMethod &signal)
{

    static const QMetaMethod networkSignalStrengthChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoPrivate::networkSignalStrengthChanged);
    static const QMetaMethod networkNameChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoPrivate::networkNameChanged);
    static const QMetaMethod networkStatusChangedSignal = QMetaMethod::fromSignal(&QNetworkInfoPrivate::networkStatusChanged);


    if (signal == networkSignalStrengthChangedSignal) {
        rssiTimer->stop();
        disconnect(rssiTimer, SIGNAL(timeout()), this, SLOT(rssiTimeout()));
    }
    if (signal == networkNameChangedSignal
            || signal == networkStatusChangedSignal) {
#ifndef MAC_SDK_10_5
        if (hasWifi && networkThreadOk) {
            runloopThread->stop();
            [delegate release];
        }
#endif
    }
}

#ifndef QT_NO_NETWORKINTERFACE
QNetworkInterface QNetworkInfoPrivate::interfaceForMode(QNetworkInfo::NetworkMode mode, int interface)
{
    NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];
     QNetworkInterface netInterface;
     CFArrayRef interfaceArray = SCNetworkInterfaceCopyAll(); //10.4
     CFStringRef iName;
     CFStringRef type;

     int interfaceTypeCount = 0;
     for (long i = 0; i < CFArrayGetCount(interfaceArray); i++) {
         SCNetworkInterfaceRef thisInterface =  (SCNetworkInterfaceRef ) CFArrayGetValueAtIndex(interfaceArray, i);
         type = SCNetworkInterfaceGetInterfaceType(thisInterface);
         iName = SCNetworkInterfaceGetBSDName(thisInterface);

         if (type != NULL) {
             if (CFEqual(type, kSCNetworkInterfaceTypeBluetooth) && mode == QNetworkInfo::BluetoothMode) {

                 netInterface = QNetworkInterface::interfaceFromName(QCFString::toQString(iName));

                 // workaround for null MAC from SCNetworkInterfaceGetHardwareAddressString and bogus BSD name here
 #ifdef  MAC_SDK_10_6
                 IOBluetoothHostController* controller = IOBluetoothHostController.defaultController;
                 QString macbtMac = QCFString::toQString([controller addressAsString]).replace("-",":").toUpper();
                 if (!macbtMac.isEmpty()) {
                     QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
                     Q_FOREACH (const QNetworkInterface thisNetInterface, interfaces) {
                         if ( thisNetInterface.hardwareAddress() == macbtMac) {
                             if (interfaceTypeCount == interface) {
                                 netInterface = thisNetInterface;
                                 break;
                             } else {
                                 interfaceTypeCount++;
                             }
                         }
                     }
                 }
#endif
             } else if (CFEqual(type, kSCNetworkInterfaceTypeEthernet) && mode == QNetworkInfo::EthernetMode) {
                 if (interfaceTypeCount == interface) {
                     netInterface = QNetworkInterface::interfaceFromName(QCFString::toQString(iName));
                     break;
                 } else {
                     interfaceTypeCount++;
                 }
             } else if (CFEqual(type, kSCNetworkInterfaceTypeIEEE80211) && mode == QNetworkInfo::WlanMode) {
                 if (interfaceTypeCount == interface) {
                     netInterface = QNetworkInterface::interfaceFromName(QCFString::toQString(iName));
                     break;
                 } else {
                     interfaceTypeCount++;
                 }
             }
         }
     }
     CFRelease(interfaceArray);
     [autoreleasepool release];
     return netInterface;
}
#endif // QT_NO_NETWORKINTERFACE

QNetworkInfo::NetworkMode QNetworkInfoPrivate::modeForInterface(QString interfaceName)
{
    QNetworkInfo::NetworkMode mode = QNetworkInfo::UnknownMode;
    CFArrayRef interfaceArray = SCNetworkInterfaceCopyAll(); //10.4
    CFStringRef iName;
    CFStringRef type;

    for ( long i = 0; i < CFArrayGetCount(interfaceArray); i++) {
        SCNetworkInterfaceRef thisInterface =  (SCNetworkInterfaceRef ) CFArrayGetValueAtIndex(interfaceArray, i);
        type = SCNetworkInterfaceGetInterfaceType(thisInterface);
        iName = SCNetworkInterfaceGetBSDName(thisInterface);
        if ( interfaceName == QCFString::toQString(iName)) {
            if (type != NULL) {
                if (CFEqual(type, kSCNetworkInterfaceTypeBluetooth)) {
                    mode = QNetworkInfo::BluetoothMode;
                    break;
                } else if (CFEqual(type, kSCNetworkInterfaceTypeEthernet)) {
                    mode = QNetworkInfo::EthernetMode;
                    break;
                } else if (CFEqual(type, kSCNetworkInterfaceTypeIEEE80211)) {
                    mode = QNetworkInfo::WlanMode;
                    break;
                }
            }
        }
    }
    CFRelease(interfaceArray);
    return mode;
}

QString QNetworkInfoPrivate::getDefaultInterface()
{
   //  resetInterfaceCount();

    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    SCDynamicStoreRef storeSession2;
    CFStringRef key;
    CFDictionaryRef globalDict;
    CFStringRef primaryInterface = NULL;
    QString interfaceName;

    storeSession2 = SCDynamicStoreCreate(NULL, CFSTR("getPrimary"), NULL, NULL);
    //    if (!storeSession2) {
    //    }
    key = SCDynamicStoreKeyCreateNetworkGlobalEntity(NULL, kSCDynamicStoreDomainState,kSCEntNetIPv4);
    globalDict = (const __CFDictionary*)SCDynamicStoreCopyValue(storeSession2, key);
    CFRelease(key);
    if (globalDict) {

        primaryInterface = (CFStringRef )CFDictionaryGetValue(globalDict,
                                                              kSCDynamicStorePropNetPrimaryInterface);
        if (primaryInterface) {
            CFRetain(primaryInterface);
        }
        CFRelease(globalDict);
    }

    CFRelease(storeSession2);
    if (primaryInterface) {
        interfaceName = QCFString::toQString(primaryInterface);
        if (interfaceName != defaultInterface) {
            Q_EMIT currentNetworkModeChanged(modeForInterface(interfaceName));
            defaultInterface = interfaceName;
        }
    }
    [pool release];
    return interfaceName;
}

void QNetworkInfoPrivate::rssiTimeout()
{
    networkSignalStrength(QNetworkInfo::WlanMode,0);
}

bool QNetworkInfoPrivate::isInterfaceActive(const char* netInterface)
{
    struct ifmediareq ifm;

    memset(&ifm, 0, sizeof(struct ifmediareq));
    strncpy(ifm.ifm_name, netInterface, IFNAMSIZ);
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    ioctl(s, SIOCGIFMEDIA, &ifm);
    if (ifm.ifm_status & IFM_ACTIVE) {
        return true;
    }
    return false;
}

void QNetworkInfoPrivate::ethernetChanged()
{
    resetInterfaceCount();
    QNetworkInfo::NetworkStatus status = QNetworkInfo::NoNetworkAvailable;
    int carrier = 0;

    if (isInterfaceActive(interfaceForMode(QNetworkInfo::EthernetMode,0).name().toLocal8Bit()),0) {
        status = QNetworkInfo::HomeNetwork/*Connected*/;
        carrier = 100;
    }
    Q_EMIT networkStatusChanged(QNetworkInfo::EthernetMode,0, status);
    Q_EMIT networkSignalStrengthChanged(QNetworkInfo::EthernetMode,0,carrier);
    Q_EMIT networkNameChanged(QNetworkInfo::EthernetMode, 0,networkName(QNetworkInfo::EthernetMode,0));
    Q_EMIT currentNetworkModeChanged(modeForInterface(getDefaultInterface()));
}

void QNetworkInfoPrivate::wifiNetworkChanged(const QString &notification, const QString interfaceName)
{
    resetInterfaceCount();
    if (notification == QLatin1String("SSID_CHANGED_NOTIFICATION")) {
        Q_EMIT networkNameChanged(QNetworkInfo::WlanMode, 0, networkName(QNetworkInfo::WlanMode, 0));
    }

    if (notification == QLatin1String("BSSID_CHANGED_NOTIFICATION")) {
        QNetworkInfo::NetworkStatus status =  networkStatus(QNetworkInfo::WlanMode,0);
        Q_EMIT networkStatusChanged(QNetworkInfo::WlanMode, 0, status);
    }
    if (notification == QLatin1String("POWER_CHANGED_NOTIFICATION")) {
#ifndef MAC_SDK_10_5
        CWInterface *wifiInterface = [CWInterface interfaceWithName:  QCFString::toNSString(interfaceName)];
        if ([wifiInterface power]) {
            if (!rssiTimer->isActive()) {
                rssiTimer->start(5000);
            }
        }  else {
            if (rssiTimer->isActive()) {
                rssiTimer->stop();
            }
            int strength = [[wifiInterface rssi] intValue];
            Q_EMIT networkSignalStrengthChanged(QNetworkInfo::WlanMode, 0, strength);
        }
#endif
    }
}

QT_END_NAMESPACE
