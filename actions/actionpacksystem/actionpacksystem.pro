include(../common_actions.pri)
TARGET = ActionPackSystem
QT += script
HEADERS += actionpacksystem.h \
	actiongetclipboarddefinition.h \
	actiongetclipboardinstance.h \
	actionsetclipboarddefinition.h \
	actionsetclipboardinstance.h \
	actioncommanddefinition.h \
	actioncommandinstance.h \
	actionkillprocessdefinition.h \
	actionkillprocessinstance.h \
	actionnotifydefinition.h \
	actionnotifyinstance.h \
    actionpixelcolordefinition.h \
	actionpixelcolorinstance.h \
	actionreadfiledefinition.h \
	actionreadfileinstance.h
RESOURCES += actionpacksystem.qrc
HEADERS +=
SOURCES += actionkillprocessinstance.cpp \
	actionnotifyinstance.cpp \
	actionreadfileinstance.cpp
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify
