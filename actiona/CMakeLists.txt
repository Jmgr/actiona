set(PROJECT actiona)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/aboutdialog.cpp
    ${SOURCES_PREFIX}/actiondialog.cpp
    ${SOURCES_PREFIX}/changelogdialog.cpp
    ${SOURCES_PREFIX}/colorpickerpushbutton.cpp
    ${SOURCES_PREFIX}/deleteactionpushbutton.cpp
    ${SOURCES_PREFIX}/filetypeguesser.cpp
    ${SOURCES_PREFIX}/filterlineedit.cpp
    ${SOURCES_PREFIX}/flagscombobox.cpp
    ${SOURCES_PREFIX}/global.cpp
    ${SOURCES_PREFIX}/main.cpp
    ${SOURCES_PREFIX}/mainwindow.cpp
    ${SOURCES_PREFIX}/newactiondialog.cpp
    ${SOURCES_PREFIX}/newactionmodel.cpp
    ${SOURCES_PREFIX}/newactionproxymodel.cpp
    ${SOURCES_PREFIX}/newactiontreewidget.cpp
    ${SOURCES_PREFIX}/progresssplashscreen.cpp
    ${SOURCES_PREFIX}/resourcedialog.cpp
    ${SOURCES_PREFIX}/resourcenamedelegate.cpp
    ${SOURCES_PREFIX}/resourcetablewidget.cpp
    ${SOURCES_PREFIX}/resourcetypedelegate.cpp
    ${SOURCES_PREFIX}/scriptcontentdialog.cpp
    ${SOURCES_PREFIX}/scriptmodel.cpp
    ${SOURCES_PREFIX}/scriptmodelundocommands.cpp
    ${SOURCES_PREFIX}/scriptparametersdialog.cpp
    ${SOURCES_PREFIX}/scriptproxymodel.cpp
    ${SOURCES_PREFIX}/scriptsettingsdialog.cpp
    ${SOURCES_PREFIX}/scripttreeview.cpp
    ${SOURCES_PREFIX}/settingsdialog.cpp
    ${SOURCES_PREFIX}/settingskeyedit.cpp
    ${SOURCES_PREFIX}/sfxscriptdialog.cpp
    ${SOURCES_PREFIX}/QHotkey/qhotkey.cpp
    ${SOURCES_PREFIX}/themeselection.cpp
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/QHotkey/qhotkey_win.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/QHotkey/qhotkey_x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/3rdparty/breakpad/src/client/windows/crash_generation/crash_generation_client.cc>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/3rdparty/breakpad/src/client/windows/handler/exception_handler.cc>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/3rdparty/breakpad/src/common/windows/guid_string.cc>
)

set(HEADERS_PREFIX src)
set(HEADERS
    ${HEADERS_PREFIX}/aboutdialog.hpp
    ${HEADERS_PREFIX}/actiondialog.hpp
    ${HEADERS_PREFIX}/actionfilteringflags.hpp
    ${HEADERS_PREFIX}/changelogdialog.hpp
    ${HEADERS_PREFIX}/colorpickerpushbutton.hpp
    ${HEADERS_PREFIX}/deleteactionpushbutton.hpp
    ${HEADERS_PREFIX}/filetypeguesser.hpp
    ${HEADERS_PREFIX}/filterlineedit.hpp
    ${HEADERS_PREFIX}/flagscombobox.hpp
    ${HEADERS_PREFIX}/global.hpp
    ${HEADERS_PREFIX}/heatmapmode.hpp
    ${HEADERS_PREFIX}/mainwindow.hpp
    ${HEADERS_PREFIX}/newactiondialog.hpp
    ${HEADERS_PREFIX}/newactionmodel.hpp
    ${HEADERS_PREFIX}/newactionproxymodel.hpp
    ${HEADERS_PREFIX}/newactiontreewidget.hpp
    ${HEADERS_PREFIX}/progresssplashscreen.hpp
    ${HEADERS_PREFIX}/resourcedialog.hpp
    ${HEADERS_PREFIX}/resourcenamedelegate.hpp
    ${HEADERS_PREFIX}/resourcesizeitem.hpp
    ${HEADERS_PREFIX}/resourcetablewidget.hpp
    ${HEADERS_PREFIX}/resourcetypedelegate.hpp
    ${HEADERS_PREFIX}/scriptcontentdialog.hpp
    ${HEADERS_PREFIX}/scriptmodel.hpp
    ${HEADERS_PREFIX}/scriptmodelundocommands.hpp
    ${HEADERS_PREFIX}/scriptparametersdialog.hpp
    ${HEADERS_PREFIX}/scriptproxymodel.hpp
    ${HEADERS_PREFIX}/scriptsettingsdialog.hpp
    ${HEADERS_PREFIX}/scripttreeview.hpp
    ${HEADERS_PREFIX}/settingsdialog.hpp
    ${HEADERS_PREFIX}/settingskeyedit.hpp
    ${HEADERS_PREFIX}/sfxscriptdialog.hpp
    ${HEADERS_PREFIX}/QHotkey/qhotkey.h
    ${HEADERS_PREFIX}/QHotkey/qhotkey_p.h
    ${HEADERS_PREFIX}/themeselection.hpp
)

