set(PROJECT backend)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/backend.cpp
    ${SOURCES_PREFIX}/keyinput.cpp
    ${SOURCES_PREFIX}/windowhandle.cpp
    ${SOURCES_PREFIX}/mouse.cpp
    ${SOURCES_PREFIX}/mouse-qt.cpp
    ${SOURCES_PREFIX}/capabilities.cpp
    ${SOURCES_PREFIX}/keyboard.cpp
    ${SOURCES_PREFIX}/process.cpp
    ${SOURCES_PREFIX}/windowing.cpp
    ${SOURCES_PREFIX}/system.cpp
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/backend-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/system-unix.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/system-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/windowing-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/windowing-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/process-unix.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/process-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/mouse-xtest.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keyboard-xtest.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/mouse-windows.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/keyboard-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keyinput-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/keyinput-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keysymhelper-x11.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/keymapper-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/keymapper-windows.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/positionchooser-x11.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/areachooser-x11.cpp>
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/windowchooser-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/positionchooser-windows.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/areachooser-windows.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/windowchooser-windows.cpp>
)

set(HEADERS_PREFIX include/${PROJECT})
set(HEADERS
    ${HEADERS_PREFIX}/backend_global.hpp
    ${HEADERS_PREFIX}/backend.hpp
    ${HEADERS_PREFIX}/keymapper.hpp
    ${HEADERS_PREFIX}/keyinput.hpp
    ${HEADERS_PREFIX}/mouse.hpp
    ${HEADERS_PREFIX}/mouse-qt.hpp
    ${HEADERS_PREFIX}/keyboard.hpp
    ${HEADERS_PREFIX}/process.hpp
    ${HEADERS_PREFIX}/windowing.hpp
    ${HEADERS_PREFIX}/system.hpp
    ${HEADERS_PREFIX}/scopeguard.hpp
    ${HEADERS_PREFIX}/3rdparty/scope_guard/scope_guard.hpp
    ${HEADERS_PREFIX}/windowhandle.hpp
    ${HEADERS_PREFIX}/3rdparty/functional_unwrap.hpp
    ${HEADERS_PREFIX}/feature.hpp
    ${HEADERS_PREFIX}/capabilities.hpp
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/backend-windows.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/system-unix.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/system-windows.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/windowing-x11.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/windowing-windows.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/process-unix.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/process-windows.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/mouse-xtest.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/keyboard-xtest.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/mouse-windows.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/keyboard-windows.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/keysymhelper-x11.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/positionchooser-x11.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/areachooser-x11.hpp>
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/windowchooser-x11.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/positionchooser-windows.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/areachooser-windows.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/windowchooser-windows.hpp>
)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
)

target_compile_definitions(${PROJECT} PRIVATE BACKEND_LIBRARY)

setup_target(${PROJECT})

find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Widgets REQUIRED)

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
    Qt5::Widgets
    ${X11_Xtst_LIB}
    ${X11_Xtest_LIB} # allow old cmake to compile this
    $<$<PLATFORM_ID:Windows>:PowrProf>
)

if(UNIX AND NOT APPLE)
    find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS X11Extras DBus REQUIRED)
    target_link_libraries(${PROJECT} Qt5::X11Extras Qt5::DBus)
endif()

compile_translations(${PROJECT})

add_subdirectory(test)

if(UNIX AND NOT APPLE)
    add_subdirectory(3rdparty/xrectsel)
endif()
