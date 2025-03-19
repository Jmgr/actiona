cmake_minimum_required(VERSION 3.10)

project(actiona VERSION 3.11.0 DESCRIPTION "Cross-platform automation tool" LANGUAGES CXX)

# Constants
set(ACT_SCRIPT_VERSION 1.1.0)
set(ACT_MINIMUM_QT_VERSION 6.4)
set(ACT_MINIMUM_QT_VERSION_HEX 0x060400)

# Build parameters
set(ACT_BUILD_NAME "" CACHE STRING "Build name")
option(ACT_UPDATER "Build the Updater" ON)
option(ACT_PROFILE "Enable profiling" OFF)

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

set(CMAKE_CXX_STANDARD 17)
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
        $<$<PLATFORM_ID:Windows>:WINVER=0x0A00>
        $<$<PLATFORM_ID:Windows>:_WIN32_WINNT=0x0A00>
    )
    set_target_properties(${TARGET}
        PROPERTIES
            PREFIX ""
    )
endfunction()

function(compile_translations TARGET)
    find_package(Qt6 ${ACT_MINIMUM_QT_VERSION} COMPONENTS LinguistTools REQUIRED)

    set(TS_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/translations/${TARGET}_fr_FR.ts
        ${CMAKE_CURRENT_SOURCE_DIR}/translations/${TARGET}_zh_CN.ts
        # Add other translations here
    )

    set(QM_OUTPUT_DIR ${CMAKE_BINARY_DIR}/translations)
    file(MAKE_DIRECTORY ${QM_OUTPUT_DIR})

    qt6_add_translation(QM_FILES ${TS_FILES})

    set(QM_FILES_MOVED)

    foreach(QM_FILE ${QM_FILES})
        get_filename_component(QM_BASENAME ${QM_FILE} NAME)
        set(QM_TARGET_FILE ${QM_OUTPUT_DIR}/${QM_BASENAME})

        add_custom_command(
            OUTPUT ${QM_TARGET_FILE}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QM_FILE} ${QM_TARGET_FILE}
            DEPENDS ${QM_FILE}
        )

        list(APPEND QM_FILES_MOVED ${QM_TARGET_FILE})
    endforeach()

    # Ensure translation files are built and moved correctly
    add_custom_target(${TARGET}_translations ALL DEPENDS ${QM_FILES_MOVED})

    set(QM_FILES ${QM_FILES} PARENT_SCOPE)

    add_custom_target(${TARGET}_update_translations
        COMMAND Qt6::lupdate
        -locations relative
        -no-obsolete
        ${CMAKE_CURRENT_SOURCE_DIR} # Source files directory
        -ts ${TS_FILES}             # Update these .ts files
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endfunction()

add_custom_target(update_all_translations)
foreach(subdir IN ITEMS tools actiontools execution actexec actiona actionpackdata actionpackdevice actionpackinternal actionpacksystem actionpackwindows)
    add_dependencies(update_all_translations ${subdir}_update_translations)
endforeach()

install(
    DIRECTORY ${CMAKE_BINARY_DIR}/translations
    DESTINATION share/actiona/
)

add_subdirectory(tools)
add_subdirectory(actiontools)
add_subdirectory(execution)
add_subdirectory(actexec)
add_subdirectory(actiona)
add_subdirectory(actions)
