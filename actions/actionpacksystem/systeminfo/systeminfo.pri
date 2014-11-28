INCLUDEPATH += systeminfo systeminfo/windows

PUBLIC_HEADERS = systeminfo/qsysteminfoglobal.h \
                 systeminfo/qdeviceinfo.h \
                 systeminfo/qstorageinfo.h \
                 systeminfo/qscreensaver.h \
                 systeminfo/qbatteryinfo.h \
                 systeminfo/qnetworkinfo.h

SOURCES += systeminfo/qdeviceinfo.cpp \
           systeminfo/qstorageinfo.cpp \
           systeminfo/qscreensaver.cpp \
           systeminfo/qbatteryinfo.cpp \
           systeminfo/qnetworkinfo.cpp

win32: !simulator: {
    # Wbemidl.h violates C/C++ strict strings
    QMAKE_CXXFLAGS -= -Zc:strictStrings
    QMAKE_CXXFLAGS_RELEASE -= -Zc:strictStrings

    contains(CONFIG, release) {
       CONFIG -= console
    }

    win32-msvc*: {
        LIBS += -lUser32 -lGdi32 -lPowrProf -lBthprops -lWs2_32 -lVfw32 -lSetupapi -lIphlpapi -lOle32 -lWbemuuid
    }

    win32-g++*: {
        LIBS += -luser32 -lgdi32 -lpowrprof -lbthprops -lws2_32 -lmsvfw32 -lavicap32 -luuid
    }

    PRIVATE_HEADERS += systeminfo/windows/qscreensaver_win_p.h \
                       systeminfo/windows/qdeviceinfo_win_p.h \
                       systeminfo/windows/qstorageinfo_win_p.h \
                       systeminfo/windows/qbatteryinfo_win_p.h \
                       systeminfo/windows/qnetworkinfo_win_p.h \
                       systeminfo/windows/qwmihelper_win_p.h \
                       systeminfo/qsysteminfoglobal_p.h

    SOURCES += systeminfo/windows/qscreensaver_win.cpp \
               systeminfo/windows/qdeviceinfo_win.cpp \
               systeminfo/windows/qstorageinfo_win.cpp \
               systeminfo/windows/qbatteryinfo_win.cpp \
               systeminfo/windows/qnetworkinfo_win.cpp \
               systeminfo/windows/qwmihelper_win.cpp

       LIBS += \
            -lole32 \
            -liphlpapi \
            -loleaut32 \
            -lsetupapi

  win32-g++: {
        LIBS += -luser32 -lgdi32
    }

}

linux-*: !simulator: {
    PRIVATE_HEADERS += systeminfo/linux/qdeviceinfo_linux_p.h \
                       systeminfo/linux/qstorageinfo_linux_p.h \
                       systeminfo/linux/qnetworkinfo_linux_p.h \
                       systeminfo/linux/qscreensaver_linux_p.h

    SOURCES += systeminfo/linux/qdeviceinfo_linux.cpp \
               systeminfo/linux/qstorageinfo_linux.cpp \
               systeminfo/linux/qnetworkinfo_linux.cpp \
               systeminfo/linux/qscreensaver_linux.cpp

    x11|config_x11: !contains(CONFIG,nox11option) {
        CONFIG += link_pkgconfig
        PKGCONFIG += x11
    } else: {
        DEFINES += QT_NO_X11
    }

    config_bluez {
        CONFIG += link_pkgconfig
        PKGCONFIG += bluez
    } else: {
        DEFINES += QT_NO_BLUEZ
    }

    qtHaveModule(dbus) {
        QT += dbus
        contains(CONFIG,ofono): {
            PRIVATE_HEADERS += systeminfo/linux/qofonowrapper_p.h
            SOURCES += systeminfo/linux/qofonowrapper.cpp
        } else {
            DEFINES += QT_NO_OFONO
        }

        config_udisks {
            QT_PRIVATE += dbus
        } else: {
            DEFINES += QT_NO_UDISKS
        }
        contains(CONFIG,upower): {
            SOURCES += systeminfo/linux/qdevicekitservice_linux.cpp \
                       systeminfo/linux/qbatteryinfo_upower.cpp
            HEADERS += systeminfo/linux/qdevicekitservice_linux_p.h \
                       systeminfo/linux/qbatteryinfo_upower_p.h
        } else {
            HEADERS += systeminfo/linux/qbatteryinfo_linux_p.h
            SOURCES += systeminfo/linux/qbatteryinfo_linux.cpp

            DEFINES += QT_NO_UPOWER
        }

        # SSU tool for Nemo Mobile, see https://github.com/nemomobile/ssu
        contains(CONFIG,ssu): {
            LIBS += -lssu
            DEFINES += QT_USE_SSU
        }

    } else {
        DEFINES += QT_NO_OFONO QT_NO_UDISKS QT_NO_UPOWER
        HEADERS += systeminfo/linux/qbatteryinfo_linux_p.h
        SOURCES += systeminfo/linux/qbatteryinfo_linux.cpp
    }

    config_udev {
        CONFIG += link_pkgconfig
        PKGCONFIG += udev
        LIBS += -ludev
        PRIVATE_HEADERS += systeminfo/linux/qudevwrapper_p.h
        SOURCES += systeminfo/linux/qudevwrapper.cpp
    } else {
        DEFINES += QT_NO_UDEV
    }
}

