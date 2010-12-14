DEFINES += QT_USE_FAST_CONCATENATION \
	QT_USE_FAST_OPERATOR_PLUS
contains(DEFINES, ACT_PROFILE) {
!*-msvc*:QMAKE_CXXFLAGS += -include "highresolutiontimer.h"
*-msvc*:INCLUDEPATH += . tools ../tools ../../tools
*-msvc*:QMAKE_CXXFLAGS += /FI "highresolutiontimer.h"
}
*-g++*::QMAKE_CXXFLAGS_DEBUG = -pedantic -Wno-long-long
*-g++*::QMAKE_CXXFLAGS_RELEASE = -mmmx -msse -msse2 -mfpmath=sse -O3
*-msvc*::QMAKE_CXXFLAGS_RELEASE += -Ox -arch:SSE2 -fp:fast
