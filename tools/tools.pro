include(../common.pri)
QT += network
equals(QT_MAJOR_VERSION, 5) {
QT += widgets
}
TEMPLATE = lib
CONFIG += dll
DEFINES += TOOLS_LIBRARY
TARGET = tools
DESTDIR = ..
SOURCES += version.cpp \
    highresolutiontimer.cpp
win32:SOURCES += sevenziparchivewrite.cpp
HEADERS += version.h \
	tools_global.h \
    highresolutiontimer.h
win32:HEADERS += sevenziparchivewrite.h
!contains(DEFINES, ACT_NO_UPDATER) {
	SOURCES += updater.cpp
	HEADERS += updater.h
}
INCLUDEPATH += src
TRANSLATIONS = ../locale/tools_fr_FR.ts
win32:LIBS += User32.lib

unix {
        target.path = $${PREFIX}/$${LIBDIR}/actiona

        locales.path = $${PREFIX}/share/actiona/locale
	locales.files = ../locale/tools_fr_FR.qm
	locales.CONFIG = no_check_exist

	INSTALLS += target locales
}
