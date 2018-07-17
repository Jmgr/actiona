include(../common_actions.pri)
include(actions/actions.pri)
include(code/code.pri)
TARGET = ActionPackDevice
QT += script widgets
unix:QT += x11extras
OTHER_FILES += device.json
HEADERS += actionpackdevice.h \
    mousedevice.h \
    keyboarddevice.h
RESOURCES += actionpackdevice.qrc
unix:LIBS += -lXtst
win32:LIBS += -luser32
TRANSLATIONS = ../../locale/actionpackdevice_fr_FR.ts
SOURCES += mousedevice.cpp \
    keyboarddevice.cpp

unix {
        locales.path = $${PREFIX}/share/actiona/locale
        locales.files = ../../locale/actionpackdevice_fr_FR.qm
	locales.CONFIG = no_check_exist

	INSTALLS += locales
}
