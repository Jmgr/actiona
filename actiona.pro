TEMPLATE = subdirs
CONFIG = ordered

unix:!mac {
	!system(pkg-config --exists 'x11') {
		error(Please install pkg-config)	#Here whe assume that x11 is always present, so this is to check if pkg-config is installed
}
	!system(pkg-config --exists 'libnotify') {
		error(Please install libnotify-dev)
}
	!system(pkg-config --exists 'xtst') {
		error(Please install libxtst-dev)
}
        !system(pkg-config --exists 'opencv') {
                error(Please install libopencv-dev)
}
}

win32-g++:error(Mingw is currently not supported, please use the Microsoft compiler suite)

contains(DEFINES, ACT_NO_UPDATER){
message(** No updater will be built **)
}
contains(DEFINES, ACT_PROFILE){
message(** Profiling activated **)
}

unix:QMAKE_CLEAN += actions/*.so
win32:QMAKE_CLEAN += actions/*.dll
QMAKE_CLEAN += locale/*.qm

isEmpty(QMAKE_LRELEASE) {
	win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\\lrelease.exe
	else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

locale_release.name = lrelease
locale_release.commands = \
	$$QMAKE_LRELEASE tools/tools.pro && \
	$$QMAKE_LRELEASE actiontools/actiontools.pro && \
	$$QMAKE_LRELEASE executer/executer.pro && \
	$$QMAKE_LRELEASE actexecuter/actexecuter.pro && \
	$$QMAKE_LRELEASE gui/gui.pro && \
	$$QMAKE_LRELEASE actions/actionpackinternal/actionpackinternal.pro && \
	$$QMAKE_LRELEASE actions/actionpackwindows/actionpackwindows.pro && \
	$$QMAKE_LRELEASE actions/actionpackdevice/actionpackdevice.pro && \
	$$QMAKE_LRELEASE actions/actionpacksystem/actionpacksystem.pro && \
	$$QMAKE_LRELEASE actions/actionpackdata/actionpackdata.pro

locale_release.CONFIG = no_link
QMAKE_EXTRA_TARGETS += locale_release

SUBDIRS += tools \
	actiontools \
	executer \
        gui \
	actexecuter \
	actions/actionpackinternal \
	actions/actionpackwindows \
	actions/actionpackdevice \
	actions/actionpacksystem \
	actions/actionpackdata
