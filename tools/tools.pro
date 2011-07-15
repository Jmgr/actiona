include(../common.pri)
QT += network
TEMPLATE = lib
CONFIG += dll
DEFINES += TOOLS_LIBRARY
TARGET = tools
DESTDIR = ..
SOURCES += version.cpp \
	updater.cpp \
    highresolutiontimer.cpp
win32:SOURCES += sevenziparchivewrite.cpp
HEADERS += version.h \
	tools_global.h \
	updater.h \
    highresolutiontimer.h
win32:HEADERS += sevenziparchivewrite.h
INCLUDEPATH += src
TRANSLATIONS = ../locale/tools_fr_FR.ts
system(lrelease $$TRANSLATIONS)
win32:LIBS += User32.lib
