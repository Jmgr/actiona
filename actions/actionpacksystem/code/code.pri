HEADERS += code/system.h \
	code/mediaplaylist.h \
    code/notify.h
SOURCES += code/system.cpp \
	code/mediaplaylist.cpp \
    code/notify.cpp
win32:LIBS += -ladvapi32
CONFIG += mobility
MOBILITY += systeminfo \
    multimedia
