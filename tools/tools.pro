include(../common.pri)
QT += network
TEMPLATE = lib
CONFIG += dll \
    qxt
QXT += core \
    gui \
    network
DEFINES += TOOLS_LIBRARY
TARGET = tools
DESTDIR = ..
SOURCES += version.cpp \
    updater.cpp
HEADERS += version.h \
    tools_global.h \
    updater.h
INCLUDEPATH += src
UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated
OBJECTS_DIR = generated
TRANSLATIONS = locale/tools_en.ts \
    locale/tools_fr.ts
