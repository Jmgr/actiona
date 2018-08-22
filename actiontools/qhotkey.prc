mac: LIBS += -framework Carbon
else:win32: LIBS += -luser32
else:unix {
	QT += x11extras
	LIBS += -lX11
}
