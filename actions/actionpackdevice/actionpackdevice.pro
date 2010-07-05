include(../common_actions.pri)
TARGET = ActionPackDevice
QT += script
HEADERS += actionpackdevice.h
RESOURCES += actionpackdevice.qrc
unix:LIBS += -lXtst
win32:LIBS += -luser32
win32:CONFIG -= embed_manifest_dll

# Text
HEADERS += actiontext.h \
	actiontextinstance.h
SOURCES += actiontextinstance.cpp

# Click
HEADERS += actionclick.h \
	actionclickinstance.h
SOURCES += actionclickinstance.cpp
