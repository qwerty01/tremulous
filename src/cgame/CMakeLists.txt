set(CMAKE_INSTALL_NAME_DIR ${PROJECT_BINARY_DIR}/gpp)

set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)
set(BGAME_SOURCE_DIR ${ENGINE_DIR}/bgame)
set(QC_SOURCE_DIR ${ENGINE_DIR}/qcommon)
set(GUI_SOURCE_DIR ${ENGINE_DIR}/gui)
set(RC_SOURCE_DIR ${ENGINE_DIR}/renderercommon)

add_definitions( -DCGAME )

set( CGAME_SOURCES
    cg_main.cpp
    cg_public.h
    cg_local.h
    binaryshader.h
    ${BGAME_SOURCE_DIR}/bg_lib.h
    ${BGAME_SOURCE_DIR}/bg_public.h
    ${BGAME_SOURCE_DIR}/bg_alloc.cpp
    ${BGAME_SOURCE_DIR}/bg_lib.cpp
    ${BGAME_SOURCE_DIR}/bg_misc.cpp
    ${BGAME_SOURCE_DIR}/bg_voice.cpp
    ${BGAME_SOURCE_DIR}/bg_pmove.cpp
    ${BGAME_SOURCE_DIR}/bg_slidemove.cpp
    cg_animation.cpp
    cg_animmapobj.cpp
    cg_attachment.cpp
    cg_buildable.cpp
    cg_consolecmds.cpp
    cg_draw.cpp
    cg_drawtools.cpp
    cg_ents.cpp
    cg_event.cpp
    cg_marks.cpp
    cg_particles.cpp
    cg_players.cpp
    cg_playerstate.cpp
    cg_predict.cpp
    cg_rangemarker.cpp
    cg_scanner.cpp
    cg_servercmds.cpp
    cg_snapshot.cpp
    cg_trails.cpp
    cg_tutorial.cpp
    cg_view.cpp
    cg_weapons.cpp
    #
    ${GUI_SOURCE_DIR}/gui_shared.h
    ${GUI_SOURCE_DIR}/gui_shared.cpp
    #
    ${QC_SOURCE_DIR}/q_shared.h
    ${QC_SOURCE_DIR}/q_shared.cpp
    ${QC_SOURCE_DIR}/q_math.cpp
    #
    ${RC_SOURCE_DIR}/tr_types.h
    )

add_library( cgame SHARED ${CGAME_SOURCES} cg_syscalls.cpp )

target_include_directories(
    cgame PUBLIC
    ${BGAME_SOURCE_DIR}
    ${QC_SOURCE_DIR}
    ${RC_SOURCE_DIR}
    ${GUI_SOURCE_DIR}
	${ENGINE_DIR}
    )

include( ${CMAKE_SOURCE_DIR}/cmake/AddQVM.cmake )
add_qvm( cgame ${CGAME_SOURCES} cg_syscalls.asm )

if(WIN32 AND MSVC)
	SET_TARGET_PROPERTIES( cgame PROPERTIES LINK_FLAGS "/DEF:\"${ENGINE_DIR}/cgame/cgame.def" )

	if(CMAKE_CL_64)
		add_definitions( -D_CRT_SECURE_NO_WARNINGS -D__AMD64__ -D_AMD64_ -D__WIN64__ -D_WIN64 )
	else()
		add_definitions( -D_WIN32 -DWIN32 )
	endif()
endif()

IF (APPLE AND UNIX)
add_custom_command(
    TARGET cgame POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/libcgame${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gpp/cgame${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ENDIF(APPLE AND UNIX)
