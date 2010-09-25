HEADERS += code/system.h \
    code/mediaplaylist.h

SOURCES += code/system.cpp \
    code/mediaplaylist.cpp

CONFIG += mobility qxt
MOBILITY += systeminfo \
	multimedia
QXT += core \
    gui
