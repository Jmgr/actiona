include(../common_actions.pri)
TARGET = ActionPackInternal
QT += script
SOURCES += 
HEADERS += actionpackinternal.h
RESOURCES += actionpackinternal.qrc

# Pause
HEADERS += actionpause.h \
    actionpauseinstance.h

# Code
HEADERS += actioncode.h \
    actioncodeinstance.h

# Goto
HEADERS += actiongoto.h \
    actiongotoinstance.h

# Loop
HEADERS += actionloop.h \
    actionloopinstance.h
SOURCES += 

# No-op
HEADERS += actionnoop.h \
    actionnoopinstance.h
