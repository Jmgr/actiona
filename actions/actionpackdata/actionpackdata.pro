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
