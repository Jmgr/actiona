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
	actionreadfileinstance.h \
	actionwriteinifiledefinition.h \
	actionwriteinifileinstance.h \
	actionreadinifiledefinition.h \
	actionreadinifileinstance.h \
	actionreadregistryinstance.h \
	actionreadregistrydefinition.h \
    actionwriteregistrydefinition.h \
    actionwriteregistryinstance.h
RESOURCES += actionpacksystem.qrc
SOURCES += actionkillprocessinstance.cpp \
	actionnotifyinstance.cpp \
	actionreadfileinstance.cpp \
	actionreadregistryinstance.cpp \
    actionwriteregistryinstance.cpp
INCLUDEPATH += ../../3rdparty/rudeconfig
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify
LIBS += -L../.. -lrudeconfig

TRANSLATIONS = locale/actionpacksystem_en.ts \
	locale/actionpacksystem_fr.ts \
	locale/actionpacksystem_de.ts
