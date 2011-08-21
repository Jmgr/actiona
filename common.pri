DEFINES += QT_USE_FAST_CONCATENATION \
	QT_USE_FAST_OPERATOR_PLUS

ACTIONAZ_VERSION	= 0.0.7
SCRIPT_VERSION		= 0.0.1

DEFINES *= ACT_VERSION=$$ACTIONAZ_VERSION
DEFINES *= ACT_SCRIPT_VERSION=$$SCRIPT_VERSION

UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated
OBJECTS_DIR = generated

unix {
	isEmpty(PREFIX): PREFIX=/usr/local

	PREFIXSTR = '\\"$${PREFIX}\\"'
	DEFINES += ACT_PREFIX=\"$${PREFIXSTR}\"
}

contains(DEFINES, ACT_PROFILE) {
	!*-msvc*:QMAKE_CXXFLAGS += -include "highresolutiontimer.h"
	*-msvc*:INCLUDEPATH += . tools ../tools ../../tools
	*-msvc*:QMAKE_CXXFLAGS += /FI "highresolutiontimer.h"
}

*-g++*::QMAKE_CXXFLAGS_DEBUG = -pedantic -Wall -Wextra -Wno-long-long -g -Wpointer-arith -Wcast-qual \
	-Wcast-align -Woverloaded-virtual -Wwrite-strings -Winit-self -Wundef -Wlogical-op -Winline
*-g++*::QMAKE_CXXFLAGS_RELEASE = -mmmx -msse -msse2 -mfpmath=sse -O3 -s
*-msvc*::QMAKE_CXXFLAGS_RELEASE += -O2 -fp:fast -GL
*-msvc*::QMAKE_LFLAGS_RELEASE += /LTCG

!contains(QMAKE_HOST.arch, x86_64):{
    *-msvc*::QMAKE_CXXFLAGS_RELEASE += -arch:SSE2
}
