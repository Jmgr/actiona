set(PROJECT actionpacksystem)

set(SOURCES_PREFIX src)
set(SOURCES
    ${SOURCES_PREFIX}/systemsession.cpp
    ${SOURCES_PREFIX}/actions/findimagedefinition.cpp
    ${SOURCES_PREFIX}/actions/findimageinstance.cpp
    ${SOURCES_PREFIX}/actions/killprocessinstance.cpp
    ${SOURCES_PREFIX}/actions/notifyinstance.cpp
    ${SOURCES_PREFIX}/actions/pixelcolorinstance.cpp
    ${SOURCES_PREFIX}/actions/playsoundinstance.cpp
    ${SOURCES_PREFIX}/actions/systeminstance.cpp
    ${SOURCES_PREFIX}/code/mediaplaylist.cpp
    ${SOURCES_PREFIX}/code/notify.cpp
    ${SOURCES_PREFIX}/code/process.cpp
    ${SOURCES_PREFIX}/code/system.cpp
    ${SOURCES_PREFIX}/systeminfo/qbatteryinfo.cpp
    ${SOURCES_PREFIX}/systeminfo/qdeviceinfo.cpp
    ${SOURCES_PREFIX}/systeminfo/qnetworkinfo.cpp
    ${SOURCES_PREFIX}/systeminfo/qscreensaver.cpp
    ${SOURCES_PREFIX}/systeminfo/qstorageinfo.cpp
)

if(UNIX)
    list(APPEND SOURCES
        ${SOURCES_PREFIX}/systeminfo/linux/qbatteryinfo_linux.cpp
        ${SOURCES_PREFIX}/systeminfo/linux/qdeviceinfo_linux.cpp
        ${SOURCES_PREFIX}/systeminfo/linux/qnetworkinfo_linux.cpp
        ${SOURCES_PREFIX}/systeminfo/linux/qofonowrapper.cpp
        ${SOURCES_PREFIX}/systeminfo/linux/qscreensaver_linux.cpp
        ${SOURCES_PREFIX}/systeminfo/linux/qstorageinfo_linux.cpp
        ${SOURCES_PREFIX}/systeminfo/linux/qudevwrapper.cpp
    )
endif()

if(WIN32)
    list(APPEND SOURCES
        ${SOURCES_PREFIX}/systeminfo/windows/qbatteryinfo_win.cpp
        ${SOURCES_PREFIX}/systeminfo/windows/qdeviceinfo_win.cpp
        ${SOURCES_PREFIX}/systeminfo/windows/qnetworkinfo_win.cpp
        ${SOURCES_PREFIX}/systeminfo/windows/qscreensaver_win.cpp
        ${SOURCES_PREFIX}/systeminfo/windows/qstorageinfo_win.cpp
        ${SOURCES_PREFIX}/systeminfo/windows/qwmihelper_win.cpp
    )
endif()

