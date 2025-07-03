include(ExternalProject)

set(PROJECT actiontools)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/actiondefinition.cpp
    ${SOURCES_PREFIX}/actionexception.cpp
    ${SOURCES_PREFIX}/actionfactory.cpp
    ${SOURCES_PREFIX}/actioninstancebuffer.cpp
    ${SOURCES_PREFIX}/actioninstance.cpp
    ${SOURCES_PREFIX}/actionpack.cpp
    ${SOURCES_PREFIX}/booleanedit.cpp
    ${SOURCES_PREFIX}/booleanparameterdefinition.cpp
    ${SOURCES_PREFIX}/choosepositionpushbutton.cpp
    ${SOURCES_PREFIX}/choosewindowpushbutton.cpp
    ${SOURCES_PREFIX}/codecombobox.cpp
    ${SOURCES_PREFIX}/codedatetimeedit.cpp
    ${SOURCES_PREFIX}/codeedit.cpp
    ${SOURCES_PREFIX}/codeeditordialog.cpp
    ${SOURCES_PREFIX}/codehighlighter.cpp
    ${SOURCES_PREFIX}/codelineeditbutton.cpp
    ${SOURCES_PREFIX}/codelineedit.cpp
    ${SOURCES_PREFIX}/codespinbox.cpp
    ${SOURCES_PREFIX}/codedoublespinbox.cpp
    ${SOURCES_PREFIX}/coloredit.cpp
    ${SOURCES_PREFIX}/colorparameterdefinition.cpp
    ${SOURCES_PREFIX}/colorpositionparameterdefinition.cpp
    ${SOURCES_PREFIX}/consoletableview.cpp
    ${SOURCES_PREFIX}/consolewidget.cpp
    ${SOURCES_PREFIX}/crossplatform.cpp
    ${SOURCES_PREFIX}/datacopyactioninstance.cpp
    ${SOURCES_PREFIX}/datetimeparameterdefinition.cpp
    ${SOURCES_PREFIX}/devicecopythread.cpp
    ${SOURCES_PREFIX}/elementdefinition.cpp
    ${SOURCES_PREFIX}/environmentvariableparameterdefinition.cpp
    ${SOURCES_PREFIX}/fileedit.cpp
    ${SOURCES_PREFIX}/fileparameterdefinition.cpp
    ${SOURCES_PREFIX}/groupdefinition.cpp
    ${SOURCES_PREFIX}/helpbutton.cpp
    ${SOURCES_PREFIX}/ifactionparameterdefinition.cpp
    ${SOURCES_PREFIX}/ifactionvalue.cpp
    ${SOURCES_PREFIX}/imagelabel.cpp
    ${SOURCES_PREFIX}/imageparameterdefinition.cpp
    ${SOURCES_PREFIX}/itemlistmodel.cpp
    ${SOURCES_PREFIX}/itemlistview.cpp
    ${SOURCES_PREFIX}/itemlistwidget.cpp
    ${SOURCES_PREFIX}/itemsparameterdefinition.cpp
    ${SOURCES_PREFIX}/keyboardkey.cpp
    ${SOURCES_PREFIX}/keyboardkeyedit.cpp
    ${SOURCES_PREFIX}/keyboardkeyparameterdefinition.cpp
    ${SOURCES_PREFIX}/keyedit.cpp
    ${SOURCES_PREFIX}/keyinput.cpp
    ${SOURCES_PREFIX}/keymapper.cpp
    ${SOURCES_PREFIX}/keyparameterdefinition.cpp
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keysym2ucs.cpp>
    ${SOURCES_PREFIX}/keysymhelper.cpp
    ${SOURCES_PREFIX}/keywords.cpp
    ${SOURCES_PREFIX}/linecombobox.cpp
    ${SOURCES_PREFIX}/lineparameterdefinition.cpp
    ${SOURCES_PREFIX}/listparameterdefinition.cpp
    ${SOURCES_PREFIX}/localeparameterdefinition.cpp
    ${SOURCES_PREFIX}/multitextparameterdefinition.cpp
    ${SOURCES_PREFIX}/numberformat.cpp
    ${SOURCES_PREFIX}/numberparameterdefinition.cpp
    ${SOURCES_PREFIX}/numberrangeparameterdefinition.cpp
    ${SOURCES_PREFIX}/opencvalgorithms.cpp
    ${SOURCES_PREFIX}/parametercontainer.cpp
    ${SOURCES_PREFIX}/parameter.cpp
    ${SOURCES_PREFIX}/parameterdefinition.cpp
    ${SOURCES_PREFIX}/pointitemdelegate.cpp
    ${SOURCES_PREFIX}/pointlistparameterdefinition.cpp
    ${SOURCES_PREFIX}/pointlistwidget.cpp
    ${SOURCES_PREFIX}/positionedit.cpp
    ${SOURCES_PREFIX}/positionparameterdefinition.cpp
    ${SOURCES_PREFIX}/procedureparameterdefinition.cpp
    ${SOURCES_PREFIX}/registry.cpp
    ${SOURCES_PREFIX}/resource.cpp
    ${SOURCES_PREFIX}/resourcenamedialog.cpp
    ${SOURCES_PREFIX}/savescreenshotwizardpage.cpp
    ${SOURCES_PREFIX}/screenpositionwidget.cpp
    ${SOURCES_PREFIX}/screenshooter.cpp
    ${SOURCES_PREFIX}/screenshotpushbutton.cpp
    ${SOURCES_PREFIX}/screenshotwizard.cpp
    ${SOURCES_PREFIX}/screenshotwizardpage.cpp
    ${SOURCES_PREFIX}/scriptcompleter.cpp
    ${SOURCES_PREFIX}/script.cpp
    ${SOURCES_PREFIX}/scriptlinemodel.cpp
    ${SOURCES_PREFIX}/subparameter.cpp
    ${SOURCES_PREFIX}/switchtextmodepushbutton.cpp
    ${SOURCES_PREFIX}/targetwindow.cpp
    ${SOURCES_PREFIX}/textparameterdefinition.cpp
    ${SOURCES_PREFIX}/variablelineedit.cpp
    ${SOURCES_PREFIX}/variableparameterdefinition.cpp
    ${SOURCES_PREFIX}/windowedit.cpp
    ${SOURCES_PREFIX}/windowhandle.cpp
    ${SOURCES_PREFIX}/windowparameterdefinition.cpp
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/keyboardkey_windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keyboardkey_xkb.cpp>
    ${SOURCES_PREFIX}/code/algorithms.cpp
    ${SOURCES_PREFIX}/code/codeclass.cpp
    ${SOURCES_PREFIX}/code/color.cpp
    ${SOURCES_PREFIX}/code/image.cpp
    ${SOURCES_PREFIX}/code/point.cpp
    ${SOURCES_PREFIX}/code/processhandle.cpp
    ${SOURCES_PREFIX}/code/rawdata.cpp
    ${SOURCES_PREFIX}/code/rect.cpp
    ${SOURCES_PREFIX}/code/size.cpp
    ${SOURCES_PREFIX}/code/window.cpp
    ${SOURCES_PREFIX}/qtimagefilters/convolutionfilter.cpp
    ${SOURCES_PREFIX}/qtimagefilters/punchfilter.cpp
    ${SOURCES_PREFIX}/qtimagefilters/qtimagefilter.cpp
    ${SOURCES_PREFIX}/qtimagefilters/qtimagefilterfactory.cpp
    ${SOURCES_PREFIX}/qtsingleapplication/qtlocalpeer.cpp
    ${SOURCES_PREFIX}/qtsingleapplication/qtlockedfile.cpp
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/qtsingleapplication/qtlockedfile_unix.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/qtsingleapplication/qtlockedfile_win.cpp>
    ${SOURCES_PREFIX}/qtsingleapplication/qtsingleapplication.cpp
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/x11info.cpp>
    ${SOURCES_PREFIX}/scriptengine.cpp
)

