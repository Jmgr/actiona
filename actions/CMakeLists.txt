function(setup_action TARGET)
    set_target_properties(${TARGET}
        PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/actions"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/actions"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/actions"
    )
endfunction()

add_subdirectory(actionpackdata)
add_subdirectory(actionpackdevice)
add_subdirectory(actionpackinternal)
add_subdirectory(actionpacksystem)
add_subdirectory(actionpackwindows)