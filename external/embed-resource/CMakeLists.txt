cmake_minimum_required(VERSION 3.5)
project(EmbedResource)
set(CMAKE_CXX_FLAGS -std=c++14)

function(embed_resources out_var)
    set(RESULT)
    foreach(in_f ${ARGN})
        file(RELATIVE_PATH src_f ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${in_f})
        set(out_f "${PROJECT_BINARY_DIR}/${in_f}.h")
        get_filename_component(out_d ${out_f} DIRECTORY)
        file(MAKE_DIRECTORY ${out_d})
        add_custom_command(OUTPUT ${out_f}
                COMMAND embed-resource ${out_f} ${src_f}
                DEPENDS ${in_f}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMENT "Building binary file for embedding ${out_f}"
                VERBATIM)
        list(APPEND result ${out_f})
    endforeach()
    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

add_executable(embed-resource embedresource.cpp)
target_link_libraries(embed-resource)