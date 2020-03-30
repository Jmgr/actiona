set(PROJECT actionpackdevice)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/keyboarddevice.cpp
    ${SOURCES_PREFIX}/mousedevice.cpp
    ${SOURCES_PREFIX}/actions/clickinstance.cpp
    ${SOURCES_PREFIX}/actions/cursorpathinstance.cpp
    ${SOURCES_PREFIX}/actions/keyboardkeyconditioninstance.cpp
    ${SOURCES_PREFIX}/actions/keyinstance.cpp
    ${SOURCES_PREFIX}/actions/textinstance.cpp
    ${SOURCES_PREFIX}/code/keyboard.cpp
    ${SOURCES_PREFIX}/code/mouse.cpp
)

set(HEADERS_PREFIX src)
set(HEADERS
    ${HEADERS_PREFIX}/actionpackdevice.hpp
    ${HEADERS_PREFIX}/keyboarddevice.hpp
    ${HEADERS_PREFIX}/mousedevice.hpp
    ${HEADERS_PREFIX}/actions/clickdefinition.hpp
    ${HEADERS_PREFIX}/actions/clickinstance.hpp
    ${HEADERS_PREFIX}/actions/cursorpathdefinition.hpp
    ${HEADERS_PREFIX}/actions/cursorpathinstance.hpp
    ${HEADERS_PREFIX}/actions/keyboardkeyconditiondefinition.hpp
    ${HEADERS_PREFIX}/actions/keyboardkeyconditioninstance.hpp
    ${HEADERS_PREFIX}/actions/keydefinition.hpp
    ${HEADERS_PREFIX}/actions/keyinstance.hpp
    ${HEADERS_PREFIX}/actions/movecursordefinition.hpp
    ${HEADERS_PREFIX}/actions/movecursorinstance.hpp
    ${HEADERS_PREFIX}/actions/textdefinition.hpp
    ${HEADERS_PREFIX}/actions/textinstance.hpp
    ${HEADERS_PREFIX}/actions/wheeldefinition.hpp
    ${HEADERS_PREFIX}/actions/wheelinstance.hpp
    ${HEADERS_PREFIX}/code/keyboard.hpp
    ${HEADERS_PREFIX}/code/mouse.hpp
)

set(QRC ${PROJECT}.qrc)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
    ${QRC}
)

set_target_properties(${PROJECT} PROPERTIES VERSION ${PROJECT_VERSION})

add_dependencies(${PROJECT} tools actiontools)

target_include_directories(${PROJECT}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../actiontools/include
)

apply_definitions(${PROJECT})

find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS Network Script Sql Widgets REQUIRED)

target_link_libraries(${PROJECT} Qt5::Network Qt5::Script Qt5::Sql Qt5::Widgets)

if(UNIX)
    find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS X11Extras REQUIRED)
    target_link_libraries(${PROJECT} Qt5::X11Extras)
endif()