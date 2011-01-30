include(../common_actions.pri)
include(actions/actions.pri)
include(code/code.pri)
TARGET = ActionPackSystem
QT += script
unix:QT += dbus
HEADERS += actionpacksystem.h \
    systemsession.h
RESOURCES += actionpacksystem.qrc
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify
win32:DEFINES += _CRT_SECURE_NO_WARNINGS
win32:LIBS += -lPowrProf

TRANSLATIONS = ../../locale/actionpacksystem_fr_FR.ts

SOURCES += \
    systemsession.cpp
