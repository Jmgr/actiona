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
    ${HEADERS_PREFIX}/system_global.hpp
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

set(TS_PREFIX translations)
set(TS_FILES
    ${TS_PREFIX}/actionpacksystem_fr_FR.ts
    ${TS_PREFIX}/actionpacksystem_zh_CN.ts
)

set(QRC ${PROJECT}.qrc)

add_library(${PROJECT} SHARED
    ${SOURCES}
    ${HEADERS}
    ${TS_FILES}
    ${QRC}
)

add_dependencies(${PROJECT} tools actiontools)

if(UNIX)
    include(FindPkgConfig)
    pkg_search_module(LIBNOTIFY REQUIRED libnotify)
    pkg_search_module(BLUEZ REQUIRED bluez)
    pkg_search_module(UDEV REQUIRED libudev)
endif()

setup_target(${PROJECT})

target_compile_definitions(${PROJECT} PRIVATE ACTIONPACKSYSTEM_LIBRARY)

find_package(Qt6 ${ACT_MINIMUM_QT_VERSION} COMPONENTS
    Qml
    Network
    Widgets
    DBus
    Multimedia
    MultimediaWidgets
    TextToSpeech
REQUIRED)

target_link_directories(${PROJECT}
    PRIVATE
        ${CMAKE_BINARY_DIR}/3rdparty/qtjsapi/out
)

target_include_directories(${PROJECT}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/systeminfo
        ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../actiontools/include
        ${LIBNOTIFY_INCLUDE_DIRS}
        ${BLUEZ_INCLUDE_DIRS}
        ${UDEV_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT}
    PRIVATE
    actiontools
    Qt6::Network
    Qt6::Widgets
    Qt6::DBus
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    Qt6::TextToSpeech
    ${LIBNOTIFY_LIBRARIES}
    ${BLUEZ_LIBRARIES}
    $<$<PLATFORM_ID:Windows>:Bthprops>
    $<$<PLATFORM_ID:Windows>:Ws2_32>
    $<$<PLATFORM_ID:Windows>:PowrProf>
    $<$<PLATFORM_ID:Windows>:Vfw32>
    $<$<PLATFORM_ID:Windows>:setupapi>
)

if(UNIX)
    target_link_libraries(${PROJECT}
        PUBLIC
        ${UDEV_LIBRARIES}
    )
endif()

compile_translations(${PROJECT})

install(TARGETS ${PROJECT} DESTINATION ${ACT_ACTIONS_INSTALL_DIR})
