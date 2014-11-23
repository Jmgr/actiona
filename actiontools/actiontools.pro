include(../common.pri)
QT += script \
    xmlpatterns
equals(QT_MAJOR_VERSION, 5) {
unix:QT += x11extras
QT += widgets
}
TEMPLATE = lib
CONFIG += dll
unix:CONFIG += link_pkgconfig
unix:PKGCONFIG += opencv
DEFINES += ACTIONTOOLS_LIBRARY
TARGET = actiontools
DESTDIR = ..
FORMS += codeeditordialog.ui \
    resourcenamedialog.ui \
    screenshotwizardpage.ui \
    savescreenshotwizardpage.ui
include(globalshortcut/globalshortcut.pri)
include(qxtcommandoptions/qxtcommandoptions.pri)
include(qxtsmtp/qxtsmtp.pri)
include(qxtcore/qxtcore.pri)
include(widgets.pri)
include(parameters.pri)
include(code/code.pri)
include(qtimagefilters/qtimagefilters.pri)
include(qtsingleapplication/qtsingleapplication.pri)
SOURCES += actionfactory.cpp \
    actioninstance.cpp \
    actiondefinition.cpp \
    codeeditordialog.cpp \
    codehighlighter.cpp \
    script.cpp \
    crossplatform.cpp \
    windowhandle.cpp \
    keyinput.cpp \
    parameter.cpp \
    subparameter.cpp \
    actioninstancebuffer.cpp \
    scriptcompleter.cpp \
    keywords.cpp \
    messagehandler.cpp \
    actionexception.cpp \
    helpbutton.cpp \
	keysymhelper.cpp \
    ifactionvalue.cpp \
    keymapper.cpp \
    registry.cpp \
    devicecopythread.cpp \
    datacopyactioninstance.cpp \
	pointitemdelegate.cpp \
	actionpack.cpp \
	opencvalgorithms.cpp \
    systeminputreceiver.cpp \
    systeminputtask.cpp \
	systeminputrecorder.cpp \
    numberformat.cpp \
    resource.cpp \
    screenshooter.cpp \
    targetwindow.cpp \
    imagelabel.cpp \
    resourcenamedialog.cpp \
    screenshotwizard.cpp \
    screenshotwizardpage.cpp \
    savescreenshotwizardpage.cpp \
    parametercontainer.cpp
HEADERS += actiontools_global.h \
    actionpack.h \
    actionfactory.h \
    actiondefinition.h \
    actioninstance.h \
    parameter.h \
    subparameter.h \
    scriptparameter.h \
    codeeditordialog.h \
    codehighlighter.h \
    linenumberarea.h \
    script.h \
    crossplatform.h \
    windowhandle.h \
    keyinput.h \
    actioninstancebuffer.h \
    settings.h \
    stringlistpair.h \
    scriptcompleter.h \
    keywords.h \
    messagehandler.h \
    actionexception.h \
    helpbutton.h \
    xdisplayhelper.h \
	keysymhelper.h \
    ifactionvalue.h \
    keymapper.h \
    registry.h \
    devicecopythread.h \
    datacopyactioninstance.h \
	pointitemdelegate.h \
	actiondefinitionenums.h \
	name.h \
	opencvalgorithms.h \
	matchingpointlist.h \
    systeminputreceiver.h \
    systeminputtask.h \
    systeminputlistener.h \
    systeminput.h \
	systeminputrecorder.h \
    resource.h \
    numberformat.h \
    screenshooter.h \
    parametercontainer.h \
    targetwindow.h \
    imagelabel.h \
    resourcenamedialog.h \
    screenshotwizard.h \
    screenshotwizardpage.h \
    savescreenshotwizardpage.h
equals(QT_MAJOR_VERSION, 4) {
SOURCES += nativeeventfilteringapplication.cpp
HEADERS += nativeeventfilteringapplication.h \
    nativeeventfilter.h
}
win32:LIBS += -luser32 \
    -ladvapi32 \
    -lgdi32 \
	-lPsapi \
	-L$${OPENCV_LIB} \
	-l$${OPENCV_LIB_CORE} \
	-l$${OPENCV_LIB_IMGPROC}
unix:LIBS += -lXtst \
	-lX11
TRANSLATIONS = ../locale/actiontools_fr_FR.ts \
                ../locale/actiontools_de_DE.ts
RESOURCES += actiontools.qrc
INCLUDEPATH += . \
	../tools
win32:INCLUDEPATH += $${BOOST_INCLUDE} $${OPENCV_INCLUDE}
LIBS += -L.. \
    -ltools

unix {
        target.path = $${PREFIX}/$${LIBDIR}/actiona

        locales.path = $${PREFIX}/share/actiona/locale
        locales.files = ../locale/actiontools_fr_FR.qm \
                        ../locale/actiontools_de_DE.qm
	locales.CONFIG = no_check_exist

	INSTALLS += target locales
}








