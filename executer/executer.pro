include(../common.pri)
QT += script
TEMPLATE = lib
CONFIG += dll qxt
QXT += core \
    gui
DEFINES += EXECUTER_LIBRARY
win32:DEFINES += _CRT_SECURE_NO_WARNINGS
TARGET = executer
DESTDIR = ..
FORMS += executionwindow.ui
SOURCES += executer.cpp \
	executionwindow.cpp \
	executionenvironment.cpp \
	executionalgorithms.cpp
HEADERS += executer_global.h \
	executer.h \
	executionwindow.h \
	scriptagent.h \
	executionenvironment.h \
	executionalgorithms.h
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
TRANSLATIONS = locale/executer_en.ts \
	locale/executer_fr.ts
RESOURCES += executer.qrc
