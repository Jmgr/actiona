set(PROJECT backend)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/backend.cpp
    ${SOURCES_PREFIX}/keyinput.cpp
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/process-unix.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/process-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/mouse-x11.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keyboard-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/mouse-windows.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/keyboard-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keyinput-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/keyinput-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keysymhelper-x11.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keymapper-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/keymapper-windows.cpp>
)

set(HEADERS_PREFIX include/${PROJECT})
set(HEADERS
    ${HEADERS_PREFIX}/backend_global.hpp
    ${HEADERS_PREFIX}/backend.hpp
    ${HEADERS_PREFIX}/keymapper.hpp
    ${HEADERS_PREFIX}/keyinput.hpp
    ${HEADERS_PREFIX}/mouse.hpp
    ${HEADERS_PREFIX}/keyboard.hpp
    ${HEADERS_PREFIX}/process.hpp
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/process-unix.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/process-windows.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/mouse-x11.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/keyboard-x11.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/mouse-windows.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/keyboard-windows.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/keysymhelper-x11.hpp>
)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(${PROJECT} PRIVATE BACKEND_LIBRARY)

setup_target(${PROJECT})

find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Gui REQUIRED)

if(UNIX AND NOT APPLE)
    find_package(X11 COMPONENTS Xtst REQUIRED)
endif()

target_include_directories(${PROJECT}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${X11_Xtst_INCLUDE_PATH}
)

target_link_libraries(
    ${PROJECT}
    Qt5::Gui
    ${X11_Xtst_LIB}
)

if(UNIX AND NOT APPLE)
    find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS X11Extras REQUIRED)
    target_link_libraries(${PROJECT} Qt5::X11Extras)
endif()

compile_translations(${PROJECT})
