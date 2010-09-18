include(../common_actions.pri)
TARGET = ActionPackInternal
QT += script
SOURCES += actionvariableconditioninstance.cpp
HEADERS += actionpackinternal.h \
    actionstopdefinition.h \
    actionstopinstance.h \
    actionvariabledefinition.h \
    actionvariableinstance.h \
    actiontimeconditiondefinition.h \
    actiontimeconditioninstance.h \
    actionexitdefinition.h \
    actionexitinstance.h \
    actionvariableconditioninstance.h \
    actionvariableconditiondefinition.h \
	actionpausedefinition.h \
    actionpauseinstance.h \
	actioncodedefinition.h \
    actioncodeinstance.h \
	actiongotodefinition.h \
    actiongotoinstance.h \
	actionloopdefinition.h \
    actionloopinstance.h \
	actionnoopdefinition.h \
    actionnoopinstance.h
RESOURCES += actionpackinternal.qrc
TRANSLATIONS = locale/actionpackinternal_en.ts \
	locale/actionpackinternal_fr.ts \
	locale/actionpackinternal_de.ts
