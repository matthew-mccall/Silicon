function(add_module)
    add_library(${ARGV})
    target_link_libraries(${MODULE_NAME} PRIVATE ${SI_DEPENDENCIES})
endfunction(add_module)