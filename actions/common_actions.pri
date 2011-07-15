include(../common.pri)

TEMPLATE = lib
CONFIG += plugin dll
win32:CONFIG -= embed_manifest_dll

INCLUDEPATH += ../../tools \
	../../actiontools
DESTDIR = ../../actions

LIBS += -L../.. \
	-ltools \
	-lactiontools

unix {
	target.path = $${PREFIX}/lib/actionaz/actions
	INSTALLS += target
}
