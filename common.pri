DEFINES += QT_USE_FAST_CONCATENATION \
        QT_USE_FAST_OPERATOR_PLUS \
        QT_NO_CAST_TO_ASCII \
        QT_NO_CAST_FROM_ASCII \
        QT_NO_CAST_FROM_BYTEARRAY

ACTIONA_VERSION	= 3.10.0
SCRIPT_VERSION = 1.1.0

if(!isEmpty(VERSION_OVERRIDE)) {
    ACTIONA_VERSION = $$VERSION_OVERRIDE
}

if(isEmpty(PKGCONFIG_OPENCV)) {
    PKGCONFIG_OPENCV = opencv
}

unix:!mac {
        !system(pkg-config --exists 'x11') {
                error(Please install pkg-config)	#Here whe assume that x11 is always present, so this is to check if pkg-config is installed
}
        !system(pkg-config --exists 'libnotify') {
                error(Please install libnotify development libraries (libnotify-dev on Debian))
}
        !system(pkg-config --exists 'xtst') {
                error(Please install XTest development libraries (libxtst-dev on Debian))
}
        !system(pkg-config --exists '$$PKGCONFIG_OPENCV') {
                error(Please install OpenCV development libraries (libopencv-dev on Debian))
}
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
        DEFINES += WIN32_LEAN_AND_MEAN NOMINMAX VC_EXTRALEAN
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

defineTest(isQtVersionGreaterOrEqualThan510) {
    greaterThan(QT_MAJOR_VERSION, 5): return(true)
    equals(QT_MAJOR_VERSION, 5):greaterThan(QT_MINOR_VERSION, 10)|equals(QT_MINOR_VERSION, 10): return(true)

    return(false)
}
