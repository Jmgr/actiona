set(PROJECT actexec)

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
    ${TS_PREFIX}/actexec_fr_FR.ts
    ${TS_PREFIX}/actexec_zh_CN.ts
)

add_executable(${PROJECT}
    WIN32
    ${SOURCES}
    ${HEADERS}
    ${TS_FILES}
    ../actiona/app.rc
)

add_dependencies(${PROJECT} execution actiontools tools actionpackdata actionpackdevice actionpackinternal actionpacksystem actionpackwindows)

if(UNIX)
    include(FindPkgConfig)
    pkg_search_module(LIBNOTIFY REQUIRED libnotify)
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
        ${LIBNOTIFY_INCLUDE_DIRS}
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
)

compile_translations(${PROJECT})

if(UNIX)
    set_target_properties(${PROJECT} PROPERTIES INSTALL_RPATH "$ORIGIN/../lib/actiona:$ORIGIN/../lib/actiona/actions")
endif()

install(TARGETS ${PROJECT} DESTINATION bin)
