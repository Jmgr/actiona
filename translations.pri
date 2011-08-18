!isEmpty(TRANSLATIONS) {
	isEmpty(QMAKE_LRELEASE) {
                win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease.exe
		else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
	}

	locale_release.name = lrelease ${QMAKE_FILE_IN}
	locale_release.input = TRANSLATIONS
	locale_release.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
	locale_release.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN}
	locale_release.CONFIG = no_link
	QMAKE_EXTRA_COMPILERS += locale_release

	PRE_TARGETDEPS += compiler_locale_release_make_all
} else:message(No translation files in project)
