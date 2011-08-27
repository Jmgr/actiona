include(../common_actions.pri)
include(actions/actions.pri)
include(code/code.pri)
TARGET = ActionPackDevice
QT += script
HEADERS += actionpackdevice.h \
    mousedevice.h \
    keyboarddevice.h
RESOURCES += actionpackdevice.qrc
unix:LIBS += -lXtst
win32:LIBS += -luser32
TRANSLATIONS = ../../locale/actionpackdevice_fr_FR.ts
include(../../translations.pri)
SOURCES += mousedevice.cpp \
    keyboarddevice.cpp

unix {
	locales.path = $${PREFIX}/share/actionaz/locale
	locales.files = ../../locale/actionpackdevice_fr_FR.qm
	locales.config = no_check_exist

	INSTALLS += locales
}
