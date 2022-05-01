set(PROJECT tester)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/main.cpp
    ${SOURCES_PREFIX}/mainwindow.cpp
)

set(HEADERS_PREFIX src)
set(HEADERS
    ${HEADERS_PREFIX}/mainwindow.hpp
)

set(UI_PREFIX src)
set(UI
    ${UI_PREFIX}/mainwindow.ui
)

add_executable(${PROJECT}
    ${SOURCES}
    ${HEADERS}
    ${UI}
)

add_dependencies(${PROJECT} backend)

set_target_properties(${PROJECT} PROPERTIES OUTPUT_NAME "backend-tester")
set_target_properties(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/..)

setup_target(${PROJECT})

find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Widgets DBus REQUIRED)

target_include_directories(${PROJECT}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../actiontools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../backend/include
)

target_link_libraries(
    ${PROJECT}
    actiontools
    backend
    Qt5::Widgets
    Qt5::DBus
)

