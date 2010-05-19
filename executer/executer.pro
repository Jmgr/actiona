include(../common.pri)
QT += script
TEMPLATE = lib
CONFIG += dll
DEFINES += EXECUTER_LIBRARY
TARGET = executer
DESTDIR = ..
FORMS += executionwindow.ui
SOURCES += executer.cpp \
    executionwindow.cpp
HEADERS += executer_global.h \
    executer.h \
    executionwindow.h \
    scriptagent.h
INCLUDEPATH += src \
    ../tools \
    ../actiontools
LIBS += -L.. \
    -ltools \
    -lactiontools
PRECOMPILED_HEADER = executer_pch.h
UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated
OBJECTS_DIR = generated
TRANSLATIONS = locale/executer_en.ts \
    locale/executer_fr.ts
RESOURCES += executer.qrc
