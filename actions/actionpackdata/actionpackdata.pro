include(../common_actions.pri)
include(code/code.pri)
TARGET = ActionPackData
QT += script
HEADERS += actionpackdata.h \
	actionreadtextfiledefinition.h \
	actionreadtextfileinstance.h \
	actionwriteinifiledefinition.h \
	actionwriteinifileinstance.h \
	actionreadinifiledefinition.h \
	actionreadinifileinstance.h \
	actionreadregistryinstance.h \
	actionreadregistrydefinition.h \
	actionwriteregistrydefinition.h \
	actionwriteregistryinstance.h \
	actionreadclipboarddefinition.h \
	actionreadclipboardinstance.h \
	actionwriteclipboarddefinition.h \
	actionwriteclipboardinstance.h \
	actionreadbinaryfileinstance.h \
	actionreadbinaryfiledefinition.h \
	actionwritebinaryfileinstance.h \
	actionwritebinaryfiledefinition.h \
	actioncopyfileinstance.h \
	actioncopyfiledefinition.h
SOURCES +=	actionreadtextfileinstance.cpp \
	actionreadregistryinstance.cpp \
	actionwriteregistryinstance.cpp
INCLUDEPATH += ../../3rdparty/rudeconfig
LIBS += -L../.. -lrudeconfig
win32:LIBS += -ladvapi32

TRANSLATIONS = locale/actionpackdata_en.ts \
	locale/actionpackdata_fr.ts \
	locale/actionpackdata_de.ts
