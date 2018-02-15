set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)

find_package(CURL)

add_library(
    restclient
    restclient/connection.h
    restclient/helpers.h
    restclient/restclient.h
    restclient/version.h
    connection.cpp
    helpers.cpp
    restclient.cpp
    )
	
if(WIN32 AND MSVC)
	if(CMAKE_CL_64)
		add_definitions(
			-D_CRT_SECURE_NO_WARNINGS
			-D__AMD64__
			-D_AMD64_
			-D__WIN64__ 
			-D_WIN64
			)
	else()
		add_definitions(
			-_WIN32
			-WIN32
			)
	endif()
endif()

include_directories(
	${ENGINE_DIR}
	${CURL_INCLUDE_DIR}
    )
	
target_link_libraries(
    restclient
    ${CURL_LIBRARIES}
    )


