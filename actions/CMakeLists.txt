function(setup_action TARGET)
    set_target_properties(${TARGET}
        PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/actions"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/actions"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/actions"
    )
endfunction()

add_subdirectory(data)
add_subdirectory(device)
add_subdirectory(internal)
add_subdirectory(system)
add_subdirectory(windows)