set(PROJECT backend)

set(SOURCES_PREFIX src)
set(SOURCES
    $<$<PLATFORM_ID:Linux>:${SOURCES_PREFIX}/mouse-input-x11.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/mouse-input-windows.cpp>
)

set(HEADERS_PREFIX include/${PROJECT})
set(HEADERS
    ${HEADERS_PREFIX}/backend_global.hpp
    ${HEADERS_PREFIX}/mouse-input.hpp
    ${HEADERS_PREFIX}/mouse.hpp
    $<$<PLATFORM_ID:Linux>:${HEADERS_PREFIX}/mouse-input-x11.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/mouse-input-windows.hpp>
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
