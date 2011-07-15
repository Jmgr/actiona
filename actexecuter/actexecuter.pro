include(../common.pri)
unix:!mac:QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
QT += xml \
    network \
    script \
    scripttools
TARGET = actexec
DESTDIR = ..
SOURCES += main.cpp \
    executer.cpp \
    codeexecuter.cpp \
    scriptexecuter.cpp \
	mainclass.cpp
HEADERS += executer.h \
    codeexecuter.h \
    scriptexecuter.h \
	mainclass.h \
    global.h
INCLUDEPATH += . \
    .. \
    ../tools \
	../actiontools
FORMS += 
win32:LIBS += -luser32 \
    -ladvapi32 \
    -lole32
unix:LIBS += -lXtst
LIBS += -L.. \
    -ltools \
    -lactiontools \
    -lexecuter
win32:RC_FILE = actexecuter.rc
TRANSLATIONS = ../locale/actexecuter_fr_FR.ts
system(lrelease $$TRANSLATIONS)
unix:!mac:CONFIG += link_pkgconfig
unix:!mac:PKGCONFIG += libnotify
