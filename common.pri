DEFINES += QT_USE_FAST_CONCATENATION \
    QT_USE_FAST_OPERATOR_PLUS
contains(DEFINES, ACT_PROFILE) {
QMAKE_CXXFLAGS += -include "highresolutiontimer.h"
}
