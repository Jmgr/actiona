HEADERS += actions/wheeldefinition.h \
    actions/wheelinstance.h \
    actions/keydefinition.h \
    actions/keyinstance.h \
    actions/movecursorinstance.h \
    actions/movecursordefinition.h \
	actions/textdefinition.h \
    actions/textinstance.h \
	actions/clickdefinition.h \
    actions/clickinstance.h \
    actions/cursorpathdefinition.h \
    actions/cursorpathinstance.h \
    $$PWD/keyboardkeyconditiondefinition.h \
    $$PWD/keyboardkeyconditioninstance.h
SOURCES += actions/textinstance.cpp \
    actions/keyinstance.cpp \
	actions/clickinstance.cpp \
    actions/cursorpathinstance.cpp \
    $$PWD/keyboardkeyconditioninstance.cpp
