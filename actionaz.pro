TEMPLATE = subdirs
CONFIG = ordered

contains(DEFINES, ACT_NO_UPDATER){
message(** No updater will be built **)
}

SUBDIRS += tools \
	actiontools \
	executer \
	gui \
	actions/actionpackinternal \
	actions/actionpackwindows \
	actions/actionpackmouse \
	actions/actionpackkeyboard \
	actions/actionpacksystem \
	actions/actionpackdata \
	actions/actionpackother
