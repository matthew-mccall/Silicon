function(add_module)
    set(LOCAL_SRC ${SRC})

    foreach(FILE ${ARGV})
        list(APPEND LOCAL_SRC "src/modules/${MODULE_NAME}/${FILE}")
    endforeach(FILE)

    set(LOCAL_MODULE_SRC ${LOCAL_SRC} PARENT_SCOPE)
endfunction(add_module)