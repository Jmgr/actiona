include(../common_actions.pri)
include(actions/actions.pri)
include(code/code.pri)
TARGET = ActionPackWindows
QT += script widgets
OTHER_FILES += windows.json
HEADERS += actionpackwindows.h
RESOURCES += actionpackwindows.qrc
TRANSLATIONS = ../../locale/actionpackwindows_fr_FR.ts \
                ../../locale/actionpackwindows_de_DE.ts

unix {
        locales.path = $${PREFIX}/share/actiona/locale
        locales.files = ../../locale/actionpackwindows_fr_FR.qm \
                        ../../locale/actionpackwindows_de_DE.qm
	locales.CONFIG = no_check_exist

	INSTALLS += locales
}
