file(GLOB XDRS "*.x")
foreach (GENERATE_XDR ${XDRS})
    get_filename_component(XDRNAME ${GENERATE_XDR} NAME_WE)
    set(OUT_NAME ${CMAKE_CURRENT_SOURCE_DIR}/${XDRNAME})

    list(APPEND XDR_COMPILED_FILES ${OUT_NAME}.h)

    add_custom_command(OUTPUT ${OUT_NAME}.h
            COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xdrc -hh -o ${OUT_NAME}.h ${GENERATE_XDR}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Compiling ${GENERATE_XDR}")

endforeach (GENERATE_XDR)

add_custom_target(xdr-compile DEPENDS ${XDR_COMPILED_FILES})

set_source_files_properties(${XDR_COMPILED_FILES} PROPERTIES GENERATED TRUE)

message(STATUS "${XDR_COMPILED_FILES}")

