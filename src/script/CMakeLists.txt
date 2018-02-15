set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

if( MSVC ) # Check if we are using the Visual Studio compiler
add_definitions(
    -D__AMD64__ 
	-D__WIN64__ 
	-D_WIN64
	)
endif()

add_library(
    script_api STATIC
    client.h
    cvar.h
    http_client.h
    lnettlelib.c
    lnettlelib.h
    lnettlelib.h
    nettle.h
    rapidjson.h
    rapidjson/document.cpp
    rapidjson/file.hpp
    rapidjson/luax.hpp
    rapidjson/rapidjson.cpp
    rapidjson/schema.cpp
    rapidjson/userdata.hpp
    rapidjson/values.cpp
    rapidjson/values.hpp
    )


include_directories(
    # these are in global namespace
    ${EXTERNAL_DIR}/lua-5.3.3/include
    ${EXTERNAL_DIR}/sol
    # these have they're own namespace
    ${EXTERNAL_DIR}/nettle-3.3
    ${EXTERNAL_DIR}/rapidjson
	${ENGINE_DIR}/qcommon
	${ENGINE_DIR}
    )

target_link_libraries(
    script_api
    lua
    )
