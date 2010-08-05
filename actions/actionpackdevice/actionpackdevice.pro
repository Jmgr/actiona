include(../common_actions.pri)
TARGET = ActionPackDevice
QT += script
HEADERS += actionpackdevice.h \
    actionwheeldefinition.h \
    actionwheelinstance.h \
    actionkeydefinition.h \
    actionkeyinstance.h
RESOURCES += actionpackdevice.qrc
unix:LIBS += -lXtst
win32:LIBS += -luser32

# Text
HEADERS += actiontextdefinition.h \
    actiontextinstance.h
SOURCES += actiontextinstance.cpp \
    actionwheelinstance.cpp \
    actionkeyinstance.cpp

# Click
HEADERS += actionclickdefinition.h \
    actionclickinstance.h
SOURCES += actionclickinstance.cpp
