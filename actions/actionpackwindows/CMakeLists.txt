set(PROJECT actionpackwindows)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/actions/datainputinstance.cpp
    ${SOURCES_PREFIX}/actions/messageboxinstance.cpp
    ${SOURCES_PREFIX}/actions/multidatainputinstance.cpp
    ${SOURCES_PREFIX}/actions/windowconditioninstance.cpp
    ${SOURCES_PREFIX}/actions/windowinstance.cpp
    ${SOURCES_PREFIX}/code/basewindow.cpp
    ${SOURCES_PREFIX}/code/colordialog.cpp
    ${SOURCES_PREFIX}/code/filedialog.cpp
    ${SOURCES_PREFIX}/code/inputdialog.cpp
    ${SOURCES_PREFIX}/code/messagebox.cpp
    ${SOURCES_PREFIX}/code/progressdialog.cpp
)

set(HEADERS_PREFIX src)
set(HEADERS
    ${HEADERS_PREFIX}/actionpackwindows.hpp
    ${HEADERS_PREFIX}/actions/datainputdefinition.hpp
    ${HEADERS_PREFIX}/actions/datainputinstance.hpp
    ${HEADERS_PREFIX}/actions/messageboxdefinition.hpp
    ${HEADERS_PREFIX}/actions/messageboxinstance.hpp
    ${HEADERS_PREFIX}/actions/multidatainputdefinition.hpp
    ${HEADERS_PREFIX}/actions/multidatainputinstance.hpp
    ${HEADERS_PREFIX}/actions/windowconditiondefinition.hpp
    ${HEADERS_PREFIX}/actions/windowconditioninstance.hpp
    ${HEADERS_PREFIX}/actions/windowdefinition.hpp
    ${HEADERS_PREFIX}/actions/windowinstance.hpp
    ${HEADERS_PREFIX}/code/basewindow.hpp
    ${HEADERS_PREFIX}/code/colordialog.hpp
    ${HEADERS_PREFIX}/code/filedialog.hpp
    ${HEADERS_PREFIX}/code/inputdialog.hpp
    ${HEADERS_PREFIX}/code/messagebox.hpp
    ${HEADERS_PREFIX}/code/progressdialog.hpp
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