include(../common_actions.pri)
include(actions/actions.pri)
TARGET = ActionPackInternal
QT += script
HEADERS += actionpackinternal.h
RESOURCES += actionpackinternal.qrc
TRANSLATIONS = ../../locale/actionpackinternal_fr_FR.ts
include(../../translations.pri)
INCLUDEPATH += ../../executer
LIBS += -L../.. -lexecuter
