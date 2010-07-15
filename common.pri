DEFINES += QT_USE_FAST_CONCATENATION \
	QT_USE_FAST_OPERATOR_PLUS
contains(DEFINES, ACT_PROFILE) {
eval($$QMAKESPEC = "") {
QMAKE_CXXFLAGS += -include "highresolutiontimer.h"
}
*-msvc*:INCLUDEPATH += . tools ../tools ../../tools
*-msvc*:QMAKE_CXXFLAGS += /FI "highresolutiontimer.h"
}
