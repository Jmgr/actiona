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
UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated
OBJECTS_DIR = generated
TRANSLATIONS = ../locale/tools_en.ts \
	../locale/tools_fr.ts
win32:LIBS += User32.lib
