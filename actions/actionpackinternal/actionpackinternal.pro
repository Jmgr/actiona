include(../common_actions.pri)
TARGET = ActionPackInternal
QT += script
SOURCES +=
HEADERS += actionpackinternal.h \
	actionstopdefinition.h \
	actionstopinstance.h \
	actionvariabledefinition.h \
	actionvariableinstance.h \
    actiontimeconditiondefinition.h \
    actiontimeconditioninstance.h
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
