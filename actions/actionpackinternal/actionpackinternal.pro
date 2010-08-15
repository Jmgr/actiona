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
    actionvariableconditiondefinition.h
RESOURCES += actionpackinternal.qrc

# Pause
HEADERS += actionpausedefinition.h \
    actionpauseinstance.h

# Code
HEADERS += actioncodedefinition.h \
    actioncodeinstance.h

# Goto
HEADERS += actiongotodefinition.h \
    actiongotoinstance.h

# Loop
HEADERS += actionloopdefinition.h \
    actionloopinstance.h
SOURCES += 

# No-op
HEADERS += actionnoopdefinition.h \
    actionnoopinstance.h

TRANSLATIONS = locale/actionpackinternal_en.ts \
	locale/actionpackinternal_fr.ts \
	locale/actionpackinternal_de.ts
