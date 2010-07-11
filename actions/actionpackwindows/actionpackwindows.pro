include(../common_actions.pri)
TARGET = ActionPackWindows
QT += script
SOURCES += actiontextinputinstance.cpp
HEADERS += actionpackwindows.h \
	actiontextinputdefinition.h \
	actiontextinputinstance.h
RESOURCES += actionpackwindows.qrc

# MessageBox
SOURCES += actionmessageboxinstance.cpp
HEADERS += actionmessageboxdefinition.h \
	actionmessageboxinstance.h
