include(../common_actions.pri)
include(actions/actions.pri)
include(code/code.pri)
TARGET = ActionPackWindows
QT += script
HEADERS += actionpackwindows.h
RESOURCES += actionpackwindows.qrc
TRANSLATIONS = locale/actionpackwindows_en.ts \
	locale/actionpackwindows_fr.ts \
	locale/actionpackwindows_de.ts
