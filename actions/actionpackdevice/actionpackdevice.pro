include(../common_actions.pri)
TARGET = ActionPackDevice
QT += script
HEADERS += actionpackdevice.h \
    actionwheeldefinition.h \
    actionwheelinstance.h \
    actionkeydefinition.h \
    actionkeyinstance.h \
    actionmovecursorinstance.h \
    actionmovecursordefinition.h \
	actiontextdefinition.h \
    actiontextinstance.h \
	actionclickdefinition.h \
    actionclickinstance.h
SOURCES += actiontextinstance.cpp \
    actionwheelinstance.cpp \
    actionkeyinstance.cpp \
	actionclickinstance.cpp
RESOURCES += actionpackdevice.qrc
unix:LIBS += -lXtst
win32:LIBS += -luser32
TRANSLATIONS = locale/actionpackdevice_en.ts \
	locale/actionpackdevice_fr.ts \
	locale/actionpackdevice_de.ts
