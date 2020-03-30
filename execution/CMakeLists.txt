set(PROJECT execution)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/codeactiona.cpp
    ${SOURCES_PREFIX}/codeexecution.cpp
    ${SOURCES_PREFIX}/codeinitializer.cpp
    ${SOURCES_PREFIX}/codestdio.cpp
    ${SOURCES_PREFIX}/execution.cpp
    ${SOURCES_PREFIX}/executionwindow.cpp
    ${SOURCES_PREFIX}/scriptagent.cpp
)

set(HEADERS_PREFIX include/${PROJECT})
set(HEADERS
    ${HEADERS_PREFIX}/codeactiona.hpp
    ${HEADERS_PREFIX}/codeexecution.hpp
    ${HEADERS_PREFIX}/codeinitializer.hpp
    ${HEADERS_PREFIX}/codestdio.hpp
    ${HEADERS_PREFIX}/execution.hpp
    ${HEADERS_PREFIX}/executionwindow.hpp
    ${HEADERS_PREFIX}/scriptagent.hpp
)

set(QRC ${PROJECT}.qrc)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
    ${QRC}
)

set_target_properties(${PROJECT} PROPERTIES VERSION ${PROJECT_VERSION})

add_dependencies(${PROJECT} actiontools tools)

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

target_compile_definitions(${PROJECT} PRIVATE EXECUTION_LIBRARY)

apply_definitions(${PROJECT})

find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Script ScriptTools Widgets UiTools REQUIRED)

target_link_libraries(${PROJECT} Qt5::Script Qt5::ScriptTools Qt5::Widgets Qt5::UiTools)
