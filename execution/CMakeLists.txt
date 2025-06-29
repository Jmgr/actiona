set(PROJECT execution)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/codeactiona.cpp
    ${SOURCES_PREFIX}/codeexecution.cpp
    ${SOURCES_PREFIX}/codeinitializer.cpp
    ${SOURCES_PREFIX}/codestdio.cpp
    ${SOURCES_PREFIX}/execution.cpp
    ${SOURCES_PREFIX}/executionwindow.cpp
    ${SOURCES_PREFIX}/codescript.cpp
    ${SOURCES_PREFIX}/codeconsole.cpp
)

set(HEADERS_PREFIX include/${PROJECT})
set(HEADERS
    ${HEADERS_PREFIX}/codeactiona.hpp
    ${HEADERS_PREFIX}/codeexecution.hpp
    ${HEADERS_PREFIX}/codeinitializer.hpp
    ${HEADERS_PREFIX}/codestdio.hpp
    ${HEADERS_PREFIX}/execution.hpp
    ${HEADERS_PREFIX}/executionwindow.hpp
    ${HEADERS_PREFIX}/codescript.hpp
    ${HEADERS_PREFIX}/codeconsole.hpp
)

set(UI_PREFIX ui)
set(UI_FILES
    ${UI_PREFIX}/executionwindow.ui
)

set(TS_PREFIX translations)
set(TS_FILES
    ${TS_PREFIX}/execution_fr_FR.ts
    ${TS_PREFIX}/execution_zh_CN.ts
)

set(QRC ${PROJECT}.qrc)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${TS_FILES}
    ${QRC}
)

add_dependencies(${PROJECT} actiontools tools)

target_compile_definitions(${PROJECT} PRIVATE EXECUTION_LIBRARY)

setup_target(${PROJECT})

find_package(Qt6 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Widgets UiTools Qml REQUIRED)

target_link_directories(${PROJECT}
    PRIVATE
        ${CMAKE_BINARY_DIR}/3rdparty/qtjsapi/out
)

target_include_directories(${PROJECT}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT}>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../tools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../actiontools/include
)

target_link_libraries(${PROJECT} actiontools Qt6::Widgets Qt6::UiTools Qt6::Qml)

compile_translations(${PROJECT})

install(TARGETS ${PROJECT} DESTINATION ${ACT_LIB_INSTALL_DIR})
