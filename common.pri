DEFINES += QT_USE_FAST_CONCATENATION \
	QT_USE_FAST_OPERATOR_PLUS

ACTIONA_VERSION	= 3.9.1
SCRIPT_VERSION = 1.1.0

if(!isEmpty(VERSION_OVERRIDE)) {
    ACTIONA_VERSION = $$VERSION_OVERRIDE
}

DEFINES *= ACT_VERSION=$$ACTIONA_VERSION
DEFINES *= ACT_SCRIPT_VERSION=$$SCRIPT_VERSION
DEFINES *= ACT_BUILD_NAME=\\\"$$BUILD_NAME\\\"

UI_DIR = generated
MOC_DIR = generated
RCC_DIR = generated
OBJECTS_DIR = generated

unix {
	isEmpty(PREFIX): PREFIX=/usr/local
	isEmpty(LIBDIR): LIBDIR=lib

	PREFIXSTR = '\\"$${PREFIX}\\"'
	LIBDIRSTR = '\\"$${LIBDIR}\\"'
	DEFINES += ACT_PREFIX=\"$${PREFIXSTR}\"
	DEFINES += ACT_LIBDIR=\"$${LIBDIRSTR}\"
}

win32 {
	DEFINES += WIN32_LEAN_AND_MEAN NOMINMAX
}

contains(DEFINES, ACT_PROFILE) {
	!*-msvc*:QMAKE_CXXFLAGS += -include "highresolutiontimer.h"
	*-msvc*:QMAKE_CXXFLAGS += /FI "highresolutiontimer.h"
	INCLUDEPATH += . tools ../tools ../../tools
}

*clang*|*-g++*::QMAKE_CXXFLAGS += -std=c++11
*clang*|*-g++*::QMAKE_CXXFLAGS_DEBUG += -pedantic -Wall -Wextra -Wno-long-long -g -Wpointer-arith -Wcast-qual \
	-Wcast-align -Woverloaded-virtual -Wwrite-strings -Winit-self -Wundef -Wlogical-op -Winline
*clang*|*-g++*::QMAKE_CXXFLAGS_RELEASE += -O3 -s
*-msvc*::QMAKE_CXXFLAGS_RELEASE += -O2 -fp:fast -GL
*-msvc*::QMAKE_LFLAGS_RELEASE += /LTCG

!contains(QMAKE_HOST.arch, x86_64):{
    *-msvc*::QMAKE_CXXFLAGS_RELEASE += -arch:SSE2
}
