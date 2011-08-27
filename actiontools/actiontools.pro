include(../common.pri)
QT += script \
    xmlpatterns
TEMPLATE = lib
CONFIG += dll
DEFINES += ACTIONTOOLS_LIBRARY
TARGET = actiontools
DESTDIR = ..
FORMS += codeeditordialog.ui
include(globalshortcut/globalshortcut.pri)
include(qxtcommandoptions/qxtcommandoptions.pri)
include(widgets.pri)
include(parameters.pri)
include(code/code.pri)
include(qtimagefilters/qtimagefilters.pri)
include(qtsingleapplication/qtsingleapplication.pri)
SOURCES += actionfactory.cpp \
    actioninstance.cpp \
    actiondefinition.cpp \
    codeeditordialog.cpp \
    codehighlighter.cpp \
    script.cpp \
    crossplatform.cpp \
    windowhandle.cpp \
    keyinput.cpp \
    parameter.cpp \
    subparameter.cpp \
    actioninstancebuffer.cpp \
    actioninstanceexecutionhelper.cpp \
    scriptcompleter.cpp \
    keywords.cpp \
    messagehandler.cpp \
    actionexception.cpp \
    helpbutton.cpp \
	keysymhelper.cpp \
    ifactionvalue.cpp \
    keymapper.cpp \
    registry.cpp \
    devicecopythread.cpp \
    datacopyactioninstance.cpp \
	nativeeventfilteringapplication.cpp \
	pointitemdelegate.cpp \
	actionpack.cpp
HEADERS += actiontools_global.h \
    actionpack.h \
    actionfactory.h \
    actiondefinition.h \
    actioninstance.h \
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
    actioninstancebuffer.h \
    actioninstanceexecutionhelper.h \
    settings.h \
    stringlistpair.h \
    scriptcompleter.h \
    keywords.h \
    messagehandler.h \
    actionexception.h \
    helpbutton.h \
    xdisplayhelper.h \
	keysymhelper.h \
    ifactionvalue.h \
    keymapper.h \
    registry.h \
    devicecopythread.h \
    datacopyactioninstance.h \
    nativeeventfilter.h \
	nativeeventfilteringapplication.h \
	pointitemdelegate.h \
	actiondefinitionenums.h
win32:LIBS += -luser32 \
    -ladvapi32 \
    -lgdi32 \
    -lPsapi
unix:LIBS += -lXtst -lX11
TRANSLATIONS = ../locale/actiontools_fr_FR.ts
include(../translations.pri)
RESOURCES += actiontools.qrc
INCLUDEPATH += . \
    ../tools
LIBS += -L.. \
    -ltools

unix {
	target.path = $${PREFIX}/lib/actionaz

	locales.path = $${PREFIX}/share/actionaz/locale
	locales.files = ../locale/actiontools_fr_FR.qm
	locales.config = no_check_exist

	INSTALLS += target locales
}
