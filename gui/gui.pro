include(../common.pri)
unix:!mac{
  QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
  QMAKE_RPATH=
}
QT += xml \
    network \
    script
CONFIG += qxt
QXT += core \
    gui
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
    updater.cpp \
    global.cpp \
    changelogdialog.cpp \
    scriptcontentdialog.cpp
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
    updater.h \
    changelogdialog.h \
    scriptcontentdialog.h
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
    scriptcontentdialog.ui
win32:LIBS += -luser32 \
    -ladvapi32
LIBS += -L.. \
    -ltools \
    -lactiontools \
    -lexecuter
RESOURCES += gui.qrc
win32:RC_FILE = gui.rc
UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated
OBJECTS_DIR = generated
TRANSLATIONS = locale/gui_en.ts \
    locale/gui_fr.ts
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify
