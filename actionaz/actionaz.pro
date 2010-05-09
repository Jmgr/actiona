include(../common.pri)
QT += xml \
    network \
    script
CONFIG += qxt \
    mobility
MOBILITY += systeminfo
QXT += core \
    gui
win32:TARGET = actionaz
unix:TARGET = actionaz.bin
DESTDIR = ..
SOURCES += main.cpp \
    mainwindow.cpp \
    pch.cpp \
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
    scriptcontentdialog.cpp \
    keywords.cpp
HEADERS += mainwindow.h \
    pch.h \
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
    scriptcontentdialog.h \
    keywords.h
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
PRECOMPILED_HEADER = pch.h
win32:LIBS += -luser32 \
    -ladvapi32
LIBS += -L.. \
    -ltools \
    -lactiontools \
    -lexecuter
RESOURCES += actionaz.qrc
win32:RC_FILE = actionaz.rc
UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated
OBJECTS_DIR = generated
TRANSLATIONS = locale/actionaz_en.ts \
    locale/actionaz_fr.ts
