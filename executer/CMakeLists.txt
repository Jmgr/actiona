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

add_executable(${PROJECT}
    ${SOURCES}
    ${HEADERS}
)

add_dependencies(${PROJECT} execution actiontools tools)

if(UNIX)
    include(FindPkgConfig)
    pkg_search_module(LIBNOTIFY REQUIRED libnotify)
endif()

target_include_directories(${PROJECT}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../tools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../actiontools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../execution/include
        ${LIBNOTIFY_INCLUDE_DIRS}
)

set_target_properties(${PROJECT} PROPERTIES OUTPUT_NAME "actexec")

setup_target(${PROJECT})

find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Network Script ScriptTools Widgets REQUIRED)

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