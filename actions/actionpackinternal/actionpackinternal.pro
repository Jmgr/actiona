include(../common_actions.pri)
include(actions/actions.pri)
TARGET = ActionPackInternal
QT += script
HEADERS += actionpackinternal.h
RESOURCES += actionpackinternal.qrc
TRANSLATIONS = locale/actionpackinternal_en.ts \
	locale/actionpackinternal_fr.ts \
	locale/actionpackinternal_de.ts
INCLUDEPATH += ../../executer
