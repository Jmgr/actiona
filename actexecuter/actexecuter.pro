include(../common.pri)
unix:!mac:QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN -Wl,--rpath=$${PREFIX}/$${LIBDIR}/actiona
QT += network \
    script \
    scripttools \
    widgets
TARGET = actexec
VERSION = $$ACTIONA_VERSION
DESTDIR = ..
SOURCES += main.cpp \
    executer.cpp \
    codeexecuter.cpp \
    scriptexecuter.cpp \
	mainclass.cpp
HEADERS += executer.h \
    codeexecuter.h \
    scriptexecuter.h \
	mainclass.h \
    global.h
INCLUDEPATH += . \
    .. \
    ../tools \
	../actiontools
FORMS += 
win32:LIBS += -luser32 \
    -ladvapi32 \
    -lole32
unix:LIBS += -lXtst
LIBS += -L.. \
    -ltools \
    -lactiontools \
    -lexecuter
TRANSLATIONS = ../locale/actexecuter_fr_FR.ts
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify

win32 {
QMAKE_TARGET_COMPANY = "https://actiona.tools"
QMAKE_TARGET_DESCRIPTION = "Actiona: Cross-Platform Automation Tool"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2005 Jonathan Mercier-Ganady"
QMAKE_TARGET_PRODUCT = "Actiona"
RC_ICONS = "../gui/icons/actiona.ico"
}

win32 {
    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS += $$quote( /MANIFESTUAC:\"level=\'asInvoker\' uiAccess=\'true\'\" )
}

unix {
	target.path = $${PREFIX}/bin

        locales.path = $${PREFIX}/share/actiona/locale
        locales.files = ../locale/actexecuter_fr_FR.qm
	locales.CONFIG = no_check_exist

	manfile.path = $${PREFIX}/share/man/man1
	manfile.files = ../docs/actexec.1

	INSTALLS += target locales manfile
}
