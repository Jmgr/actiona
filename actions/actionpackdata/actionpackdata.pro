include(../common_actions.pri)
include(actions/actions.pri)
include(code/code.pri)
TARGET = ActionPackData
QT += script network sql
HEADERS += actionpackdata.h
INCLUDEPATH += ../../3rdparty/rudeconfig
LIBS += -L../.. -lrudeconfig
win32:LIBS += -ladvapi32 \
	-lshell32

TRANSLATIONS = ../../locale/actionpackdata_fr_FR.ts
include(../../translations.pri)

RESOURCES += \
    actionpackdata.qrc

unix {
	locales.path = $${PREFIX}/share/actionaz/locale
	locales.files = ../../locale/actionpackdata_fr_FR.qm
	locales.config = no_check_exist

	INSTALLS += locales
}
