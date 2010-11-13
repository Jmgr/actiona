include(../common_actions.pri)
include(actions/actions.pri)
include(code/code.pri)
TARGET = ActionPackDevice
QT += script
HEADERS += actionpackdevice.h \
    mousedevice.h \
    keyboarddevice.h
RESOURCES += actionpackdevice.qrc
unix:LIBS += -lXtst
win32:LIBS += -luser32
TRANSLATIONS = ../../locale/actionpackdevice_en.ts \
    ../../locale/actionpackdevice_fr.ts
SOURCES += mousedevice.cpp \
    keyboarddevice.cpp