set(HEADERS_PREFIX include/${PROJECT})
set(HEADERS
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/keysym2ucs.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/x11info.hpp>
    ${HEADERS_PREFIX}/abstractcodeeditor.hpp
    ${HEADERS_PREFIX}/actiondefinition.hpp
    ${HEADERS_PREFIX}/actiondefinitionenums.hpp
    ${HEADERS_PREFIX}/actionexception.hpp
    ${HEADERS_PREFIX}/actionfactory.hpp
    ${HEADERS_PREFIX}/actioninstance.hpp
    ${HEADERS_PREFIX}/actioninstancebuffer.hpp
    ${HEADERS_PREFIX}/actionpack.hpp
    ${HEADERS_PREFIX}/actiontools_global.hpp
    ${HEADERS_PREFIX}/booleanedit.hpp
    ${HEADERS_PREFIX}/booleanparameterdefinition.hpp
    ${HEADERS_PREFIX}/choosepositionpushbutton.hpp
    ${HEADERS_PREFIX}/choosewindowpushbutton.hpp
    ${HEADERS_PREFIX}/code/algorithms.hpp
    ${HEADERS_PREFIX}/code/codeclass.hpp
    ${HEADERS_PREFIX}/code/color.hpp
    ${HEADERS_PREFIX}/code/image.hpp
    ${HEADERS_PREFIX}/code/point.hpp
    ${HEADERS_PREFIX}/code/processhandle.hpp
    ${HEADERS_PREFIX}/code/rawdata.hpp
    ${HEADERS_PREFIX}/code/rect.hpp
    ${HEADERS_PREFIX}/code/size.hpp
    ${HEADERS_PREFIX}/code/window.hpp
    ${HEADERS_PREFIX}/codecombobox.hpp
    ${HEADERS_PREFIX}/codedatetimeedit.hpp
    ${HEADERS_PREFIX}/codeedit.hpp
    ${HEADERS_PREFIX}/codeeditordialog.hpp
    ${HEADERS_PREFIX}/codehighlighter.hpp
    ${HEADERS_PREFIX}/codelineedit.hpp
    ${HEADERS_PREFIX}/codelineeditbutton.hpp
    ${HEADERS_PREFIX}/codespinbox.hpp
    ${HEADERS_PREFIX}/codedoublespinbox.hpp
    ${HEADERS_PREFIX}/coloredit.hpp
    ${HEADERS_PREFIX}/colorparameterdefinition.hpp
    ${HEADERS_PREFIX}/colorpositionparameterdefinition.hpp
    ${HEADERS_PREFIX}/consoletableview.hpp
    ${HEADERS_PREFIX}/consolewidget.hpp
    ${HEADERS_PREFIX}/crossplatform.hpp
    ${HEADERS_PREFIX}/datacopyactioninstance.hpp
    ${HEADERS_PREFIX}/datetimeparameterdefinition.hpp
    ${HEADERS_PREFIX}/devicecopythread.hpp
    ${HEADERS_PREFIX}/element.hpp
    ${HEADERS_PREFIX}/elementdefinition.hpp
    ${HEADERS_PREFIX}/environmentvariableparameterdefinition.hpp
    ${HEADERS_PREFIX}/fileedit.hpp
    ${HEADERS_PREFIX}/fileparameterdefinition.hpp
    ${HEADERS_PREFIX}/groupdefinition.hpp
    ${HEADERS_PREFIX}/helpbutton.hpp
    ${HEADERS_PREFIX}/ifactionparameterdefinition.hpp
    ${HEADERS_PREFIX}/ifactionvalue.hpp
    ${HEADERS_PREFIX}/imagelabel.hpp
    ${HEADERS_PREFIX}/imageparameterdefinition.hpp
    ${HEADERS_PREFIX}/itemlistmodel.hpp
    ${HEADERS_PREFIX}/itemlistview.hpp
    ${HEADERS_PREFIX}/itemlistwidget.hpp
    ${HEADERS_PREFIX}/itemsparameterdefinition.hpp
    ${HEADERS_PREFIX}/keyboardkey.hpp
    ${HEADERS_PREFIX}/keyboardkeyedit.hpp
    ${HEADERS_PREFIX}/keyboardkeyparameterdefinition.hpp
    ${HEADERS_PREFIX}/keyedit.hpp
    ${HEADERS_PREFIX}/keyinput.hpp
    ${HEADERS_PREFIX}/keymapper.hpp
    ${HEADERS_PREFIX}/keyparameterdefinition.hpp
    ${HEADERS_PREFIX}/keysymhelper.hpp
    ${HEADERS_PREFIX}/keywords.hpp
    ${HEADERS_PREFIX}/linecombobox.hpp
    ${HEADERS_PREFIX}/linenumberarea.hpp
    ${HEADERS_PREFIX}/lineparameterdefinition.hpp
    ${HEADERS_PREFIX}/listparameterdefinition.hpp
    ${HEADERS_PREFIX}/localeparameterdefinition.hpp
    ${HEADERS_PREFIX}/matchingpointlist.hpp
    ${HEADERS_PREFIX}/multitextparameterdefinition.hpp
    ${HEADERS_PREFIX}/name.hpp
    ${HEADERS_PREFIX}/numberformat.hpp
    ${HEADERS_PREFIX}/numberparameterdefinition.hpp
    ${HEADERS_PREFIX}/numberrangeparameterdefinition.hpp
    ${HEADERS_PREFIX}/opencvalgorithms.hpp
    ${HEADERS_PREFIX}/opencvalgorithms_private.hpp
    ${HEADERS_PREFIX}/parameter.hpp
    ${HEADERS_PREFIX}/parametercontainer.hpp
    ${HEADERS_PREFIX}/parameterdefinition.hpp
    ${HEADERS_PREFIX}/pointitemdelegate.hpp
    ${HEADERS_PREFIX}/pointlistparameterdefinition.hpp
    ${HEADERS_PREFIX}/pointlistwidget.hpp
    ${HEADERS_PREFIX}/positionedit.hpp
    ${HEADERS_PREFIX}/positionparameterdefinition.hpp
    ${HEADERS_PREFIX}/procedureparameterdefinition.hpp
    ${HEADERS_PREFIX}/qtimagefilters/convolutionfilter.h
    ${HEADERS_PREFIX}/qtimagefilters/gaussfilter.h
    ${HEADERS_PREFIX}/qtimagefilters/punchfilter.h
    ${HEADERS_PREFIX}/qtimagefilters/qtimagefilter.h
    ${HEADERS_PREFIX}/qtimagefilters/qtmatrix.h
    ${HEADERS_PREFIX}/qtsingleapplication/qtlocalpeer.h
    ${HEADERS_PREFIX}/qtsingleapplication/qtlockedfile.h
    ${HEADERS_PREFIX}/qtsingleapplication/qtsingleapplication.h
    ${HEADERS_PREFIX}/registry.hpp
    ${HEADERS_PREFIX}/resource.hpp
    ${HEADERS_PREFIX}/resourcenamedialog.hpp
    ${HEADERS_PREFIX}/savescreenshotwizardpage.hpp
    ${HEADERS_PREFIX}/scopeguard.hpp
    ${HEADERS_PREFIX}/screenpositionwidget.hpp
    ${HEADERS_PREFIX}/screenshooter.hpp
    ${HEADERS_PREFIX}/screenshotpushbutton.hpp
    ${HEADERS_PREFIX}/screenshotwizard.hpp
    ${HEADERS_PREFIX}/screenshotwizardpage.hpp
    ${HEADERS_PREFIX}/script.hpp
    ${HEADERS_PREFIX}/scriptcompleter.hpp
    ${HEADERS_PREFIX}/scriptlinemodel.hpp
    ${HEADERS_PREFIX}/scriptparameter.hpp
    ${HEADERS_PREFIX}/settings.hpp
    ${HEADERS_PREFIX}/subparameter.hpp
    ${HEADERS_PREFIX}/swappairs.hpp
    ${HEADERS_PREFIX}/switchtextmodepushbutton.hpp
    ${HEADERS_PREFIX}/systeminput.hpp
    ${HEADERS_PREFIX}/targetwindow.hpp
    ${HEADERS_PREFIX}/textparameterdefinition.hpp
    ${HEADERS_PREFIX}/variablelineedit.hpp
    ${HEADERS_PREFIX}/variableparameterdefinition.hpp
    ${HEADERS_PREFIX}/windowedit.hpp
    ${HEADERS_PREFIX}/windowhandle.hpp
    ${HEADERS_PREFIX}/windowparameterdefinition.hpp
    ${HEADERS_PREFIX}/scriptengine.hpp
)

