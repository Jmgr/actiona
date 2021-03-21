set(PROJECT gui)

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
)

set(QRC ${PROJECT}.qrc)

#TODO: add UI files
add_executable(${PROJECT}
    ${SOURCES}
    ${HEADERS}
    ${QRC}
)

add_dependencies(${PROJECT} execution actiontools tools)

if(UNIX AND NOT APPLE)
    include(FindPkgConfig)
    pkg_search_module(LIBNOTIFY REQUIRED libnotify)
endif()

set_target_properties(${PROJECT} PROPERTIES OUTPUT_NAME "actiona")
set_target_properties(${PROJECT} PROPERTIES SUFFIX "")
set_target_properties(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..)
set(CMAKE_EXECUTABLE_SUFFIX "")
set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")

add_subdirectory(src/3rdparty/singleapplication)
add_subdirectory(src/3rdparty/qhotkey)

setup_target(${PROJECT})

find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Network Script Widgets REQUIRED)

if(UNIX AND NOT APPLE)
    find_package(X11)
endif()

target_include_directories(${PROJECT}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty
        ${CMAKE_CURRENT_SOURCE_DIR}/../backend/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../tools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../actiontools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../execution/include
        ${LIBNOTIFY_INCLUDE_DIRS}
        ${X11_INCLUDE_DIR}
)

target_link_libraries(
    ${PROJECT}
    backend
    tools
    actiontools
    execution
    Qt5::Network
    Qt5::Script
    Qt5::Widgets
    SingleApplication::SingleApplication
    qhotkey
    ${LIBNOTIFY_LIBRARIES}
    ${X11_LIBRARIES}
    $<$<PLATFORM_ID:Windows>:shlwapi>
)

if(UNIX AND NOT APPLE)
    find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS X11Extras REQUIRED)
    target_link_libraries(${PROJECT} Qt5::X11Extras)
endif()
if(WIN32)
    find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS WinExtras REQUIRED)
    target_link_libraries(${PROJECT} Qt5::WinExtras)
endif()

compile_translations(${PROJECT})
