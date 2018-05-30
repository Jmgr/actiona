HEADERS += actions/commanddefinition.h \
	actions/commandinstance.h \
	actions/killprocessdefinition.h \
	actions/killprocessinstance.h \
	actions/notifydefinition.h \
	actions/notifyinstance.h \
	actions/pixelcolordefinition.h \
	actions/pixelcolorinstance.h \
	actions/systeminstance.h \
	actions/systemdefinition.h \
    actions/openurlinstance.h \
    actions/openurldefinition.h \
    actions/detachedcommanddefinition.h \
    actions/detachedcommandinstance.h \
    actions/playsounddefinition.h \
	actions/playsoundinstance.h \
	actions/findimagedefinition.h \
        actions/findimageinstance.h
SOURCES += actions/killprocessinstance.cpp \
	actions/notifyinstance.cpp \
	actions/systeminstance.cpp \
	actions/pixelcolorinstance.cpp \
	actions/playsoundinstance.cpp \
	actions/findimageinstance.cpp \
    actions/findimagedefinition.cpp

isQtVersionGreaterOrEqualThan510() {
HEADERS += actions/texttospeechdefinition.h \
        actions/texttospeechinstance.h
}
