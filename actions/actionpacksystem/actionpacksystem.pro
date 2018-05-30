include(../common_actions.pri)
include(actions/actions.pri)
include(code/code.pri)
TARGET = ActionPackSystem
QT += script network widgets multimedia multimediawidgets network
isQtVersionGreaterOrEqualThan510() {
QT += texttospeech
}
OTHER_FILES += system.json
unix:QT += dbus

include(systeminfo/systeminfo.pri)

HEADERS += actionpacksystem.h \
    systemsession.h
RESOURCES += actionpacksystem.qrc
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify
win32:DEFINES += _CRT_SECURE_NO_WARNINGS
win32:LIBS += -lPowrProf

TRANSLATIONS = ../../locale/actionpacksystem_fr_FR.ts

SOURCES += \
    systemsession.cpp

unix {
        locales.path = $${PREFIX}/share/actiona/locale
        locales.files = ../../locale/actionpacksystem_fr_FR.qm
	locales.CONFIG = no_check_exist

	INSTALLS += locales
}