set(UI_PREFIX ui)
set(UI_FILES
    ${UI_PREFIX}/booleanedit.ui
    ${UI_PREFIX}/codeeditordialog.ui
    ${UI_PREFIX}/coloredit.ui
    ${UI_PREFIX}/consolewidget.ui
    ${UI_PREFIX}/fileedit.ui
    ${UI_PREFIX}/itemlistwidget.ui
    ${UI_PREFIX}/pointlistwidget.ui
    ${UI_PREFIX}/positionedit.ui
    ${UI_PREFIX}/resourcenamedialog.ui
    ${UI_PREFIX}/savescreenshotwizardpage.ui
    ${UI_PREFIX}/screenshotwizardpage.ui
    ${UI_PREFIX}/windowedit.ui
)

set(TS_PREFIX translations)
set(TS_FILES
    ${TS_PREFIX}/actiontools_fr_FR.ts
    ${TS_PREFIX}/actiontools_zh_CN.ts
)

set(QRC ${PROJECT}.qrc)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${TS_FILES}
    ${QRC}
)

add_dependencies(${PROJECT} tools external_qtjsapi)

if(UNIX)
    include(FindPkgConfig)
    pkg_search_module(LIBXTST REQUIRED xtst)
endif()

target_compile_definitions(${PROJECT} PRIVATE ACTIONTOOLS_LIBRARY)

