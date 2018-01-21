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

include_directories(
	${CURL_INCLUDE_DIRS}
)