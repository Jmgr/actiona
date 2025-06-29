set(PROJECT tools)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/highresolutiontimer.cpp
    ${SOURCES_PREFIX}/languages.cpp
    $<$<BOOL:${ACT_UPDATER}>:${SOURCES_PREFIX}/updater.cpp>
    $<$<PLATFORM_ID:Windows>:${SOURCES_PREFIX}/sevenziparchivewrite.cpp>
)

set(HEADERS_PREFIX include/${PROJECT})
set(HEADERS
    ${HEADERS_PREFIX}/highresolutiontimer.hpp
    ${HEADERS_PREFIX}/languages.hpp
    ${HEADERS_PREFIX}/stringlistpair.hpp
    $<$<BOOL:${ACT_UPDATER}>:${HEADERS_PREFIX}/updater.hpp>
    $<$<PLATFORM_ID:Windows>:${HEADERS_PREFIX}/sevenziparchivewrite.hpp>
)

set(TS_PREFIX translations)
set(TS_FILES
    ${TS_PREFIX}/tools_fr_FR.ts
    ${TS_PREFIX}/tools_zh_CN.ts
)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
    ${TS_FILES}
)

target_compile_definitions(${PROJECT} PRIVATE TOOLS_LIBRARY)

setup_target(${PROJECT})

find_package(Qt6 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Network Widgets REQUIRED)

target_include_directories(${PROJECT}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(
    ${PROJECT}
    Qt6::Network
    Qt6::Widgets
)

compile_translations(${PROJECT})

install(TARGETS ${PROJECT} DESTINATION ${ACT_LIB_INSTALL_DIR})