macx:!simulator {
#CONFIG -= x86_64
QT += core-private
         OBJECTIVE_SOURCES += systeminfo/mac/qbatteryinfo_mac.mm \
                  systeminfo/mac/qdeviceinfo_mac.mm \
                  systeminfo/mac/qnetworkinfo_mac.mm \
                  systeminfo/mac/qscreensaver_mac.mm \
                  systeminfo/mac/qstorageinfo_mac.mm

         PRIVATE_HEADERS += systeminfo/mac/qbatteryinfo_mac_p.h \
                  systeminfo/mac/qdeviceinfo_mac_p.h \
                  systeminfo/mac/qnetworkinfo_mac_p.h \
                  systeminfo/mac/qscreensaver_mac_p.h \
                  systeminfo/mac/qstorageinfo_mac_p.h

         LIBS += -framework SystemConfiguration \
                -framework Foundation \
                -framework IOKit  \
                -framework QTKit \
                -framework CoreWLAN \
                -framework CoreLocation \
                -framework CoreFoundation \
                -framework ScreenSaver \
                -framework IOBluetooth \
                -framework CoreServices \
                -framework DiskArbitration \
                -framework ApplicationServices
}

simulator {
    QT_PRIVATE += simulator
    DEFINES += QT_SIMULATOR
QMAKE_DOCS = $$PWD/../../doc/config/systeminfo/qtsysteminfo.qdocconf

    PRIVATE_HEADERS += systeminfo/simulator/qsysteminfodata_simulator_p.h \
                       systeminfo/simulator/qsysteminfobackend_simulator_p.h \
                       systeminfo/simulator/qsysteminfoconnection_simulator_p.h \
                       systeminfo/simulator/qsysteminfo_simulator_p.h


    SOURCES += systeminfo/simulator/qsysteminfodata_simulator.cpp \
               systeminfo/simulator/qsysteminfobackend_simulator.cpp \
               systeminfo/simulator/qsysteminfoconnection_simulator.cpp \
               systeminfo/simulator/qsysteminfo_simulator.cpp


    linux-*: {
        PRIVATE_HEADERS += \
                           systeminfo/linux/qscreensaver_linux_p.h \
                           systeminfo/linux/qstorageinfo_linux_p.h

        SOURCES += \
                   systeminfo/linux/qscreensaver_linux.cpp \
                   systeminfo/linux/qstorageinfo_linux.cpp

        x11|config_x11 {
            CONFIG += link_pkgconfig
            PKGCONFIG += x11
        } else: {
            DEFINES += QT_NO_X11
        }

        config_bluez {
            CONFIG += link_pkgconfig
            PKGCONFIG += bluez
        } else: {
            DEFINES += QT_NO_BLUEZ
        }

        qtHaveModule(dbus) {
            config_ofono: {
            QT += dbus
            PRIVATE_HEADERS += systeminfo/linux/qofonowrapper_p.h \
                               systeminfo/linux/qnetworkinfo_linux_p.h

            SOURCES += systeminfo/linux/qofonowrapper.cpp \
                       systeminfo/linux/qnetworkinfo_linux.cpp
            } else {
                DEFINES += QT_NO_OFONO
            }
            contains(config_test_udisks, yes): {
                QT_PRIVATE += dbus
            } else: {
                DEFINES += QT_NO_UDISKS
            }
        } else {
            DEFINES += QT_NO_OFONO QT_NO_UDISKS
        }

        config_udev {
            CONFIG += link_pkgconfig
            PKGCONFIG += udev
            LIBS += -ludev
            PRIVATE_HEADERS += systeminfo/linux/qudevwrapper_p.h
            SOURCES += systeminfo/linux/qudevwrapper.cpp
        } else {
            DEFINES += QT_NO_UDEV
        }

    }
}

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

