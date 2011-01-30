include(../common.pri)
QT += script scripttools
TEMPLATE = lib
CONFIG += dll uitools
DEFINES += EXECUTER_LIBRARY
win32:DEFINES += _CRT_SECURE_NO_WARNINGS
TARGET = executer
DESTDIR = ..
FORMS += executionwindow.ui
SOURCES += executer.cpp \
	executionwindow.cpp \
    codeinitializer.cpp \
    codeexecution.cpp \
	codestdio.cpp \
    scriptagent.cpp \
    codeactionaz.cpp
HEADERS += executer_global.h \
	executer.h \
	executionwindow.h \
	scriptagent.h \
    codeinitializer.h \
    codeexecution.h \
	codestdio.h \
    codeactionaz.h
INCLUDEPATH += src \
	../tools \
	../actiontools
LIBS += -L.. \
	-ltools \
	-lactiontools
win32:LIBS += -ladvapi32
UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated
OBJECTS_DIR = generated
TRANSLATIONS = ../locale/executer_fr_FR.ts
RESOURCES += executer.qrc
