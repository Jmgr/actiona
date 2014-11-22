include(../common.pri)
QT += script scripttools
equals(QT_MAJOR_VERSION, 5) {
QT += widgets
}
TEMPLATE = lib
CONFIG += dll
equals(QT_MAJOR_VERSION, 4) {
CONFIG += uitools
}
equals(QT_MAJOR_VERSION, 5) {
QT += uitools
}
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
    codeactiona.cpp
HEADERS += executer_global.h \
	executer.h \
	executionwindow.h \
	scriptagent.h \
    codeinitializer.h \
    codeexecution.h \
	codestdio.h \
    codeactiona.h
INCLUDEPATH += src \
	../tools \
	../actiontools
LIBS += -L.. \
	-ltools \
	-lactiontools
win32:LIBS += -ladvapi32
TRANSLATIONS = ../locale/executer_fr_FR.ts \
                ../locale/executer_de_DE.ts
RESOURCES += executer.qrc

unix {
        target.path = $${PREFIX}/$${LIBDIR}/actiona

        locales.path = $${PREFIX}/share/actiona/locale
        locales.files = ../locale/executer_fr_FR.qm \
                        ../locale/executer_de_DE.qm
	locales.CONFIG = no_check_exist

	INSTALLS += target locales
}
