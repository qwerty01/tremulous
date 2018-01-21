set(PARENT_DIR ${CMAKE_SOURCE_DIR}/src)

#if(USE_RESTCLIENT)
#set(SOURCES
#    http_client.h
#    )
#endif(USE_RESTCLIENT)

add_library(
    script_api STATIC
    client.h
    cvar.h
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
    ${PARENT_DIR}
    ${CMAKE_SOURCE_DIR}
    # these are in global namespace
    ${CMAKE_SOURCE_DIR}/external/lua-5.3.3/include
    ${CMAKE_SOURCE_DIR}/external/sol
    # these have they're own namespace
    ${CMAKE_SOURCE_DIR}/external/nettle-3.3
    ${CMAKE_SOURCE_DIR}/external/rapidjson
    )

target_link_libraries(script_api lua)