set(UI_PREFIX ui)
set(UI_FILES
    ${UI_PREFIX}/aboutdialog.ui
    ${UI_PREFIX}/actiondialog.ui
    ${UI_PREFIX}/changelogdialog.ui
    ${UI_PREFIX}/mainwindow.ui
    ${UI_PREFIX}/newactiondialog.ui
    ${UI_PREFIX}/resourcedialog.ui
    ${UI_PREFIX}/scriptcontentdialog.ui
    ${UI_PREFIX}/scriptparametersdialog.ui
    ${UI_PREFIX}/scriptsettingsdialog.ui
    ${UI_PREFIX}/settingsdialog.ui
    ${UI_PREFIX}/sfxscriptdialog.ui
)

set(TS_PREFIX translations)
set(TS_FILES
    ${TS_PREFIX}/actiona_fr_FR.ts
    ${TS_PREFIX}/actiona_zh_CN.ts
)

set(QRC ${PROJECT}.qrc)

add_executable(${PROJECT}
    WIN32
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${TS_FILES}
    ${QRC}
    app.rc
    3rdparty/QDarkStyleSheet/qdarkstyle/dark/darkstyle.qrc
    3rdparty/QDarkStyleSheet/qdarkstyle/light/lightstyle.qrc
)

add_dependencies(${PROJECT} execution actiontools tools actionpackdata actionpackdevice actionpackinternal actionpacksystem actionpackwindows)

if(UNIX)
    include(FindPkgConfig)
    pkg_search_module(LIBNOTIFY REQUIRED libnotify)
    pkg_search_module(LIBX11 REQUIRED x11)
endif()

setup_target(${PROJECT})

find_package(Qt6 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Qml Network Widgets Core5Compat Multimedia TextToSpeech REQUIRED)

target_link_directories(${PROJECT}
    PRIVATE
        ${CMAKE_BINARY_DIR}/3rdparty/qtjsapi/out
)

target_include_directories(${PROJECT}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../tools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../actiontools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../execution/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../actions/data/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../actions/device/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../actions/internal/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../actions/system/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../actions/windows/src
        $<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/breakpad/src>
        ${LIBNOTIFY_INCLUDE_DIRS}
        ${LIBX11_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT}
    execution
    actiontools
    tools
    actionpackdata
    actionpackdevice
    actionpackinternal
    actionpacksystem
    actionpackwindows
    Qt6::Network
    Qt6::Widgets
    Qt6::Core5Compat
    Qt6::Multimedia
    Qt6::TextToSpeech
    ${LIBNOTIFY_LIBRARIES}
    ${LIBX11_LIBRARIES}
    $<$<PLATFORM_ID:Windows>:shlwapi>
)

compile_translations(${PROJECT})

if(UNIX)
    set_target_properties(${PROJECT} PROPERTIES INSTALL_RPATH "$ORIGIN/../lib/actiona:$ORIGIN/../lib/actiona/actions")
endif()

install(TARGETS ${PROJECT} DESTINATION bin)
