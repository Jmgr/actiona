set(PROJECT executer)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/codeexecuter.cpp
    ${SOURCES_PREFIX}/executer.cpp
    ${SOURCES_PREFIX}/mainclass.cpp
    ${SOURCES_PREFIX}/main.cpp
    ${SOURCES_PREFIX}/scriptexecuter.cpp
)

set(HEADERS_PREFIX src)
set(HEADERS
    ${HEADERS_PREFIX}/codeexecuter.hpp
    ${HEADERS_PREFIX}/executer.hpp
    ${HEADERS_PREFIX}/mainclass.hpp
    ${HEADERS_PREFIX}/scriptexecuter.hpp
)

set(TS_PREFIX translations)
set(TS_FILES
    ${TS_PREFIX}/executer_fr_FR.ts
    ${TS_PREFIX}/executer_zh_CN.ts
)

add_executable(${PROJECT}
    WIN32
    ${SOURCES}
    ${HEADERS}
    ${TS_FILES}
    ../gui/app.rc
)

add_dependencies(${PROJECT} execution actiontools tools)

if(UNIX)
    include(FindPkgConfig)
    pkg_search_module(LIBNOTIFY REQUIRED libnotify)
endif()

set_target_properties(${PROJECT} PROPERTIES OUTPUT_NAME "actexec")
set_target_properties(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..)

setup_target(${PROJECT})

find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Network Script ScriptTools Widgets REQUIRED)

target_include_directories(${PROJECT}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../tools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../actiontools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../execution/include
        ${LIBNOTIFY_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT}
    execution
    actiontools
    tools
    Qt5::Network
    Qt5::Script
    Qt5::ScriptTools
    Qt5::Widgets
    ${LIBNOTIFY_LIBRARIES}
)

compile_translations(${PROJECT})
