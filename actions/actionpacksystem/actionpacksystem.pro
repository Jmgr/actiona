include(../common_actions.pri)
TARGET = ActionPackSystem
QT += script
HEADERS += actionpacksystem.h \
	actiongetclipboard.h \
	actiongetclipboardinstance.h \
	actionsetclipboard.h \
	actionsetclipboardinstance.h \
	actioncommand.h \
	actioncommandinstance.h \
	actionkillprocess.h \
	actionkillprocessinstance.h \
	actionnotify.h \
	actionnotifyinstance.h
RESOURCES += actionpacksystem.qrc
HEADERS +=
SOURCES += actionkillprocessinstance.cpp \
	actionnotifyinstance.cpp
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify
win32:CONFIG -= embed_manifest_dll
