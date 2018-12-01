TEMPLATE = subdirs
CONFIG = ordered

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
