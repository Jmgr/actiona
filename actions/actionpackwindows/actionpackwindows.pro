include(../common_actions.pri)
TARGET = ActionPackWindows
QT += script
CONFIG += qxt
QXT += core \
    gui
SOURCES += actiontextinputinstance.cpp \
    actionwindowconditioninstance.cpp \
    actionwindowinstance.cpp
HEADERS += actionpackwindows.h \
    actiontextinputdefinition.h \
    actiontextinputinstance.h \
    actionwindowconditiondefinition.h \
    actionwindowconditioninstance.h \
    actionwindowinstance.h \
    actionwindowdefinition.h
RESOURCES += actionpackwindows.qrc

# MessageBox
SOURCES += actionmessageboxinstance.cpp
HEADERS += actionmessageboxdefinition.h \
    actionmessageboxinstance.h
