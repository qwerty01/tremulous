add_library (
    sol STATIC
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