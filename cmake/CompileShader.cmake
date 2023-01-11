function(CompileShader PATH)
    add_custom_command(
            COMMAND ${Vulkan_GLSLC_EXECUTABLE} -V ${PATH} -o ${CMAKE_CURRENT_BINARY_DIR}/${PATH}.spv
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${PATH}.spv
            DEPENDS ${PATH}
    )
endfunction()