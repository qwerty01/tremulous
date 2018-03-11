set(CMAKE_INSTALL_NAME_DIR ${PROJECT_BINARY_DIR}/gpp)

set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)
set(BGAME_SOURCE_DIR ${ENGINE_DIR}/bgame)
set(QC_SOURCE_DIR ${ENGINE_DIR}/qcommon)
set(RC_SOURCE_DIR ${ENGINE_DIR}/renderercommon)
set(CLIENT_SOURCE_DIR ${ENGINE_DIR}/client)

add_definitions( -DUI )

IF (APPLE AND UNIX)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/menudef.h
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${CMAKE_BINARY_DIR}/assets/ui/menudef.h ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/menudef.h
    DEPENDS ${CMAKE_BINARY_DIR}/assets/ui/menudef.h
    )
set_source_files_properties(menudef.h PROPERTIES GENERATED TRUE)
ENDIF (APPLE AND UNIX)

set(UI_SOURCES
    gui_main.cpp
    gui_atoms.cpp
    gui_gameinfo.cpp
    gui_local.h
    gui_shared.cpp
    gui_shared.h
    ${CMAKE_SOURCE_DIR}/assets/ui/menudef.h
    ${BGAME_SOURCE_DIR}/bg_lib.h
    ${BGAME_SOURCE_DIR}/bg_public.h
    ${BGAME_SOURCE_DIR}/bg_alloc.cpp
    ${BGAME_SOURCE_DIR}/bg_lib.cpp
    ${BGAME_SOURCE_DIR}/bg_misc.cpp
    ${BGAME_SOURCE_DIR}/bg_voice.cpp
    ${QC_SOURCE_DIR}/q_shared.h
    ${QC_SOURCE_DIR}/q_shared.cpp
    ${QC_SOURCE_DIR}/q_math.cpp
    ${RC_SOURCE_DIR}/tr_types.h
    ${CLIENT_SOURCE_DIR}/keycodes.h
    )

add_library(
    ui SHARED
    ${UI_SOURCES}
    gui_syscalls.cpp
    )

target_include_directories(
    ui PUBLIC
    ${QC_SOURCE_DIR}
    ${BGAME_SOURCE_DIR}
	${ENGINE_DIR}
    )

	
if(WIN32 AND MSVC)
    SET_TARGET_PROPERTIES( ui PROPERTIES LINK_FLAGS "/DEF:\"${ENGINE_DIR}/gui/gui.def" )
    if(CMAKE_CL_64)
         add_definitions( -D_CRT_SECURE_NO_WARNINGS -D__AMD64__ -D_AMD64_ -D__WIN64__ -D_WIN64 )
    else()
         add_definitions( -D_WIN32 -DWIN32 )
    endif()
endif()


include(${CMAKE_SOURCE_DIR}/cmake/AddQVM.cmake)
add_qvm( ui ${UI_SOURCES} gui_syscalls.asm )

IF (APPLE AND UNIX)
add_custom_command(
    TARGET ui POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/libui${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gpp/gui${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ENDIF (APPLE AND UNIX)
