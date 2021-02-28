set(PROJECT backend)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/backend.cpp
    ${SOURCES_PREFIX}/keyinput.cpp
    ${SOURCES_PREFIX}/mouse-autoreleaser.cpp
    ${SOURCES_PREFIX}/keyboard-autoreleaser.cpp
    ${SOURCES_PREFIX}/keyboard-output.cpp
    ${SOURCES_PREFIX}/mouse-output.cpp
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keyinput-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/keyinput-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keysymhelper-x11.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keymapper-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/keymapper-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/mouse-input-x11.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/mouse-output-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/mouse-input-windows.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/mouse-output-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keyboard-input-x11.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keyboard-output-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/keyboard-input-windows.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/keyboard-output-windows.cpp>
)

set(HEADERS_PREFIX include/${PROJECT})
set(HEADERS
    ${HEADERS_PREFIX}/backend_global.hpp
    ${HEADERS_PREFIX}/mouse-input.hpp
    ${HEADERS_PREFIX}/mouse-output.hpp
    ${HEADERS_PREFIX}/keyboard-input.hpp
    ${HEADERS_PREFIX}/keyboard-output.hpp
    ${HEADERS_PREFIX}/backend.hpp
    ${HEADERS_PREFIX}/keymapper.hpp
    ${HEADERS_PREFIX}/keyinput.hpp
    ${HEADERS_PREFIX}/mouse-autoreleaser.hpp
    ${HEADERS_PREFIX}/keyboard-autoreleaser.hpp
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/keysymhelper-x11.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/mouse-input-x11.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/mouse-output-x11.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/mouse-input-windows.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/mouse-output-windows.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/keyboard-input-x11.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/keyboard-output-x11.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/keyboard-input-windows.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/keyboard-output-windows.hpp>
)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(${PROJECT} PRIVATE BACKEND_LIBRARY)

setup_target(${PROJECT})

find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Network Widgets REQUIRED)

target_include_directories(${PROJECT}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
    ${PROJECT}
    Qt5::Network # TODO
    Qt5::Widgets
)

if(UNIX)
    find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS X11Extras REQUIRED)
    target_link_libraries(${PROJECT} Qt5::X11Extras)
endif()

compile_translations(${PROJECT})