setup_target(${PROJECT})

find_package(OpenCV REQUIRED core imgproc)
find_package(Qt6 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Widgets Qml REQUIRED)

set(EXTRA_CXX_FLAGS "")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(EXTRA_CXX_FLAGS "-DQT_NO_DEBUG_OUTPUT")
endif()

ExternalProject_Add(external_qtjsapi
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qtjsapi
    BINARY_DIR ${CMAKE_BINARY_DIR}/3rdparty/qtjsapi
    INSTALL_COMMAND cmake -E echo "Skipping install step."
    CMAKE_ARGS
            -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_CXX_FLAGS=${EXTRA_CXX_FLAGS}
)

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/3rdparty/qtjsapi/out/
    DESTINATION ${ACT_LIB_INSTALL_DIR}
)

target_link_directories(${PROJECT}
    PRIVATE
        ${CMAKE_BINARY_DIR}/3rdparty/qtjsapi/out
)

target_include_directories(${PROJECT}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../tools/include
    SYSTEM
        ${LIBXTST_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
)

target_link_libraries(
    ${PROJECT}
    tools
    qtjsapi
    Qt6::Widgets
    Qt6::Qml
    Qt6::QmlPrivate
    ${LIBXTST_LIBRARIES}
    $<$<PLATFORM_ID:Windows>:Psapi>
    ${OpenCV_LIBS}
)

compile_translations(${PROJECT})

if(UNIX)
    # We depend on the qtjsapi library
    set_target_properties(${PROJECT} PROPERTIES INSTALL_RPATH "$ORIGIN")
endif()

install(TARGETS ${PROJECT} DESTINATION ${ACT_LIB_INSTALL_DIR})

