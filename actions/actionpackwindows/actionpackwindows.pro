include(../common_actions.pri)
include(code/code.pri)
TARGET = ActionPackWindows
QT += script
CONFIG += qxt
QXT += core \
    gui
SOURCES += actiontextinputinstance.cpp \
    actionwindowconditioninstance.cpp \
    actionwindowinstance.cpp \
	actionmessageboxinstance.cpp
HEADERS += actionpackwindows.h \
    actiontextinputdefinition.h \
    actiontextinputinstance.h \
    actionwindowconditiondefinition.h \
    actionwindowconditioninstance.h \
    actionwindowinstance.h \
    actionwindowdefinition.h \
	actionmessageboxdefinition.h \
    actionmessageboxinstance.h
RESOURCES += actionpackwindows.qrc
TRANSLATIONS = locale/actionpackwindows_en.ts \
	locale/actionpackwindows_fr.ts \
	locale/actionpackwindows_de.ts
