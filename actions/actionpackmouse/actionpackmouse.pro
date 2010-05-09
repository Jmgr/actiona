include(../common_actions.pri)
TARGET = ActionPackMouse
QT += script
HEADERS += actionpackmouse.h
RESOURCES += actionpackmouse.qrc

# Click
HEADERS += actionclick.h \
	actionclickinstance.h
SOURCES += actionclickinstance.cpp
unix:LIBS += -lXtst
win32:LIBS += -luser32
