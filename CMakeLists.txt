cmake_minimum_required(VERSION 3.10)

project(actiona VERSION 3.10.1 DESCRIPTION "Cross-platform automation tool" LANGUAGES CXX)

# Constants
set(ACT_SCRIPT_VERSION 1.1.0)
set(ACT_MINIMUM_QT_VERSION 5.6)
set(ACT_MINIMUM_BOOST_VERSION 1.65)

# Build parameters
set(ACT_BUILD_NAME "" CACHE STRING "Build name")
option(ACT_UPDATER "Build the Updater" ON)

include(CheckIPOSupported)
check_ipo_supported(RESULT result)
if(result)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)
endif()

function(apply_definitions TARGET)
target_compile_definitions(${TARGET} PRIVATE
    QT_USE_FAST_CONCATENATION
    QT_USE_FAST_OPERATOR_PLUS
    QT_NO_CAST_TO_ASCII
    QT_NO_CAST_FROM_ASCII
    QT_NO_CAST_FROM_BYTEARRAY
    ACT_VERSION="${PROJECT_VERSION}"
    ACT_SCRIPT_VERSION="${ACT_SCRIPT_VERSION}"
    ACT_BUILD_NAME="${ACT_BUILD_NAME}"
    $<$<BOOL:${ACT_UPDATER}>:ACT_UPDATER>
)
endfunction()

add_subdirectory(tools)
add_subdirectory(actiontools)
add_subdirectory(execution)
add_subdirectory(executer)
add_subdirectory(gui)
add_subdirectory(actions)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    # TMP add_subdirectory(tests)
endif()
