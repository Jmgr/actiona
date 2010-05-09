include(../common_actions.pri)
TARGET = ActionPackKeyboard
QT += script
HEADERS += actionpackkeyboard.h
RESOURCES += actionpackkeyboard.qrc

# Text
HEADERS += actiontext.h \
	actiontextinstance.h
SOURCES += actiontextinstance.cpp
unix:LIBS += -lXtst
win32:LIBS += -luser32
