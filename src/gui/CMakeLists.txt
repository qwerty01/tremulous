#
##  _   _ ___    ____          _      
## | | | |_ _|  / ___|___   __| | ___ 
## | | | || |  | |   / _ \ / _` |/ _ \
## | |_| || |  | |__| (_) | (_| |  __/
##  \___/|___|  \____\___/ \__,_|\___|
##                                    
#

set(CMAKE_INSTALL_NAME_DIR ${PROJECT_BINARY_DIR}/gpp)

set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)
set(BGAME_SOURCE_DIR ${ENGINE_DIR}/bgame)
set(QC_SOURCE_DIR ${ENGINE_DIR}/qcommon)
set(RC_SOURCE_DIR ${ENGINE_DIR}/renderercommon)
set(CLIENT_SOURCE_DIR ${ENGINE_DIR}/client)

add_definitions( -DUI )

#add_custom_command(
#    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/menudef.h
#    COMMAND ${CMAKE_COMMAND}
#    ARGS -E copy ${CMAKE_BINARY_DIR}/assets/ui/menudef.h ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/menudef.h
#    DEPENDS ${CMAKE_BINARY_DIR}/assets/ui/menudef.h
#    )
#set_source_files_properties(menudef.h PROPERTIES GENERATED TRUE)

set(UI_SOURCES
    gui_main.c # Must be listed first!
    gui_atoms.c
    gui_gameinfo.c
    gui_local.h
    gui_shared.c
    gui_shared.h
    ${CMAKE_SOURCE_DIR}/assets/ui/menudef.h
    ${BGAME_SOURCE_DIR}/bg_lib.h
    ${BGAME_SOURCE_DIR}/bg_public.h
    ${BGAME_SOURCE_DIR}/bg_alloc.c
    ${BGAME_SOURCE_DIR}/bg_lib.c
    ${BGAME_SOURCE_DIR}/bg_misc.c
    ${BGAME_SOURCE_DIR}/bg_voice.c
    ${QC_SOURCE_DIR}/q_shared.h
    ${QC_SOURCE_DIR}/q_shared.c
    ${QC_SOURCE_DIR}/q_math.c
    ${RC_SOURCE_DIR}/tr_types.h
    ${CLIENT_SOURCE_DIR}/keycodes.h
    )

add_library(
    ui SHARED
    ${UI_SOURCES}
    gui_syscalls.c
    )

target_include_directories(
    ui PUBLIC
    ${QC_SOURCE_DIR}
    ${BGAME_SOURCE_DIR}
	${ENGINE_DIR}
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


include(${CMAKE_SOURCE_DIR}/cmake/AddQVM.cmake)
add_qvm( ui ${UI_SOURCES} gui_syscalls.asm )

add_custom_command(
    TARGET ui POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/libui${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gpp/ui${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
