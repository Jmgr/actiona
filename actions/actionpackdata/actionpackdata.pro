include(../common_actions.pri)
include(actions/actions.pri)
include(code/code.pri)
TARGET = ActionPackData
QT += script network sql
HEADERS += actionpackdata.h
INCLUDEPATH += ../../3rdparty/rudeconfig
LIBS += -L../.. -lrudeconfig
unix:LIBS += -lespeak
win32:LIBS += -ladvapi32 \
	-lshell32 \
	-lespeak_sapi

TRANSLATIONS = locale/actionpackdata_en.ts \
	locale/actionpackdata_fr.ts \
	locale/actionpackdata_de.ts
