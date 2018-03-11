set(CMAKE_INSTALL_NAME_DIR ${PROJECT_BINARY_DIR}/gpp)

set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src)
set(QC_SOURCE_DIR ${ENGINE_DIR}/qcommon)
set(BGAME_SOURCE_DIR ${ENGINE_DIR}/bgame)
#set(RC_SOURCE_DIR ${ENGINE_DIR}/renderercommon)
#set(CLIENT_SOURCE_DIR ${ENGINE_DIR}/client)

add_definitions( -DGAME )

set( GAME_SOURCES
    g_main.cpp
    ${BGAME_SOURCE_DIR}/bg_alloc.cpp
    ${BGAME_SOURCE_DIR}/bg_lib.cpp
    ${BGAME_SOURCE_DIR}/bg_lib.h
    ${BGAME_SOURCE_DIR}/bg_local.h
    ${BGAME_SOURCE_DIR}/bg_misc.cpp
    ${BGAME_SOURCE_DIR}/bg_pmove.cpp
    ${BGAME_SOURCE_DIR}/bg_public.h
    ${BGAME_SOURCE_DIR}/bg_slidemove.cpp
    ${BGAME_SOURCE_DIR}/bg_voice.cpp
    g_active.cpp
    g_admin.cpp
    g_admin.h
    g_buildable.cpp
    g_client.cpp
    g_cmds.cpp
    g_combat.cpp
    g_local.h
    g_maprotation.cpp
    g_misc.cpp
    g_missile.cpp
    g_mover.cpp
    g_namelog.cpp
    g_physics.cpp
    g_playermodel.cpp
    g_public.h
    g_session.cpp
    g_spawn.cpp
    g_svcmds.cpp
    g_target.cpp
    g_team.cpp
    g_trigger.cpp
    g_utils.cpp
    g_weapon.cpp
    g_weapondrop.cpp
    tremulous.h
    ${QC_SOURCE_DIR}/q_shared.h 
    ${QC_SOURCE_DIR}/q_shared.cpp
    ${QC_SOURCE_DIR}/q_math.cpp
    )

add_library( game SHARED ${GAME_SOURCES} g_syscalls.cpp )
IF (WIN32)
SET_TARGET_PROPERTIES( game PROPERTIES LINK_FLAGS "/DEF:\"${ENGINE_DIR}/game/game.def" )
ENDIF (WIN32)	

if(WIN32 AND MSVC)
	if(CMAKE_CL_64)
		add_definitions( -D_CRT_SECURE_NO_WARNINGS -D__AMD64__ -D_AMD64_ -D__WIN64__ -D_WIN64 )
	else()
		add_definitions( -D_WIN32 -DWIN32 )
	endif()
endif()

target_include_directories(
    game PUBLIC
    ${QC_SOURCE_DIR}
    ${BGAME_SOURCE_DIR}
    ${ENGINE_DIR}
    )

include( ${CMAKE_SOURCE_DIR}/cmake/AddQVM.cmake )
add_qvm( game ${GAME_SOURCES} g_syscalls.asm )

IF (APPLE AND UNIX)
add_custom_command(
    TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/libgame${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gpp/game${CMAKE_SHARED_LIBRARY_SUFFIX}
    )
ENDIF (APPLE AND UNIX)
