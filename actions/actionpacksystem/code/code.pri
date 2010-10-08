HEADERS += code/system.h \
	code/mediaplaylist.h
SOURCES += code/system.cpp \
	code/mediaplaylist.cpp
win32:LIBS += -ladvapi32
CONFIG += mobility \
    qxt
MOBILITY += systeminfo \
    multimedia
QXT += core \
    gui
