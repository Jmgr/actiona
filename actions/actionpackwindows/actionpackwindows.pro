include(../common_actions.pri)
TARGET = ActionPackWindows
QT += script
SOURCES += actiontextinputinstance.cpp
HEADERS += actionpackwindows.h \
    actiontextinput.h \
    actiontextinputinstance.h
RESOURCES += actionpackwindows.qrc

# MessageBox
SOURCES += actionmessageboxinstance.cpp
HEADERS += actionmessagebox.h \
    actionmessageboxinstance.h
