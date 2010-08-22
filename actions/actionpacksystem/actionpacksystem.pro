include(../common_actions.pri)
TARGET = ActionPackSystem
QT += script
HEADERS += actionpacksystem.h \
	actioncommanddefinition.h \
	actioncommandinstance.h \
	actionkillprocessdefinition.h \
	actionkillprocessinstance.h \
	actionnotifydefinition.h \
	actionnotifyinstance.h \
	actionpixelcolordefinition.h \
	actionpixelcolorinstance.h
RESOURCES += actionpacksystem.qrc
SOURCES += actionkillprocessinstance.cpp \
	actionnotifyinstance.cpp
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify

TRANSLATIONS = locale/actionpacksystem_en.ts \
	locale/actionpacksystem_fr.ts \
	locale/actionpacksystem_de.ts
