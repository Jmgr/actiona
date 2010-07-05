TEMPLATE = subdirs
CONFIG = ordered

unix:!mac {
	!system(pkg-config --exists 'libnotify') {
		error(Please install libnotify-dev)
}
	!system(pkg-config --exists 'xtst') {
		error(Please install libxtst-dev)
}
}

win32-g++:error(Mingw is currently not supported, please use the Microsoft compiler suite)

contains(DEFINES, ACT_NO_UPDATER){
message(** No updater will be built **)
}

SUBDIRS += tools \
	actiontools \
	executer \
	gui \
	actions/actionpackinternal \
	actions/actionpackwindows \
	actions/actionpackdevice \
	actions/actionpacksystem \
	actions/actionpackother
