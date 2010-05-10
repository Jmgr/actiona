include(../common.pri)
QT += script
CONFIG += qxt
QXT += core \
    gui
TEMPLATE = lib
CONFIG += dll
DEFINES += ACTIONTOOLS_LIBRARY
TARGET = actiontools
DESTDIR = ..
FORMS += codeeditordialog.ui
include(widgets.pri)
include(parameters.pri)
SOURCES += pch.cpp \
    actionfactory.cpp \
    action.cpp \
    actioninterface.cpp \
    codeeditordialog.cpp \
    codehighlighter.cpp \
    script.cpp \
    crossplatform.cpp \
    windowhandle.cpp \
    keyinput.cpp \
    windowskeys.cpp \
    keytypes.cpp \
    parameter.cpp \
    subparameter.cpp \
    actionbuffer.cpp \
    actionexecution.cpp \
    scriptcompleter.cpp \
    keywords.cpp
HEADERS += actiontools_global.h \
    actionpackinterface.h \
    pch.h \
    actionfactory.h \
    actioninterface.h \
    action.h \
    parameter.h \
    subparameter.h \
    scriptparameter.h \
    codeeditordialog.h \
    codehighlighter.h \
    linenumberarea.h \
    script.h \
    crossplatform.h \
    windowhandle.h \
    keyinput.h \
    keytypes.h \
    keytypesdef.h \
    windowskeys.h \
    actionbuffer.h \
    actionexecution.h \
    settings.h \
    stringlistpair.h \
    scriptcompleter.h \
    keywords.h
PRECOMPILED_HEADER = pch.h
win32:LIBS += -luser32 \
    -ladvapi32 \
    -lgdi32
UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated
OBJECTS_DIR = generated
TRANSLATIONS = locale/actiontools_en.ts \
    locale/actiontools_fr.ts
RESOURCES += actiontools.qrc
INCLUDEPATH += . \
    ../tools
LIBS += -L.. \
    -ltools
