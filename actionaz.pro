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
}

win32-g++:error(Mingw is currently not supported, please use the Microsoft compiler suite)

contains(QMAKE_HOST.arch, x86_64){
message(** Buiding 64 bit version **)
DEFINES += ACT_64_BIT
} else {
message(** Buiding 32 bit version **)
DEFINES += ACT_32_BIT
}

contains(DEFINES, ACT_NO_UPDATER){
message(** No updater will be built **)
}
contains(DEFINES, ACT_PROFILE){
message(** Profiling activated **)
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
