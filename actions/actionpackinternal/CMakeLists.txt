set(PROJECT actionpackinternal)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/actions/consoleinstance.cpp
    ${SOURCES_PREFIX}/actions/pauseinstance.cpp
    ${SOURCES_PREFIX}/actions/variableconditioninstance.cpp
    ${SOURCES_PREFIX}/actions/variableinstance.cpp
)

set(HEADERS_PREFIX src)
set(HEADERS
    ${HEADERS_PREFIX}/actionpackinternal.hpp
    ${HEADERS_PREFIX}/actions/beginproceduredefinition.hpp
    ${HEADERS_PREFIX}/actions/beginprocedureinstance.hpp
    ${HEADERS_PREFIX}/actions/callproceduredefinition.hpp
    ${HEADERS_PREFIX}/actions/callprocedureinstance.hpp
    ${HEADERS_PREFIX}/actions/codedefinition.hpp
    ${HEADERS_PREFIX}/actions/codeinstance.hpp
    ${HEADERS_PREFIX}/actions/consoledefinition.hpp
    ${HEADERS_PREFIX}/actions/consoleinstance.hpp
    ${HEADERS_PREFIX}/actions/endproceduredefinition.hpp
    ${HEADERS_PREFIX}/actions/endprocedureinstance.hpp
    ${HEADERS_PREFIX}/actions/exitdefinition.hpp
    ${HEADERS_PREFIX}/actions/exitinstance.hpp
    ${HEADERS_PREFIX}/actions/gotodefinition.hpp
    ${HEADERS_PREFIX}/actions/gotoinstance.hpp
    ${HEADERS_PREFIX}/actions/loopdefinition.hpp
    ${HEADERS_PREFIX}/actions/loopinstance.hpp
    ${HEADERS_PREFIX}/actions/noopdefinition.hpp
    ${HEADERS_PREFIX}/actions/noopinstance.hpp
    ${HEADERS_PREFIX}/actions/pausedefinition.hpp
    ${HEADERS_PREFIX}/actions/pauseinstance.hpp
    ${HEADERS_PREFIX}/actions/stopdefinition.hpp
    ${HEADERS_PREFIX}/actions/stopinstance.hpp
    ${HEADERS_PREFIX}/actions/timeconditiondefinition.hpp
    ${HEADERS_PREFIX}/actions/timeconditioninstance.hpp
    ${HEADERS_PREFIX}/actions/variableconditiondefinition.hpp
    ${HEADERS_PREFIX}/actions/variableconditioninstance.hpp
    ${HEADERS_PREFIX}/actions/variabledefinition.hpp
    ${HEADERS_PREFIX}/actions/variableinstance.hpp
)

set(QRC ${PROJECT}.qrc)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
    ${QRC}
)

set_target_properties(${PROJECT} PROPERTIES VERSION ${PROJECT_VERSION})

add_dependencies(${PROJECT} tools actiontools execution)

target_include_directories(${PROJECT}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../actiontools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../execution/include
)

apply_definitions(${PROJECT})

find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Network Script Sql Widgets REQUIRED)

target_link_libraries(${PROJECT} Qt5::Network Qt5::Script Qt5::Sql Qt5::Widgets)