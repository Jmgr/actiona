include(../common.pri)
unix:!mac {
	QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
}
QT += xml \
	network \
	script \
	scripttools
CONFIG += mobility
contains(DEFINES, ACT_PROFILE) {
CONFIG += console
}
MOBILITY += systeminfo
TARGET = actionaz
DESTDIR = ..
SOURCES += main.cpp \
	mainwindow.cpp \
	scriptmodel.cpp \
	deleteactionpushbutton.cpp \
	actiondialog.cpp \
	aboutdialog.cpp \
	scriptparametersdialog.cpp \
	settingsdialog.cpp \
	settingskeyedit.cpp \
	newactiontreewidget.cpp \
	scripttableview.cpp \
	scriptmodelundocommands.cpp \
	newactiondialog.cpp \
	global.cpp \
	changelogdialog.cpp \
	scriptcontentdialog.cpp \
    sfxscriptdialog.cpp \
    progresssplashscreen.cpp \
    scriptsettingsdialog.cpp
HEADERS += mainwindow.h \
	global.h \
	scriptmodel.h \
	deleteactionpushbutton.h \
	actiondialog.h \
	aboutdialog.h \
	scriptparametersdialog.h \
	settingsdialog.h \
	settingskeyedit.h \
	newactiontreewidget.h \
	scripttableview.h \
	scriptmodelundocommands.h \
	newactiondialog.h \
	changelogdialog.h \
	scriptcontentdialog.h \
    sfxscriptdialog.h \
    progresssplashscreen.h \
    scriptsettingsdialog.h
INCLUDEPATH += . \
	src \
	../tools \
	../actiontools \
	../executer
FORMS += mainwindow.ui \
	actiondialog.ui \
	aboutdialog.ui \
	scriptparametersdialog.ui \
	settingsdialog.ui \
	newactiondialog.ui \
	changelogdialog.ui \
	scriptcontentdialog.ui \
    sfxscriptdialog.ui \
    scriptsettingsdialog.ui
win32:LIBS += -luser32 \
	-ladvapi32 \
	-lole32 \
	-lshlwapi \
	-lshell32
unix:LIBS += -lXtst
LIBS += -L.. \
	-ltools \
	-lactiontools \
	-lexecuter
RESOURCES += gui.qrc
win32:RC_FILE = gui.rc
TRANSLATIONS = ../locale/gui_fr_FR.ts
for(translation, TRANSLATIONS):system(lrelease $${translation})
system(lrelease ../locale/qt_fr_FR.ts)
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify
