include(../common.pri)
unix:!mac:QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN -Wl,--rpath=$${PREFIX}/lib/actionaz
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
VERSION = $$ACTIONAZ_VERSION
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
	scriptcontentdialog.cpp \
    sfxscriptdialog.cpp \
    progresssplashscreen.cpp \
    scriptsettingsdialog.cpp \
    resourcedialog.cpp \
    filetypeguesser.cpp \
    resourcetablewidget.cpp \
    resourcetypedelegate.cpp
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
	scriptcontentdialog.h \
    sfxscriptdialog.h \
    progresssplashscreen.h \
    scriptsettingsdialog.h \
    resourcedialog.h \
    filetypeguesser.h \
    resourcetablewidget.h \
    resourcetypedelegate.h \
    resourcesizeitem.h
!contains(DEFINES, ACT_NO_UPDATER) {
	SOURCES += changelogdialog.cpp
	HEADERS += changelogdialog.h
}
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
    scriptsettingsdialog.ui \
    resourcedialog.ui
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
win32:system(lrelease ../locale/qt_fr_FR.ts) #For Windows we need to copy the qt translation files
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify

unix {
	target.path = $${PREFIX}/bin

	locales.path = $${PREFIX}/share/actionaz/locale
	locales.files = ../locale/gui_fr_FR.qm
	locales.CONFIG = no_check_exist

	icon.path = $${PREFIX}/share/pixmaps
	icon.files = icons/actionaz.png

	desktopicon.path = $${PREFIX}/share/icons/hicolor/48x48/apps
	desktopicon.files = icons/actionaz.png

	desktopfile.path = $${PREFIX}/share/applications
	desktopfile.files = ../actionaz.desktop

	manfile.path = $${PREFIX}/share/man/man1
	manfile.files = ../docs/actionaz.1

	mimefile.path = $${PREFIX}/share/mime/packages/
	mimefile.files = ../docs/actionaz.xml

	INSTALLS += target locales icon desktopicon desktopfile manfile mimefile
}
