set(PROJECT xrectsel)

project(${PROJECT} LANGUAGES C)

set(SOURCES
    main.c
)

add_executable(${PROJECT}
    ${SOURCES}
)

set_target_properties(${PROJECT} PROPERTIES OUTPUT_NAME "backend-xrectsel")
set_target_properties(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../..)

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
endif()

target_include_directories(${PROJECT}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/xrectsel
)

target_link_libraries(
    ${PROJECT}
    ${X11_LIBRARIES}
)