set(HEADERS_PREFIX src)
set(HEADERS
    ${HEADERS_PREFIX}/actionpacksystem.hpp
    ${HEADERS_PREFIX}/systemsession.hpp
    ${HEADERS_PREFIX}/actions/commanddefinition.hpp
    ${HEADERS_PREFIX}/actions/commandinstance.hpp
    ${HEADERS_PREFIX}/actions/detachedcommanddefinition.hpp
    ${HEADERS_PREFIX}/actions/detachedcommandinstance.hpp
    ${HEADERS_PREFIX}/actions/findimagedefinition.hpp
    ${HEADERS_PREFIX}/actions/findimageinstance.hpp
    ${HEADERS_PREFIX}/actions/killprocessdefinition.hpp
    ${HEADERS_PREFIX}/actions/killprocessinstance.hpp
    ${HEADERS_PREFIX}/actions/notifydefinition.hpp
    ${HEADERS_PREFIX}/actions/notifyinstance.hpp
    ${HEADERS_PREFIX}/actions/openurldefinition.hpp
    ${HEADERS_PREFIX}/actions/openurlinstance.hpp
    ${HEADERS_PREFIX}/actions/pixelcolordefinition.hpp
    ${HEADERS_PREFIX}/actions/pixelcolorinstance.hpp
    ${HEADERS_PREFIX}/actions/playsounddefinition.hpp
    ${HEADERS_PREFIX}/actions/playsoundinstance.hpp
    ${HEADERS_PREFIX}/actions/systemdefinition.hpp
    ${HEADERS_PREFIX}/actions/systeminstance.hpp
    ${HEADERS_PREFIX}/actions/texttospeechdefinition.hpp
    ${HEADERS_PREFIX}/actions/texttospeechinstance.hpp
    ${HEADERS_PREFIX}/code/mediaplaylist.hpp
    ${HEADERS_PREFIX}/code/notify.hpp
    ${HEADERS_PREFIX}/code/process.hpp
    ${HEADERS_PREFIX}/code/system.hpp
    ${HEADERS_PREFIX}/systeminfo/qbatteryinfo.h
    ${HEADERS_PREFIX}/systeminfo/qdeviceinfo.h
    ${HEADERS_PREFIX}/systeminfo/qnetworkinfo.h
    ${HEADERS_PREFIX}/systeminfo/qscreensaver.h
    ${HEADERS_PREFIX}/systeminfo/qstorageinfo.h
    ${HEADERS_PREFIX}/systeminfo/qsysteminfoglobal.h
    ${HEADERS_PREFIX}/systeminfo/qsysteminfoglobal_p.h
)

if(UNIX)
    list(APPEND HEADERS
        ${HEADERS_PREFIX}/systeminfo/linux/qbatteryinfo_linux_p.h
        ${HEADERS_PREFIX}/systeminfo/linux/qdeviceinfo_linux_p.h
        ${HEADERS_PREFIX}/systeminfo/linux/qnetworkinfo_linux_p.h
        ${HEADERS_PREFIX}/systeminfo/linux/qofonowrapper_p.h
        ${HEADERS_PREFIX}/systeminfo/linux/qscreensaver_linux_p.h
        ${HEADERS_PREFIX}/systeminfo/linux/qstorageinfo_linux_p.h
        ${HEADERS_PREFIX}/systeminfo/linux/qudevwrapper_p.h
    )
endif()

if(WIN32)
    list(APPEND HEADERS
        ${HEADERS_PREFIX}/systeminfo/windows/qbatteryinfo_win_p.h
        ${HEADERS_PREFIX}/systeminfo/windows/qdeviceinfo_win_p.h
        ${HEADERS_PREFIX}/systeminfo/windows/qnetworkinfo_win_p.h
        ${HEADERS_PREFIX}/systeminfo/windows/qscreensaver_win_p.h
        ${HEADERS_PREFIX}/systeminfo/windows/qstorageinfo_win_p.h
        ${HEADERS_PREFIX}/systeminfo/windows/qwmihelper_win_p.h
    )
endif()

set(QRC ${PROJECT}.qrc)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
    ${QRC}
)

set_target_properties(${PROJECT} PROPERTIES VERSION ${PROJECT_VERSION})

add_dependencies(${PROJECT} tools actiontools)

if(UNIX)
    include(FindPkgConfig)
    pkg_search_module(LIBNOTIFY REQUIRED libnotify)
    pkg_search_module(BLUEZ REQUIRED bluez)
endif()

target_include_directories(${PROJECT}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../actiontools/include
        ${LIBNOTIFY_INCLUDE_DIRS}
        ${BLUEZ_INCLUDE_DIRS}
)

apply_definitions(${PROJECT})

find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS
    Network
    Script
    Sql
    Widgets
    DBus
    Multimedia
    MultimediaWidgets
    TextToSpeech
REQUIRED)

target_link_libraries(${PROJECT}
    Qt5::Network
    Qt5::Script
    Qt5::Sql
    Qt5::Widgets
    Qt5::DBus
    Qt5::Multimedia
    Qt5::MultimediaWidgets
    Qt5::TextToSpeech
    ${LIBNOTIFY_LIBRARIES}
    ${BLUEZ_LIBRARIES}
)