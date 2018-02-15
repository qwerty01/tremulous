#
##  ____                                           _      
## / ___|__ _  __ _ _ __ ___   ___    ___ ___   __| | ___ 
##| |   / _` |/ _` | '_ ` _ \ / _ \  / __/ _ \ / _` |/ _ \
##| |__| (_| | (_| | | | | | |  __/ | (_| (_) | (_| |  __/
## \____\__, |\__,_|_| |_| |_|\___|  \___\___/ \__,_|\___|
##      |___/                                             
#

set(CMAKE_INSTALL_NAME_DIR ${PROJECT_BINARY_DIR}/gpp)

set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)
set(BGAME_SOURCE_DIR ${ENGINE_DIR}/bgame)
set(QC_SOURCE_DIR ${ENGINE_DIR}/qcommon)
set(GUI_SOURCE_DIR ${ENGINE_DIR}/gui)
set(RC_SOURCE_DIR ${ENGINE_DIR}/renderercommon)

add_definitions( -DCGAME )

set( CGAME_SOURCES
    cg_main.c # Must be listed first
    cg_public.h
    cg_local.h
    binaryshader.h
    ${BGAME_SOURCE_DIR}/bg_lib.h
    ${BGAME_SOURCE_DIR}/bg_public.h
    ${BGAME_SOURCE_DIR}/bg_alloc.c
    ${BGAME_SOURCE_DIR}/bg_lib.c
    ${BGAME_SOURCE_DIR}/bg_misc.c
    ${BGAME_SOURCE_DIR}/bg_voice.c
    ${BGAME_SOURCE_DIR}/bg_pmove.c
    ${BGAME_SOURCE_DIR}/bg_slidemove.c
    cg_animation.c
    cg_animmapobj.c
    cg_attachment.c
    cg_buildable.c
    cg_consolecmds.c
    cg_draw.c
    cg_drawtools.c
    cg_ents.c
    cg_event.c
    cg_marks.c
    cg_particles.c
    cg_players.c
    cg_playerstate.c
    cg_predict.c
    cg_rangemarker.c
    cg_scanner.c
    cg_servercmds.c
    cg_snapshot.c
    cg_trails.c
    cg_tutorial.c
    cg_view.c
    cg_weapons.c
    #
    ${GUI_SOURCE_DIR}/gui_shared.h
    ${GUI_SOURCE_DIR}/gui_shared.c
    #
    ${QC_SOURCE_DIR}/q_shared.h
    ${QC_SOURCE_DIR}/q_shared.c
    ${QC_SOURCE_DIR}/q_math.c
    #
    ${RC_SOURCE_DIR}/tr_types.h
    )

add_library( cgame SHARED ${CGAME_SOURCES} cg_syscalls.c )
SET_TARGET_PROPERTIES( cgame PROPERTIES LINK_FLAGS "/DEF:\"${ENGINE_DIR}/cgame/cgame.def" )

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


add_custom_command(
    TARGET cgame POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/libcgame${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gpp/cgame${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
