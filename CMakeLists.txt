cmake_minimum_required(VERSION 3.10)

project(actiona VERSION 3.10.2 DESCRIPTION "Cross-platform automation tool" LANGUAGES CXX)

# Constants
set(ACT_SCRIPT_VERSION 1.1.0)
set(ACT_MINIMUM_QT_VERSION 5.12)
set(ACT_MINIMUM_QT_VERSION_HEX 0x050C00)

# Build parameters
set(ACT_BUILD_NAME "" CACHE STRING "Build name")
option(ACT_UPDATER "Build the Updater" ON)
option(ACT_PROFILE "Enable profiling" OFF)

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

function(setup_target TARGET)
    target_compile_definitions(${TARGET} PRIVATE
        QT_USE_QSTRINGBUILDER
        QT_NO_CAST_TO_ASCII
        QT_NO_CAST_FROM_ASCII
        QT_NO_CAST_FROM_BYTEARRAY
	QT_DISABLE_DEPRECATED_BEFORE=${ACT_MINIMUM_QT_VERSION_HEX}
        ACT_VERSION="${PROJECT_VERSION}"
        ACT_SCRIPT_VERSION="${ACT_SCRIPT_VERSION}"
        ACT_BUILD_NAME="${ACT_BUILD_NAME}"
        $<$<BOOL:${ACT_UPDATER}>:ACT_UPDATER>
        $<$<BOOL:${ACT_PROFILE}>:ACT_PROFILE>
        $<$<PLATFORM_ID:Windows>:UNICODE>
        $<$<PLATFORM_ID:Windows>:_UNICODE>
        $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN>
        $<$<PLATFORM_ID:Windows>:NOMINMAX>
        $<$<PLATFORM_ID:Windows>:WINVER=_WIN32_WINNT_WIN7>
    )
    set_target_properties(${TARGET}
        PROPERTIES
            PREFIX ""
    )
endfunction()

function(compile_translations TARGET)
    find_package(Qt5 ${ACT_MINIMUM_QT_VERSION} COMPONENTS LinguistTools REQUIRED)
    set(TS_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/translations/${TARGET}_fr_FR.ts
        ${CMAKE_CURRENT_SOURCE_DIR}/translations/${TARGET}_zh_CN.ts
        # Add other translations here
    )
    set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_BINARY_DIR}/translations)
    qt5_add_translation(QM_FILES ${TS_FILES})
    add_custom_target(${TARGET}_translations ALL DEPENDS ${QM_FILES})
endfunction()

add_subdirectory(tools)
add_subdirectory(actiontools)
add_subdirectory(execution)
add_subdirectory(executer)
add_subdirectory(gui)
add_subdirectory(actions)
